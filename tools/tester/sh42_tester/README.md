# sh42_tester

Testeur différentiel : compare le comportement de `42_sh` à celui de
`bash --posix` sur un ensemble de cas de test.

## Principe

Pour chaque cas (une ligne d'un fichier `cases/**/*.tests`), la même entrée est
envoyée sur `stdin` aux deux shells, puis les résultats sont comparés sur quatre
axes indépendants :

| Axe        | Comparaison                                   |
|------------|-----------------------------------------------|
| stdout     | sortie standard (`\n` finaux ignorés)         |
| exit code  | code de sortie du processus                   |
| outfiles   | fichiers produits dans `outfiles/`            |
| stderr     | message d'erreur, normalisé (préfixe retiré)  |

Un test est `strict_ok` s'il concorde sur stdout, exit code et outfiles. La
divergence de stderr est signalée séparément (les messages d'erreur exacts ne
sont pas toujours exigés).

Les codes de sortie sont normalisés à la convention shell : un process tué par
le signal *n* (code Popen négatif) devient `128 + n` (ex. SIGINT → 130), ce qui
permet de comparer 42_sh et bash quelle que soit leur façon de terminer.

## Formats de cas

Deux extensions sont découvertes dans `cases/<catégorie>/` :

- **`.tests`** — une ligne = un cas. Simple et compact.
- **`.toml`** — cas spéciaux : entrées multi-lignes, heredocs, interruptions.

Format `.toml` (une table `[[test]]` par cas). Cas simple :

```toml
[[test]]
name = "heredoc simple"      # libellé (optionnel)
input = """
cat << EOF
bonjour
EOF
"""
```

Pour injecter des signaux, un cas est une **séquence de `steps`** jouée dans
l'ordre. Chaque step porte un seul champ :

| Champ    | Effet                                                        |
|----------|--------------------------------------------------------------|
| `send`   | octets écrits sur stdin (inclut ses propres `\n`)            |
| `signal` | signal envoyé au groupe une fois celui-ci quiescent          |
| `eof`    | ferme stdin (CTRL-D)                                         |

```toml
[[test]]
name = "CTRL-C au milieu d'un heredoc"
steps = [
  { send = "cat << EOF\nligne1\n" },
  { signal = "INT" },
  { send = "echo apres\n" },
]

[[test]]
name = "CTRL-C pendant sleep"
steps = [
  { send = "sleep 5\n" },
  { signal = "INT" },
]
```

`signal` accepte n'importe quel nom `SIGxxx` (INT, QUIT, TERM, TSTP, USR1, ...).
CTRL-\ = QUIT, CTRL-Z = TSTP. Le sucre `input` + `signal` équivaut à
`[{send=input+"\n"}, {signal=signal}]`.

### Envoi déterministe, sans chrono

Un step `signal` n'utilise **aucun timer**. Le testeur attend que le **groupe de
processus soit quiescent** (via `/proc/<pid>/stat` : plus aucun process du groupe
à l'état `R`), c'est-à-dire que le shell est bloqué en lecture (heredoc, `read`)
ou attend un enfant endormi (`sleep`), puis envoie le signal au groupe.

Ce point est reproductible et les deux shells sont traités à l'identique : la
comparaison reste donc **différentielle stricte** (stdout, exit, stderr,
outfiles) et **stable** — pas de mode « robustesse » ni de cas par cas. Un écart
constaté est un vrai écart de conformité de 42_sh, pas du bruit de timing.

Pourquoi pas un PTY interactif : le comportement interactif (réaffichage du
prompt après ^C, écho, ^\ ignoré) n'est **pas spécifié par POSIX** et rendrait la
sortie bruitée (prompts, écho, séquences readline). Hors périmètre d'une suite
différentielle POSIX.

## Isolation et parallélisme

Chaque cas s'exécute dans deux répertoires temporaires distincts (un par shell),
peuplés à l'identique depuis `fixtures/`. Les fichiers produits n'entrant jamais
en collision, la suite peut tourner en parallèle (`-j`, un `ThreadPoolExecutor`).

## Sélection pipe / TTY

Par défaut, les cas `dual` exécutent les deux axes, les cas `pipe` seulement
stdin pipe, et les cas `tty` seulement TTY. Deux options permettent de limiter
la suite à un seul monde :

```sh
./tester.py -n
./tester.py --non-interactive-only

./tester.py -I
./tester.py --interactive-only
```

Les cas incompatibles avec le mode demandé sont ignorés.

## Modules

| Module           | Responsabilité                                             |
|------------------|------------------------------------------------------------|
| `config.py`      | Chemins et constantes (locales, signaux, motifs valgrind)  |
| `case.py`        | Dataclass `Case` (entrée d'un test + interruption)         |
| `result.py`      | Dataclass `Result` (résultat d'une comparaison)            |
| `ui.py`          | Couleurs, bannières, lignes de statut, blocs d'échec, menu |
| `execution.py`   | Bac à sable, exécution des shells, comparaison             |
| `discovery.py`   | Découverte des fichiers `.tests`, lecture des cas          |
| `environment.py` | Build du binaire, sélection de locale, env, préflight      |
| `cli.py`         | Arguments, orchestration (`run_suite`, `report`, `main`)   |

Graphe de dépendances acyclique : `config`/`result` en bas, `cli` au sommet.

## Flux d'exécution

1. `cli.main` construit le binaire (`environment.build_binary`), sélectionne la
   locale et prépare l'environnement.
2. Un préflight vérifie que 42_sh fonctionne en mode non-interactif.
3. `run_suite` exécute les cas en parallèle et agrège les résultats.
4. `report` affiche le bilan et écrit le détail des échecs dans `failures.log`.
5. En cas d'échec et si le terminal est interactif, un menu propose de
   réexécuter uniquement les cas fautifs (boucle jusqu'à résolution ou abandon).

## Usage

Voir `../tester.py --help`.
