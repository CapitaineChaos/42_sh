const term = new Terminal({
  fontFamily: "JetBrains Mono, DejaVu Sans Mono, Consolas, monospace",
  fontSize: 13,
  cursorBlink: true,
  allowProposedApi: true,
  theme: { background: "#000000", foreground: "#cdd6f4" },
});
const fit = new FitAddon.FitAddon();
term.loadAddon(fit);
const uni = new Unicode11Addon.Unicode11Addon();
term.loadAddon(uni);
term.unicode.activeVersion = "11";
term.open(document.getElementById("term"));
fit.fit();

const STATE = (() => {
  const d = { astMode: "full", collapsed: {}, maximized: null,
    selectedKey: null, leftFlex: null, inspectCollapsed: false };
  try { return { ...d, ...JSON.parse(localStorage.getItem("dbgview") || "{}") }; }
  catch (e) { return d; }
})();
function saveState() {
  try { localStorage.setItem("dbgview", JSON.stringify(STATE)); } catch (e) {}
}

const left = document.getElementById("left");
if (STATE.leftFlex) {
  left.style.flex = STATE.leftFlex;
  fit.fit();
} else if (term.cols > 0) {
  const w = left.getBoundingClientRect().width;
  left.style.flex = "0 0 " + Math.round((w * 80) / term.cols) + "px";
  fit.fit();
}

function postResize() {
  fetch("/resize", {
    method: "POST",
    body: JSON.stringify({ rows: term.rows, cols: term.cols }),
  });
}
postResize();
let refitTimer = null;
function refit() {
  fit.fit();
  clearTimeout(refitTimer);
  refitTimer = setTimeout(postResize, 120);
}
window.addEventListener("resize", refit);
if (window.ResizeObserver) {
  let first = true;
  new ResizeObserver(() => { if (first) { first = false; return; } refit(); })
    .observe(document.getElementById("term"));
}

const gutter = document.getElementById("gutter");
let dragging = false;
gutter.addEventListener("mousedown", (e) => { dragging = true; e.preventDefault(); });
window.addEventListener("mousemove", (e) => {
  if (!dragging) return;
  const rect = document.getElementById("main").getBoundingClientRect();
  const w = Math.max(200, Math.min(e.clientX - rect.left, rect.width - 200));
  left.style.flex = "0 0 " + w + "px";
  fit.fit();
});
window.addEventListener("mouseup", () => {
  if (dragging) {
    dragging = false;
    STATE.leftFlex = left.style.flex;
    saveState();
    postResize();
  }
});

function b64bytes(s) {
  const bin = atob(s);
  const a = new Uint8Array(bin.length);
  for (let i = 0; i < bin.length; i++) a[i] = bin.charCodeAt(i);
  return a;
}
const es = new EventSource("/stream");
es.onmessage = (e) => term.write(b64bytes(e.data));
term.onData((d) => fetch("/input", { method: "POST", body: d }));

document.getElementById("compile").onclick = async () => {
  const j = await (await fetch("/compile", { method: "POST" })).json();
  const box = document.getElementById("compilebox");
  if (j.ok) {
    box.hidden = true;
    box.textContent = "";
  } else {
    box.textContent = (j.out || "erreur").trim();
    box.hidden = false;
  }
};
document.getElementById("restart").onclick = async () => {
  postResize();
  await fetch("/restart", { method: "POST" });
  term.reset();
  postResize();
};

const US = "\x1f";
const RS = "\x1e";
const seen = {};
const INSPECT = {};
let footerLast = null;
let selectedKey = STATE.selectedKey;
let astMode = STATE.astMode;

const NODE_COLORS = {
  CMD: "#fab387", PIPE: "#89b4fa", AND: "#a6e3a1", OR: "#f9e2af",
  HEREDOC: "#94e2d5", SUBSHELL: "#cba6f7", SUBSH_MARK: "#cba6f7",
  SUBSH_END: "#cba6f7", SEQ: "#b4befe", UNKNOWN: "#6c7086",
};
const FLAG_HELP = [
  ["q", "partie quotée"],
  ["$", "expansion de variable"],
  ["~", "expansion tilde"],
  ["*", "motif glob"],
];
const IC_CHEVRON =
  `<svg class="ic" viewBox="0 0 16 16" aria-hidden="true"><path d="M4 6l4 4 4-4"/></svg>`;
