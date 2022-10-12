
# Projet CoCa 2022 : Lightup!

## Prérequis

Pour compiler et exécuter ce projet, il est nécessaire de disposer :

- de gcc
- de make
- des librairies de développement Z3. Sous debian/ubuntu, il suffit d’installer libz3-dev (z3 ne suffit pas). Sous archlinux/manjaro, le paquet z3 est suffisant. Pour les autres systèmes, je ne sais pas, si vous ne trouvez pas, travaillez au CREMI.

Le projet a été testé et fonctionne au CREMI.

## Contenu et fonctionnement

Cette archive contient :

- Le présent README
- Un exécutable solver_solution.out qui a été compilé avec la solution des enseignants, pour vous permettre de comparer avec votre solution. Il vous est conseillé de l’utiliser. Il a été compilé au CREMI (sur boursouflet), et il est donc préférable de l’utiliser là-bas.
- Un Makefile
- Un fichier de configuration doxygen
- Un dossier include contenant les fichiers .h décrivant les fonctions présentes et celles à implémenter
- Un dossier src contenant les fichiers .c. reduction.c et brute_force.c sont à compléter.
- Un dossier games contenant des exemples sur lesquels tester vos exécutables. Vous pouvez créer facilement de nouveaux exemples en allant sur [le site](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html) en générant un nouveau jeu, en cliquant sur «by game ID», et en recopiant l’ID généré (à la fin de l’URL après le #) dans un fichier. Le programme est capable de charger cette description de jeu. Il probable que le brute force souffre fortement sur les instances de tailles plus grandes que 10x10. La réduction devrait être beaucoup plus rapide.
- Un dossier example contenant Z3Example.c qui vous montre comment utiliser Z3 par l’exemple. Ce programme peut être compilé en tapant «make Z3Example».

Le programme principal se compile avec «make» et s’exécute en le lançant avec «./solver.out». L’option -h (ou l’absence d’option) font que le programme décrit ses options.

Le fichier main.c place un timeout de 30 secondes pour le brute force (cf main.c). Libre à vous de le modifier si vous voulez faire des tests plus longs (mais le programme fourni a ce timeout).
Il est attendu que le brute force soit trop lent vers des tailles supérieures à 8x8. La réduction SAT, elle est résoluble rapidement jusqu’à 100x100 (et probablement au delà, mais il est compliqué de générer des instances plus grandes).

## Travail demandé.

Il est attendu que vous complétiez les fichiers brute_force.c et reduction.c de manière cohérente avec le sujet pdf fourni.
Pour l’évaluation, la correction des algos implémentés sera avant tout prise en compte, mais la lisibilité du code sera également un facteur important.
On attendra un rapport d’implémentation présentant votre implémentation, et particulièrement un descriptif de ce qui fonctionne et ne fonctionne pas par rapport à ce qui est demandé. Il sera particulièrement peu apprécié si vous prétendez que votre algorithme fonctionne alors qu’on peut observer que ce n’est pas le cas (et que vous le pouvez, puisque vous avez un exécutable fonctionnel qui vous est fourni).
Enfin, la performance des algorithmes, quand ceux-ci sont corrects, sera légèrement prise en compte. On utilisera l’implémentation solution comme une évaluation d’une bonne performance.

Vous rendrez uniquement votre dossier src.
Dans l’hypothèse (improbable -- les fonctions de game.h devraient être suffisante à manipuler convenablement le jeu) où vous auriez besoin de rajouter des fonctions visibles extérieurement, il vous est demandé de placer les fichiers d’interface dans le dossier src.
