#!/usr/bin/env bash
set -euo pipefail


# ————— Réglages ————————————————————————————————————————————————
PRESENTATION_BRANCH="minishell42"   # nom de la branche de rendu
MAIN_BRANCH="main"                  # nom de la branche de référence
WORKTREE_DIR="../minishell42"
GIT_USER_NAME="AncientCodeArchitects"
GIT_USER_EMAIL="actions@github.com"
ORIG_DIR="$(pwd)"

trap 'cd "${ORIG_DIR}"' EXIT




############# Options CLI #####################################################
QUIET=false
XTRACE=false
while [[ ${1:-} =~ ^- ]]; do
  case "$1" in
    -q|--quiet)  QUIET=true  ;;
    -x|--xtrace) XTRACE=true ;;
    *) echo "Option inconnue : $1" >&2; exit 2 ;;
  esac
  shift
done

############# Helpers #########################################################
log() {
  $QUIET && return 0
  # bleu gras “➜ …”
  printf '\033[1;34m➜ %s\033[0m\n' "$*"
}

$XTRACE && set -x


###############################################################################
#                 Prépare la branche « main » pour la remise                  #
###############################################################################

log "Vérification des changements dans ${MAIN_BRANCH}…"
git -C "${ORIG_DIR}" checkout "${MAIN_BRANCH}"

if ! git -C "${ORIG_DIR}" diff --quiet || ! git -C "${ORIG_DIR}" diff --cached --quiet; then
  log "Changements détectés dans ${MAIN_BRANCH}, ajout et commit automatique…"
  git -C "${ORIG_DIR}" add -A
  git -C "${ORIG_DIR}" commit -m "✅ Commit automatique des changements avant export"
else
  log "✔️ Aucun changement à committer dans ${MAIN_BRANCH}."
fi




###############################################################################
#               Copie forcée de MAIN_BRANCH vers WORKTREE_DIR                 #
###############################################################################

log "Copie forcée de ${MAIN_BRANCH} (local) → ${WORKTREE_DIR}…"

# Purge complète du contenu de WORKTREE_DIR (sans supprimer le dossier)
mkdir -p "${WORKTREE_DIR}"
shopt -s dotglob
for item in "${WORKTREE_DIR}"/*; do
  rm -rf "${item}"
done
shopt -u dotglob

# Copie de l’état de MAIN_BRANCH vers WORKTREE_DIR (sans Git)
log "Extraction locale (sans fetch)…"
git archive --format=tar "${MAIN_BRANCH}" | tar -x -C "${WORKTREE_DIR}"

# Création de la branche de présentation dans le dépôt du WORKTREE
cd "${WORKTREE_DIR}"
if [[ ! -d .git ]]; then
  git init
fi

cd "${WORKTREE_DIR}"
git checkout -B "${PRESENTATION_BRANCH}"

log "✅ Copie complète dans ${WORKTREE_DIR} terminée."


EXCLUDE_LIST_DIRS=$(cat <<'EOF'
/.vscode
*debug*
obj
doc*
*tester*
.github
*m_debug*
EOF
)

EXCLUDE_LIST_FILES=$(cat <<'EOF'
*.log
.vignore.supp
*.o
*.d
*.a
*.out
.gitignore
*.debug
module_debug.h
Makefile.debug
minishell
EOF
)

ROOT_KEEP_LIST=$(cat <<'EOF'
/include
/src
/.git
Makefile
EOF
)


###############################################################################
#                              Machine à exclure                              #
###############################################################################


shopt -s globstar nullglob dotglob

# ————— Exclusion des Dossiers ————————————————————————
log "Exclusion des dossiers…"
while IFS= read -r pattern; do
[[ -z $pattern || $pattern =~ ^# ]] && continue
log "  motif « ${pattern} »"
if [[ $pattern == /* ]]; then
	target="${pattern:1}"
	if [[ -d $target ]]; then
	rm -rf "$target"
	log "    🗑️  $target"
	fi
elif [[ $pattern == *'*'* || $pattern == *'?'* || $pattern == *'['* ]]; then
	for d in **/$pattern; do
	[[ -d $d ]] && rm -rf "$d" && log "    🗑️  $d"
	done
else
	for d in **/"$pattern"; do
	[[ -d $d ]] && rm -rf "$d" && log "    🗑️  $d"
	done
fi
done <<< "$EXCLUDE_LIST_DIRS"