const IC_EXPAND =
  `<svg class="ic" viewBox="0 0 16 16" aria-hidden="true"><path d="M6 3H3v3M13 6V3h-3M6 13H3v-3M13 10v3h-3"/></svg>`;

function esc(s) {
  return s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
}
function color(s) {
  s = esc(s);
  s = s.replace(/\b[A-Z][A-Z0-9_]+\b/g, (m) => `<span class="type">${m}</span>`);
  s = s.replace(/\[\s*\d+\]/g, (m) => `<span class="dim">${m}</span>`);
  s = s.replace(/·/g, `<span class="accent">·</span>`);
  return s;
}
function title(n) {
  const m = n.match(/^\d+[_-]?(.*)$/);
  return (m ? m[1] : n).replace(/_/g, " ");
}
function list(items, cls) {
  return `<ul class="ilist ${cls}">` +
    items.map((v) => `<li>${color(v)}</li>`).join("") + "</ul>";
}
function cardList(items, cls) {
  return `<div class="ilist ${cls}">` +
    items.map((v) => `<div class="irow">${color(v)}</div>`).join("") + "</div>";
}
function flagsHTML(fl) {
  return [...fl].map((c) =>
    c === "-" ? `<span class="fx">·</span>` : `<span class="fon">${c}</span>`
  ).join("");
}

function nodeBody(nd) {
  const col = NODE_COLORS[nd.title] || NODE_COLORS.UNKNOWN;
  const argv = nd.argv ? nd.argv.split(US).filter((x) => x.length) : [];
  const redirs = nd.redirs ? nd.redirs.split(US).filter((x) => x.length) : [];
  if (astMode === "simple") {
    let s = `<div class="nhead" style="background:${col}">${nd.title}</div>`;
    if (argv.length || redirs.length) {
      s += `<div class="nsimple">`;
      if (argv.length) s += `<span class="sargv">${color(argv.join(" "))}</span>`;
      if (redirs.length) s += `<span class="sredir">${redirs.length}r</span>`;
      s += `</div>`;
    }
    return s;
  }
  let h = `<div class="nhead" style="background:${col}">${nd.title}</div>`;
  h += `<div class="nbody">`;
  if (argv.length)
    h += `<div class="sec"><div class="lbl">argv</div>${cardList(argv, "alist")}</div>`;
  if (redirs.length)
    h += `<div class="sec"><div class="lbl">redir · ${redirs.length}</div>` +
      `${cardList(redirs, "rlist")}</div>`;
  if (!argv.length && !redirs.length) h += `<div class="empty">—</div>`;
  h += `</div>`;
  const meta = nd.meta ? nd.meta.split(US).filter((x) => x.length) : [];
  if (meta.length) h += `<div class="nfoot">${meta.join("  ·  ")}</div>`;
  return h;
}
function treeHTML(text, name) {
  const lines = text.split("\n").filter((l) => l.length);
  if (!lines.length) return "";
  const nodes = lines.map((l) => {
    const f = l.split("\t");
    const nd = {
      depth: +f[0] || 0, kind: f[1] || "cmd", title: f[2] || "",
      argv: f[3] || "", redirs: f[4] || "", meta: f[5] || "", children: [],
    };
    const m = (nd.meta.match(/id=(-?\d+)/) || [])[1];
    nd.key = `ast:${name}:${m != null ? m : nodes.length}`;
    INSPECT[nd.key] = { kind: "node", ...nd };
    return nd;
  });
  const roots = [];
  const stack = [];
  for (const nd of nodes) {
    while (stack.length && stack[stack.length - 1].depth >= nd.depth) stack.pop();
    (stack.length ? stack[stack.length - 1].children : roots).push(nd);
    stack.push(nd);
  }
  const li = (nd) =>
    `<li><div class="node" data-key="${nd.key}">${nodeBody(nd)}</div>` +
    (nd.children.length ? `<ul>${nd.children.map(li).join("")}</ul>` : "") +
    "</li>";
  return `<ul>${roots.map(li).join("")}</ul>`;
}

