Pour compiler : "make" créé un fichier myos.iso.
make start utilises qemu pour executer l'os.

Les outils nécéssaire sont :
-un cross compilateur avec une cible générique (si son nom est différent du mien, il suffit de changer la variable CC du makefile pour gcc et AS pour as.)
-xorriso pour créer le fichier iso
-qemu pour les architectures x86.


Les fichiers sont théoriquement répartis de la manière suivante :
-les ficheirs spécifiques à l'architecture (drivers, boot, gestion des pages) sont dans arch/i386
-les fichiers spécifiques au fonctionnement du noyau sont dans kernel
-les fichiers pour des additions exterieurs au noyau (shell et ses programmes globalement) sont dans modules
-les fichiers implémentant des fonctions qui auraient vocation à être dans une librairie standard sont dans lib.

Les headers sont rassemblées sauf pour les librairies. Certains headers n'ont pas de sources (io.h).

Le rapport est au format pdf.
