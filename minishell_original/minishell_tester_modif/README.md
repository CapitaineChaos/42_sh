# Installation

Cloner le repos dans le dossier du projet minishell

lancer la version classique :

```bash
bash minitester.sh -o
```

lancer la version valgrind :

```bash
bash minitester.sh -o -v
```




Pour ajouter des tests :

- Soit dans des fichiers .tests existants
Dans ce cas simplement ajouter la nouvelle commande dans une ligne

- Soit un nouveau fichier .tests
Après l'ajout du fichier il faut lancer

```bash
bash autoload_lists.sh
```




Il faut aussi permettre au minishelle d'accepter les commandes pipées dans le stdin
```C
if (isatty(STDIN_FILENO))
{
  readline.....
}
```