function tokenTable(text, name) {
  const rows = text.split("\n").filter((l) => l.length);
  let h = `<div class="hscroll"><table class="ttab"><thead><tr><th>#</th>` +
    `<th>GROUP</th><th>TYPE</th><th>FLAG</th><th>TEXT</th></tr></thead><tbody>`;
  for (const l of rows) {
    const f = l.split("\t");
    const o = {
      kind: "tok", idx: f[0] || "", group: f[1] || "", type: f[2] || "",
      flags: f[3] || "----", text: f[4] || "", count: f[5] || "0",
      parts: f[6] || "",
    };
    const key = `tok:${name}:${o.idx}`;
    INSPECT[key] = o;
    h += `<tr data-key="${key}"><td class="ti">${esc(o.idx)}</td>` +
      `<td>${esc(o.group)}</td><td class="type">${esc(o.type)}</td>` +
      `<td class="tf">${flagsHTML(o.flags)}</td>` +
      `<td class="tt">${color(o.text)}</td></tr>`;
  }
  if (!rows.length) h += `<tr><td colspan="5" class="empty">aucun token</td></tr>`;
  return h + `</tbody></table></div>`;
}

function execRow(l) {
  const f = l.split("\t");
  return {
    kind: "exec", seq: f[0] || "", cmd: f[1] || "", argc: f[2] || "",
    path: f[3] || "", exit: f[4] || "", status: f[5] || "", pid: f[6] || "",
    args: f[7] || "", redirs: f[8] || "",
  };
}
function execTable(text, name) {
  const rows = text.split("\n").filter((l) => l.length);
  let h = `<div class="hscroll"><table class="ttab etab"><thead><tr><th>#</th>` +
    `<th>CMD</th><th>EXIT</th><th>STATUS</th><th>PID</th>` +
    `<th>ARGS + REDIRS</th></tr></thead><tbody>`;
  rows.forEach((l, i) => {
    const o = execRow(l);
    const key = `exec:${name}:${i}`;
    INSPECT[key] = o;
    const dash = (v) => (v ? esc(v) : `<span class="fx">-</span>`);
    const sig = +o.exit > 128 && +o.exit <= 192
      ? ` <span class="dim">sig ${+o.exit - 128}</span>` : "";
    const args = o.args ? o.args.split("\x1f").filter((x) => x.length) : [];
    const redirs = o.redirs ? o.redirs.split("\x1f").filter((x) => x.length) : [];
    const cell = args.map((a) => `<div class="erow">${color(a)}</div>`).join("") +
      redirs.map((r) => `<div class="erow erd">${color(r)}</div>`).join("");
    h += `<tr data-key="${key}"><td class="ti">${esc(o.seq)}</td>` +
      `<td class="type">${esc(o.cmd)}</td>` +
      `<td class="tt">${esc(o.exit)}${sig}</td>` +
      `<td class="tt">${dash(o.status)}</td>` +
      `<td class="tt">${dash(o.pid)}</td>` +
      `<td class="tt ecell">${cell || `<span class="fx">-</span>`}</td></tr>`;
  });
  if (!rows.length)
    h += `<tr><td colspan="6" class="empty">aucune exécution</td></tr>`;
  return h + `</tbody></table></div>`;
}

