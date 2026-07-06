#!/usr/bin/env python3
import base64
import fcntl
import http.server
import json
import os
import pty
import queue
import re
import select
import signal
import struct
import subprocess
import sys
import termios
import threading

ANSI = re.compile(r"\x1b\[[0-9;?]*[ -/]*[@-~]")
CTRL = re.compile(r"[\r\x00-\x08\x0b-\x1f]")

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.join(os.path.dirname(os.path.dirname(HERE)), "42_sh")
DBG_DIR = "/tmp/sh42_debug"
PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 8765
STATIC = {
    "/": ("debugview.html", "text/html; charset=utf-8"),
    "/debugview.html": ("debugview.html", "text/html; charset=utf-8"),
    "/debugview.css": ("debugview.css", "text/css; charset=utf-8"),
    "/debugview.js": ("debugview.js", "application/javascript; charset=utf-8"),
}


class Shell:
    def __init__(self):
        self.fd = None
        self.pid = None
        self.rows = 40
        self.cols = 110
        self.subs = []
        self.lock = threading.Lock()
        self.start()

    def start(self):
        pid, fd = pty.fork()
        if pid == 0:
            os.environ["SH42_DEBUG"] = "1"
            os.chdir(ROOT)
            os.execvp("./42_sh", ["./42_sh"])
            os._exit(1)
        self.pid, self.fd = pid, fd
        self._apply_size()
        threading.Thread(target=self._reader, daemon=True).start()

    def _apply_size(self):
        try:
            fcntl.ioctl(self.fd, termios.TIOCSWINSZ,
                        struct.pack("HHHH", self.rows, self.cols, 0, 0))
        except OSError:
            pass

    def resize(self, rows, cols):
        self.rows, self.cols = rows, cols
        self._apply_size()

    def _reader(self):
        fd = self.fd
        while True:
            try:
                r, _, _ = select.select([fd], [], [], 0.2)
                if fd not in r:
                    continue
                data = os.read(fd, 8192)
            except OSError:
                break
            if not data:
                break
            with self.lock:
                for q in self.subs:
                    q.put(data)

    def subscribe(self):
        q = queue.Queue()
        with self.lock:
            self.subs.append(q)
        return q

    def unsubscribe(self, q):
        with self.lock:
            if q in self.subs:
                self.subs.remove(q)

    def write(self, data):
        try:
            os.write(self.fd, data)
        except OSError:
            pass

    def restart(self):
        try:
            os.kill(self.pid, signal.SIGKILL)
            os.waitpid(self.pid, 0)
        except OSError:
            pass
        try:
            os.close(self.fd)
        except OSError:
            pass
        self.start()


SHELL = None


def clear_debug():
    try:
        for name in os.listdir(DBG_DIR):
            if not name.startswith("."):
                try:
                    os.remove(os.path.join(DBG_DIR, name))
                except OSError:
                    pass
    except OSError:
        pass


def read_debug():
    out = {}
    try:
        names = sorted(os.listdir(DBG_DIR))
    except OSError:
        names = []
    for name in names:
        if name.startswith("."):
            continue
        try:
            with open(os.path.join(DBG_DIR, name),
                      encoding="utf-8", errors="replace") as f:
                out[name] = f.read()
        except OSError:
            pass
    return out


class Handler(http.server.BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def log_message(self, *a):
        pass

    def _bytes(self, code, ctype, body):
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        if self.path in STATIC:
            name, ctype = STATIC[self.path]
            try:
                with open(os.path.join(HERE, name), "rb") as f:
                    return self._bytes(200, ctype, f.read())
            except OSError:
                return self._bytes(404, "text/plain", b"missing")
        if self.path.startswith("/data"):
            return self._bytes(200, "application/json",
                               json.dumps(read_debug()).encode())
        if self.path.startswith("/stream"):
            return self._stream()
        return self._bytes(404, "text/plain", b"not found")

    def do_POST(self):
        n = int(self.headers.get("Content-Length", "0"))
        body = self.rfile.read(n) if n else b""
        if self.path.startswith("/input"):
            SHELL.write(body)
            return self._bytes(200, "text/plain", b"ok")
        if self.path.startswith("/resize"):
            try:
                d = json.loads(body or b"{}")
                SHELL.resize(int(d.get("rows", 40)), int(d.get("cols", 110)))
            except (ValueError, TypeError):
                pass
            return self._bytes(200, "text/plain", b"ok")
        if self.path.startswith("/restart"):
            SHELL.restart()
            clear_debug()
            return self._bytes(200, "text/plain", b"ok")
        if self.path.startswith("/compile"):
            p = subprocess.run(["make"], cwd=ROOT, capture_output=True,
                               text=True)
            out = CTRL.sub("", ANSI.sub("", p.stdout + p.stderr))
            noise = ("stty:", "expr:", "/bin/sh:")
            out = "\n".join(l for l in out.split("\n") if l.strip()
                            and not l.lstrip().startswith(noise))
            ok = "error:" not in out.lower() and "make: ***" not in out
            body = json.dumps({"ok": ok, "out": out[-4000:]}).encode()
            return self._bytes(200, "application/json", body)
        return self._bytes(404, "text/plain", b"not found")

    def _stream(self):
        self.send_response(200)
        self.send_header("Content-Type", "text/event-stream")
        self.send_header("Cache-Control", "no-store")
        self.send_header("Connection", "keep-alive")
        self.end_headers()
        q = SHELL.subscribe()
        try:
            while True:
                try:
                    data = q.get(timeout=15)
                except queue.Empty:
                    self.wfile.write(b": ping\n\n")
                    self.wfile.flush()
                    continue
                payload = base64.b64encode(data).decode()
                self.wfile.write(f"data: {payload}\n\n".encode())
                self.wfile.flush()
        except (BrokenPipeError, ConnectionResetError, OSError):
            pass
        finally:
            SHELL.unsubscribe(q)


def main():
    global SHELL
    os.chdir(ROOT)
    SHELL = Shell()
    srv = http.server.ThreadingHTTPServer(("127.0.0.1", PORT), Handler)
    print(f"http://127.0.0.1:{PORT}", flush=True)
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        pass
    try:
        os.kill(SHELL.pid, signal.SIGKILL)
    except OSError:
        pass


if __name__ == "__main__":
    main()
