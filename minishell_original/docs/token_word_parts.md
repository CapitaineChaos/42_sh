Voici la liste complète des « word parts » (sous‑tokens) dont tu auras besoin pour un shell POSIX‑compatible :

1. **LITERAL**  
   Chaîne de caractères bruts (lettres, chiffres, et tous les caractères non spéciaux) non entourés de quotes.

2. **SINGLE_QUOTED** (`'…'`)  
   Contenu littéral entre apostrophes simples : tout est pris tel quel, pas d’expansion.

3. **DOUBLE_QUOTED** (`"…"`)  
   Contenu entre guillemets doubles : permet des expansions (`$…`, `` `…` ``) et échappements, mais empêche le découpage par IFS et la plupart des globbing.

4. **ESCAPE** (`\x`)  
   Un caractère précédé d’un antislash : sert à neutraliser la sémantique spéciale du caractère suivant (notamment dans du LITERAL ou à l’intérieur de DOUBLE_QUOTED).

5. **TILDE_EXP** (`~` ou `~user`)  
   Expansion du répertoire de l’utilisateur quand c’est en tête de mot.

6. **PARAM_EXP**  
   Tout type d’expansion de paramètre :  
   - `$var`  
   - `${var}`  
   - `${var:-word}`, `${var=default}`, `${#var}`, etc.

7. **ARITH_EXP** (`$(( expression ))`)  
   Expansion arithmétique.

8. **CMD_SUBST**  
   Substitution de commande, deux syntaxes :  
   - `` `commande` ``  
   - `$(commande)`

9. **PROC_SUBST**  
   Substitution de processus (Bash/ksh extensions) :  
   - `<(commande)`  
   - `>(commande)`

10. **GLOB**  
    Métacaractères de motif simples :  
    - `*`  
    - `?`  
    - `[abc]`, `[!a-c]`, etc.

11. **EXTGLOB** (si activé)  
    Glob avancé :  
    - `@(pat1|pat2)`  
    - `*(pat)`  
    - `?(pat)`  
    - `+(pat)`  
    - `!(pat)`

12. **HEREDOC_BODY**  
    Lignes d’un here‑document (tout ce qui suit un `<<` jusqu’au délimiteur).

---

**Comment ça se combine ?**  
- Ton lexer reconnaît ces parts **à l’intérieur** d’un mot.  
- Dès qu’un mot est terminé (séparateur de champ, opérateur, fin de ligne…), tu regroupes **séquentiellement** ses parts en un unique token `WORD`.  
- Ce `WORD` est ensuite ce que ton parseur / ta première passe (« shunting‑yard » ou récursif‑descendant) consommera comme unité.

Avec cette table exhaustive, tu peux gérer **toutes** les formes de mot qu’un shell POSIX (et un peu plus, si tu veux) peut rencontrer.