function nodesTable(text, name) {
  const rows = text.split("\n").filter((l) => l.length);
  let h = `<div class="hscroll"><table class="ttab"><thead><tr><th>#</th>` +
    `<th>KIND</th><th>CONTENT</th><th>REDIRS</th></tr></thead><tbody>`;
  rows.forEach((l, i) => {
    const f = l.split("\t");
    const o = {
      kind: "nodei", idx: f[0] || "", k: f[1] || "op", label: f[2] || "",
      redirs: f[3] || "", type: f[4] || "", arity: f[5] || "", prec: f[6] || "",
    };
    const key = `nodei:${name}:${i}`;
    INSPECT[key] = o;
    const content = o.k === "cmd"
      ? o.label.split("\x1f").filter((x) => x.length).join(" ") : o.label;
    const rd = o.redirs.split("\x1f").filter((x) => x.length).join(" ");
    h += `<tr data-key="${key}"><td class="ti">${esc(o.idx)}</td>` +
      `<td class="type">${esc(o.k.toUpperCase())}</td>` +
      `<td class="tt">${color(content)}</td>` +
      `<td class="tt">${rd ? color(rd) : `<span class="fx">-</span>`}</td></tr>`;
  });
  if (!rows.length) h += `<tr><td colspan="4" class="empty">aucun nœud</td></tr>`;
  return h + `</tbody></table></div>`;
}