# ————— Exclusion des Fichiers ————————————————————————
log "Exclusion des fichiers…"
while IFS= read -r pattern; do
[[ -z $pattern || $pattern =~ ^# ]] && continue
log "  motif « ${pattern} »"
if [[ $pattern == /* ]]; then
	target="${pattern:1}"
	if [[ -f $target ]]; then
	rm -f "$target"
	log "    🗑️  $target"
	fi
elif [[ $pattern == *'*'* || $pattern == *'?'* || $pattern == *'['* ]]; then
	for f in **/$pattern; do
	[[ -f $f ]] && rm -f "$f" && log "    🗑️  $f"
	done
else
	for f in **/"$pattern"; do
	[[ -f $f ]] && rm -f "$f" && log "    🗑️  $f"
	done
fi
done <<< "$EXCLUDE_LIST_FILES"


shopt -u globstar nullglob dotglob

###############################################################################
#                              Machine à inclure                              #
###############################################################################


log "Nettoyage racine – garde uniquement ROOT_KEEP_LIST…"

shopt -s dotglob nullglob globstar

while IFS= read -r pattern; do
  [[ -z $pattern || $pattern =~ ^# ]] && continue
  pattern=${pattern#/}  # Retire le / initial éventuel
  KEEP_PATTERNS+=("$pattern")
done <<< "$ROOT_KEEP_LIST"

for item in ./*; do
  base=$(basename "$item")
  keep=false

  for pat in "${KEEP_PATTERNS[@]}"; do
    if [[ $pat == *'*'* || $pat == *'?'* || $pat == *'['* ]]; then
      [[ $base == $pat ]] && keep=true && break
    else
      [[ $base == $pat ]] && keep=true && break
    fi
  done

  if ! $keep; then
    rm -rf "$item"
    log "    🗑️  $item"
  fi
done

shopt -u dotglob nullglob globstar



# ————— Nettoyages de code (trace_, debug, HIDE…) ——————————
echo "🧹  Nettoyage des fichiers source…"
find . -type f \( -name '*.c' -o -name '*.h' \) | while read -r file; do
  [[ ! -s "${file}" ]] && continue
	echo "      🛠️  ${file}"
  # 1. Supprime les lignes contenant trace_ ou debug
  sed -i '/trace_/d;/debug/d' "${file}"

  # 2. Supprime les commentaires /* … */ à partir de la ligne 13
  sed -i '13,$ {s/\/\*[^*]*\*\///g; /\/\*/,/\*\//d}' "${file}"

  # 3. Supprime les blocs // >>>HIDE_START … // <<<HIDE_END
  sed -i '/\/\/ >>>HIDE_START/,/\/\/ <<<HIDE_END/d' "${file}"

  # 4. Supprime les blocs #pragma region/endregion
  sed -i '/^[[:space:]]*#[[:space:]]*pragma[[:space:]]\+region/,/^[[:space:]]*#[[:space:]]*pragma[[:space:]]\+endregion/d' "${file}"

  # 5. Détruit toute ligne commentaire // complète
  sed -i '/^[[:space:]]*\/\/.*$/d' "${file}"

  # 6. Supprime commentaire de fin de ligne après code
  sed -i 's/^\([^"]*\)\(\/\/.*\)$/\1/' "${file}"

  # 7. Trim des espaces fin de ligne
  sed -i 's/[[:space:]]\+$//' "${file}"

  # 8. Compacte lignes vides multiples
  awk 'NF { print; blank=0; next } !blank++' "${file}" > "${file}.tmp" && mv "${file}.tmp" "${file}"

  # 9. Ajoute \n final si absent
  tail -c1 "${file}" | od -An -t u1 | grep -q 10 || echo >> "${file}"
done

# Nettoie également les Makefile
log "Nettoyage des Makefile…"
find . -type f -name 'Makefile' | while read -r file; do
  sed -i '/^#[[:space:]]*>>>HIDE_START/,/^#[[:space:]]*<<<HIDE_END/d' "${file}"
done

# # ————— Commit et push ————————————————————————————————
log "Préparation du commit final…"
git config user.name  "${GIT_USER_NAME}"
git config user.email "${GIT_USER_EMAIL}"

git add -A
if git diff --cached --quiet; then
  log "✔️  Aucun changement à committer."
else
  git commit -m "✨ Branche nettoyée pour présentation"
fi

# Propagation de l'URL du dépôt distant (origin) si manquant
if ! git remote get-url origin &>/dev/null; then
  ORIGIN_URL=$(git -C "${ORIG_DIR}" remote get-url origin)
  git remote add origin "${ORIGIN_URL}"
  log "Ajout du remote 'origin' → ${ORIGIN_URL}"
fi

log "Push force vers origin/${PRESENTATION_BRANCH}"
git push origin "${PRESENTATION_BRANCH}" --force

log "✅  Fini !"
