# Testeur différentiel 42_sh (Python, parallèle)

Compare `42_sh` à `bash --posix` sur chaque ligne de test : **stdout**,
**code de sortie** et **fichiers de sortie** (fail strict), plus **stderr** en
avertissement. Chaque test tourne dans son propre bac à sable isolé, donc
l'exécution est **parallèle** (`-j`).

## Usage

```bash
./tester.py                          # tout, en parallèle (nb cœurs)
./tester.py suite                    # une catégorie (cases/suite/*.tests)
./tester.py cases/suite/10_pipes.tests # un fichier précis
./tester.py -v                       # sous valgrind (+ classement erreurs)
./tester.py -i                       # environnement minimal (comme env -i)
./tester.py -j 8                     # 8 workers
./tester.py -t 5                     # timeout 5 s par test
./tester.py --no-build               # ne pas recompiler 42_sh
./tester.py --skip-preflight         # ne pas sonder stdin/stdout d'abord
```

**Locale** : auto-détectée (`en_US.UTF-8` si dispo, sinon `C.UTF-8`/`C`). Forcer
une locale absente ferait cracher à bash un `warning: setlocale...` sur stderr à
chaque run - tous les `error msg` différeraient alors pour rien.

**Préflight** : avant la suite, le testeur sonde que 42_sh lit bien un
pipe-in (stdin) et peut être pipé en sortie (stdout). S'il échoue, on n'exécute
pas les milliers de tests pour rien (contourner avec `--skip-preflight`).

`make -C ../../42_sh NAME=42_sh_test` est lancé automatiquement (sauf
`--no-build`), puis le binaire de test est copié en local (`./42_sh`).

## Arborescence

```
tester/
├── tester.py            # exécutable unique
├── README.md
├── sh42_tester/         # package du testeur (voir sh42_tester/README.md)
├── cases/               # jeux de tests, 1 ligne = 1 test
│   ├── suite/           # suite principale (01_echo.tests ... 15_edge_cases.tests)
│   └── special/         # cas .toml : heredocs, interruptions
├── cases_legacy/        # anciens jeux (common, lapinou, wildcat, bonus)
├── fixtures/            # fichiers d'entrée -> symlinkés dans test_files/ des sandboxes
└── manual/              # scénarios manuels (heredoc, signaux...) NON différentiels
```

## Ajouter des tests

Ajouter une ligne dans un `.tests` existant, ou créer un
`cases/<catégorie>/<nom>.tests` (découvert automatiquement, aucune liste à
régénérer). Les commandes peuvent écrire dans `./outfiles/` et lire
`./test_files/` (fournis par le bac à sable). `./test_files/invalid_permission`
est un fichier `chmod 000` pour tester les erreurs de permission.

## Statuts par test

Quatre colonnes `O C D E` (Output, exit Code, Diff outfiles, Error msg) : `·`
si l'axe concorde, sinon la lettre de l'axe divergent (en rouge). `V` : fuite
valgrind (exit 99). `!` suivi des symboles : erreur critique classée (segfault,
invalid read/write...).