function kvRows(pairs) {
  return `<table class="kv">` + pairs.map(([k, v]) =>
    `<tr><td class="k">${k}</td><td class="v">${v}</td></tr>`).join("") +
    `</table>`;
}
function inspectToken(o) {
  const parts = o.parts ? o.parts.split(US).filter((p) => p.length) : [];
  const on = [...o.flags].map((c, i) =>
    c !== "-" ? FLAG_HELP[i][1] : null).filter(Boolean);
  let h = `<div class="ihead"><span class="ibadge type">${esc(o.type)}</span>` +
    `<span class="idx">#${esc(o.idx)}</span></div>`;
  h += kvRows([
    ["group", esc(o.group)],
    ["flags", `${flagsHTML(o.flags)}${on.length ? " — " + on.join(", ") : ""}`],
    ["count", esc(o.count)],
    ["text", `<code>${color(o.text)}</code>`],
  ]);
  if (parts.length) {
    h += `<div class="lbl">parts</div><table class="ptab"><thead><tr>` +
      `<th>type</th><th>off</th><th>flag</th><th>text</th></tr></thead><tbody>`;
    for (const p of parts) {
      const s = p.split(RS);
      h += `<tr><td class="type">${esc(s[0] || "")}</td>` +
        `<td>${esc(s[1] || "")}</td><td class="tf">${flagsHTML(s[2] || "---")}</td>` +
        `<td class="tt">${color(s[3] || "")}</td></tr>`;
    }
    h += `</tbody></table>`;
  }
  return h;
}
function inspectNode(o) {
  const col = NODE_COLORS[o.title] || NODE_COLORS.UNKNOWN;
  const argv = o.argv ? o.argv.split(US).filter((x) => x.length) : [];
  const redirs = o.redirs ? o.redirs.split(US).filter((x) => x.length) : [];
  const meta = o.meta ? o.meta.split(US).filter((x) => x.length) : [];
  let h = `<div class="ihead"><span class="ibadge" style="background:${col};` +
    `color:#11111b">${esc(o.title)}</span><span class="idx">${esc(o.kind)}</span></div>`;
  h += kvRows(meta.map((m) => {
    const i = m.indexOf("=");
    return [esc(m.slice(0, i)), esc(m.slice(i + 1))];
  }));
  if (argv.length) h += `<div class="lbl">argv · ${argv.length}</div>${list(argv, "alist")}`;
  if (redirs.length) h += `<div class="lbl">redir · ${redirs.length}</div>${list(redirs, "rlist")}`;
  return h;
}
function inspectExec(o) {
  const args = o.args ? o.args.split(US).filter((x) => x.length) : [];
  const redirs = o.redirs ? o.redirs.split(US).filter((x) => x.length) : [];
  const sig = +o.exit > 128 && +o.exit <= 192 ? ` (signal ${+o.exit - 128})` : "";
  let h = `<div class="ihead"><span class="ibadge type">${esc(o.cmd)}</span>` +
    `<span class="idx">#${esc(o.seq)}</span></div>`;
  h += kvRows([
    ["exit", esc(o.exit) + sig],
    ["status", o.status ? esc(o.status) : "— (builtin)"],
    ["pid", o.pid ? esc(o.pid) : "— (builtin)"],
    ["path", o.path ? esc(o.path) : "(builtin / none)"],
    ["argc", esc(o.argc)],
  ]);
  if (args.length) h += `<div class="lbl">args · ${args.length}</div>${list(args, "alist")}`;
  if (redirs.length) h += `<div class="lbl">redir · ${redirs.length}</div>${list(redirs, "rlist")}`;
  return h;
}
function inspectNodeI(o) {
  const args = o.k === "cmd" && o.label
    ? o.label.split(US).filter((x) => x.length) : [];
  const redirs = o.redirs ? o.redirs.split(US).filter((x) => x.length) : [];
  let h = `<div class="ihead"><span class="ibadge type">${esc(o.type)}</span>` +
    `<span class="idx">#${esc(o.idx)}</span></div>`;
  const kv = [["kind", esc(o.k)], ["type", esc(o.type)], ["arity", esc(o.arity)]];
  if (o.prec) kv.push(["précédence", esc(o.prec)]);
  h += kvRows(kv);
  if (args.length) h += `<div class="lbl">argv · ${args.length}</div>${list(args, "alist")}`;
  if (redirs.length) h += `<div class="lbl">redir · ${redirs.length}</div>${list(redirs, "rlist")}`;
  return h;
}
function defaultInspect() {
  let h = `<div class="ihint">Aucun élément sélectionné.</div>`;
  h += `<div class="lbl">flags token</div><table class="kv">` +
    FLAG_HELP.map(([c, d]) =>
      `<tr><td class="k"><span class="fon">${c}</span></td><td class="v">${d}</td></tr>`
    ).join("") + `</table>`;
  h += `<div class="lbl">types de nœud</div><div class="swatches">` +
    Object.keys(NODE_COLORS).filter((k) => k !== "SUBSH_MARK" && k !== "SUBSH_END")
      .map((k) => `<span class="sw"><i style="background:${NODE_COLORS[k]}"></i>${k}</span>`)
      .join("") + `</div>`;
  return h;
}
function refreshInspect() {
  document.querySelectorAll("[data-key].selected")
    .forEach((x) => x.classList.remove("selected"));
  const box = document.getElementById("inspectbody");
  if (!box) return;
  const clear = document.getElementById("iclear");
  if (selectedKey && INSPECT[selectedKey]) {
    const o = INSPECT[selectedKey];
    const el = document.querySelector(`[data-key="${selectedKey}"]`);
    if (el) el.classList.add("selected");
    box.innerHTML = o.kind === "tok" ? inspectToken(o)
      : o.kind === "exec" ? inspectExec(o)
      : o.kind === "nodei" ? inspectNodeI(o) : inspectNode(o);
    if (clear) clear.disabled = false;
  } else {
    box.innerHTML = defaultInspect();
    if (clear) clear.disabled = true;
  }
}
document.getElementById("panels").addEventListener("click", (e) => {
  const el = e.target.closest("[data-key]");
  if (!el) return;
  const key = el.getAttribute("data-key");
  selectedKey = selectedKey === key ? null : key;
  STATE.selectedKey = selectedKey;
  saveState();
  refreshInspect();
});
document.getElementById("panels").addEventListener("click", (e) => {
  const b = e.target.closest("[data-mode]");
  if (!b) return;
  astMode = b.getAttribute("data-mode");
  STATE.astMode = astMode;
  saveState();
  document.querySelectorAll(".seg button").forEach((x) =>
    x.classList.toggle("on", x.getAttribute("data-mode") === astMode));
  for (const nm in seen)
    if (/ast/.test(nm) && seen[nm]._astText != null)
      paintAst(seen[nm], seen[nm]._astText, nm);
});
document.getElementById("panels").addEventListener("click", (e) => {
  const b = e.target.closest("[data-act]");
  if (!b) return;
  const sec = b.closest("section");
  const name = sec.getAttribute("data-name");
  const act = b.getAttribute("data-act");
  if (act === "collapse") {
    sec.classList.toggle("collapsed");
    STATE.collapsed[name] = sec.classList.contains("collapsed");
  } else if (act === "max") {
    STATE.maximized = STATE.maximized === name ? null : name;
    if (STATE.maximized) { sec.classList.remove("collapsed"); STATE.collapsed[name] = false; }
    applyMax();
  }
  saveState();
});

