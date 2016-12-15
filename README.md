# Projet-2-ExiaSaver

Ici le projet 2 ExiaSaver réalisé par Clément lesage, Paul Fontaine, Enzo Billis et Antoine Morlet

Les variables d'environnement doivent être présentes !

Mettez le dossier ExiaSaver dans votre repertoire Documents.

Ensuite vous devez déclarer vos variables evironnement...
Pour ceci, modifiez le fichier etc/bash.bashrc en ajoutant les lignes suivantes à la fin du fichier tout en editant [NOM UTILISATEUR] avec le votre pour que le chemin soit le bon.:

export EXIASAVER_HOME=/home/[NOM UTILISATEUR]/Documents/ExiaSaver/
export EXIASAVER1_PBM=/home/[NOM UTILISATEUR]/Documents/ExiaSaver/Images/Statique/
export EXIASAVER2_PBM=/home/[NOM UTILISATEUR]/Documents/ExiaSaver/Images/Dynamique/
export EXIASAVER2_TAILLE=1
export EXIASAVER2_SLEEP=1
export EXIASAVER3_PBM=/home/[NOM UTILISATEUR]/Documents/ExiaSaver/Images/Interactif/

Vous devrez alors redemarrer votre machine....

Vous pouvez modifier les valeurs de EXISAVER2_TAILLE et EXIASAVER2_SLEEP.

EXIASAVER2_TAILLE donne la taille d'affichage de l'heure, ecrivez 1 pour 5x3 et 2 pour 10x6.
EXIASAVER2_SLEEP donne le temps de rafraichissement de l'horloge. Tapez 1 pour 1 seconde entre chaque affichage etc....

Redemarrez toujours quand vous modifiez vos variables d'environnement...

Maintenant mettez le fichier launcher dans le dossier bin.