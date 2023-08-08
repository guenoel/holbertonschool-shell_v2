# holbertonschool-shell_v2
Exercice to recode shell
hola hhh

## main.c
entrée de programme où l'invite est affichée et l'entrée de l'utilisateur est gérée, la fonction main contrôlera la boucle principale du shell, où les lignes d'entrée seront lues, tokenisées et les commandes exécutées.
## prompt.c
La fonction pour afficher l'invite et lire l'entrée de l'utilisateur, vous pouvez utiliser la fonction getline ou quelque chose de similaire pour obtenir les lignes d'entrée.
## tokenizer.c
fonctions pour diviser les lignes d'entrée en jetons ; utiliser la fonction strtok
## executor.c
fonctions pour exécuter les commandes, où nous gérerions l'exécution des commandes internes (telles que exit, cd, env, setenv, unsetenv) et des commandes externes à l'aide de la fonction execve (les fonctions de vérification nous autorisent ou créent les nôtres)
## builtin.c
contient des implémentations de commandes intégrées, telles que exit, cd, env, setenv, unsetenv. Chacune de ces commandes pourrait être une fonction distincte dans ce fichier.
## redirection.c
les redirections d'entrée et de sortie (<, >, >>). des fonctions pour détecter et appliquer ces redirections lorsqu'une commande est exécutée ?
## pipes.c
pour gérer l'exécution des commandes dans les pipes (|). Ici, vous divisez la ligne de commande en commandes individuelles et vous les reliez.
## logical_operators.c
vous implémenteriez les fonctions pour gérer les opérateurs logiques && et || qui vous permettent d'exécuter des commandes de manière conditionnelle.
## utilities.c
fonctions utilitaires ici, telles que les fonctions d'impression des messages d'erreur, de gestion de la mémoire dynamique, de manipulation des variables d'environnement, etc.
### exemple:
_strcmp() : Pour comparer deux chaînes.
_strdup() : pour dupliquer une chaîne en mémoire dynamique.
_strcat() : pour concaténer deux chaînes.
_strchr() : pour rechercher un caractère dans une chaîne.
_strlen() : pour obtenir la longueur d'une chaîne.
Makefile : compilez et liez tous les fichiers source pour créer l'interpréteur.