function applyMax() {
  const host = document.getElementById("panels");
  const on = STATE.maximized && seen[STATE.maximized];
  host.classList.toggle("has-max", !!on);
  for (const nm in seen)
    seen[nm].classList.toggle("max", nm === STATE.maximized);
}
function segHTML() {
  return `<span class="seg">` +
    `<button data-mode="full" class="${astMode === "full" ? "on" : ""}">complet</button>` +
    `<button data-mode="simple" class="${astMode === "simple" ? "on" : ""}">simple</button>` +
    `</span>`;
}
function paintAst(s, text, name) {
  s._body.innerHTML = `<div class="tree tree-${astMode}">${treeHTML(text, name)}</div>`;
  refreshInspect();
}
function render(data) {
  const names = Object.keys(data)
    .filter((n) => n !== "footer" && !n.startsWith("."))
    .sort();
  for (const n in seen)
    if (!names.includes(n)) { seen[n].remove(); delete seen[n]; }
  const host = document.getElementById("panels");
  for (const n of names) {
    if (!seen[n]) {
      const s = document.createElement("section");
      s.setAttribute("data-name", n);
      s.innerHTML =
        `<h2><button class="caret" data-act="collapse" title="replier / déplier">` +
        `${IC_CHEVRON}</button><span class="htitle">${title(n)}</span>` +
        `<span class="hctl">${/ast/.test(n) ? segHTML() : ""}` +
        `<button class="iconbtn" data-act="max" title="agrandir / réduire">` +
        `${IC_EXPAND}</button></span></h2><div class="body"></div>`;
      host.appendChild(s);
      s._body = s.querySelector(".body");
      s._last = null;
      seen[n] = s;
      if (STATE.collapsed[n]) s.classList.add("collapsed");
    }
    const s = seen[n];
    if (s._last !== data[n]) {
      s._body.classList.toggle("tree-body", /ast/.test(n));
      if (/ast/.test(n)) {
        s._astText = data[n];
        s._name = n;
        paintAst(s, data[n], n);
      } else if (/token/.test(n))
        s._body.innerHTML = tokenTable(data[n], n);
      else if (/nodes/.test(n))
        s._body.innerHTML = nodesTable(data[n], n);
      else if (/exec/.test(n))
        s._body.innerHTML = execTable(data[n], n);
      else s._body.innerHTML = `<pre>${color(data[n])}</pre>`;
      s._last = data[n];
    }
  }
  applyMax();
  refreshInspect();
  const f = data["footer"];
  if (f !== footerLast) {
    document.getElementById("footer").innerHTML = f ? color(f) : "";
    footerLast = f;
  }
}

const inspectEl = document.getElementById("inspect");
inspectEl.innerHTML =
  `<div id="inspecthead">` +
  `<button id="icollapse" class="caret" title="replier / déplier">` +
  `${IC_CHEVRON}</button><span class="ititle">inspecteur</span>` +
  `<span class="ihctl">` +
  `<button id="iclear" title="désélectionner">effacer</button>` +
  `</span></div><div id="inspectbody"></div>`;
function applyInspectCollapsed() {
  inspectEl.classList.toggle("collapsed", !!STATE.inspectCollapsed);
  refit();
}
document.getElementById("iclear").addEventListener("click", () => {
  selectedKey = null;
  STATE.selectedKey = null;
  saveState();
  refreshInspect();
});
document.getElementById("icollapse").addEventListener("click", () => {
  STATE.inspectCollapsed = !STATE.inspectCollapsed;
  applyInspectCollapsed();
  saveState();
});
applyInspectCollapsed();
refreshInspect();
async function poll() {
  try {
    const r = await fetch("/data", { cache: "no-store" });
    if (r.ok) render(await r.json());
  } catch (e) {}
  setTimeout(poll, 150);
}
poll();
