#include <stdlib.h>
#include <stdio.h>
#include <string.h>	//Bibliothèque permettant de gerer les chaines de charactère
#include <sys/ioctl.h>  
#include <unistd.h>     //Bibliothèque permettant de gerer les symbols standard
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>

#include "tab.h"


int tab_editeur(char **A, int Xinit, int Yinit, char *file, int tail){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
	int X,Y, i=0, compteur=0, stop=0, m=0; // Initialisation des variables de type INT
	char nb_magique[1024]="", ligne[1024]="", cara, *token, longueur[1024]="", chaine[1024]="", largeur[1024]="", temp; // Initialisation des variables de type CHAR
	FILE* fichier = NULL; // Initialisation de notre fichier

	pid_t pid = fork(); // Creation d'un processus fils avec une fork
    if (pid == 0)
    {	
    	fichier = fopen(file, "r"); // Ouverture du fichier par le processus fils
    	exit(0); // Fin du processus fils
    }
    else if (pid > 0)
    {	
    	wait(NULL); // Processus père en attente de la fin du processus fils

    }
    else
    {
        // fork crash
        return 1;
    }

    fichier = fopen(file, "r"); // Ouverture du fichier par le processus fils

	X = Xinit;
	Y = Yinit;

    while(fgets(ligne,1024,fichier)!= NULL){ // Lecture ligne par ligne du fichier

    	if (strchr(ligne, '#') == NULL){ // Si il n'y a pas de # dans la ligne : continue
    		compteur++;
    		switch(compteur){
    		case 1: // Si ligne 1, recuperation du nombre magique
    			strcpy(nb_magique,ligne);
    			break;
    		case 2: // Si ligne 2, recuperation de la taille de l'image
    			token = strtok(ligne," ");
	    		strcpy(longueur,token);
	    		int longu = atoi(longueur);
	    		token = strtok(NULL," ");
    			strcpy(largeur,token);
    			int larg = atoi(largeur);
			if(tail == 2){ // Si tail est égal a 2 alors la taille de l'image est doublée
			larg =larg*2;
			}

    			if(Xinit == 666){ // Si les coordonnées sont 666 : Centrage par rapport a la taille de la console
    				Xinit = (w.ws_col-longu)/2;
    				X = Xinit;


    			}
    			if(Yinit == 666 && stop==0){ // Si les coordonnées sont 666 : Centrage par rapport a la taille de la console
    				Y = (w.ws_row-larg)/2;

    				stop =1;
    			}
    			break;
    		default:
    			if(Y<=0){ // Si notre ligne devrais etre sous la case 0, passage a la ligne suivante
    				for(Y; Y<=0; Y++){
    					fgets(ligne,1024,fichier);
    				}
    			}
    			token = strtok(ligne," "); // Creation du token
    			while(token != NULL){
    				if(X<=w.ws_col || Y<=w.ws_row){ // Si les coordonnées du caractère sont bien sur la console
    					if(Y>=w.ws_row) return 0; // Si les coordonées dépassents en Y, ignoré
	    				if(strncmp(token, "0",1) == 0){ // Si c'est un 0, ecrire un espace
	    					if(tail == 2){ // Si tail égal a 2
						    A[Y][X]=' '; // Ecrire un espace aux coordonnés Y X
						    A[Y][X+1]=' '; // Ecrire un espace aux coordonnés Y X+1 pour doubler le caractère
						    strcat(chaine, "  "); // Ajout a la chaine tampon le double espace
						    X = X+2;
						    i++;
						} else { // Sinon écrire un seul espace 
						    A[Y][X]=' ';
						    X++;
						    i++;
						}}
	    				if(strncmp(token, "1",1) == 0){ // Si c'est un 1, ecrire un X
	    					if(tail == 2){ // Si tail égal a 2
						    A[Y][X]='@'; // Ecrire un @ aux coordonnés Y X
						    A[Y][X+1]='@'; // Ecrire un @ aux coordonnés Y X+1 pour doubler le caractère
						    strcat(chaine, "@@"); // Ajout a la chaine tampon le double @
						    X = X+2;
						    i++;
						} else { // Sinon écrire un seul @
						    A[Y][X]='@';
						    X++;
						    i++;
						}}
	    				if(strncmp(token, "2",1) == 0){ // Si c'est un 2, mettre un vide
	    					X++;
	    					i++;}
	    				if(i>=longu){ // Si l'on a parcouru toute la ligne
	    					if(tail == 2){ // Et que tail égal a 2
						    X=Xinit;
						    for(m = 0; m < 6; m++){ // On ajoute chaque caracrères de la chaine tampon au tableau a 2 dimensions avec un Y de différence
							temp = chaine[m];
							A[Y+1][X]= temp;
							X++;
							chaine[m] = 0;
						    }
						    Y = Y+2;
						} else { // Sinon on passe a la prochaine ligne
						    Y++;
						}
						X=Xinit;
						i=0;
					}
	    			token = strtok(NULL," "); // Changement de token

    				}
    			}
    			break; // Arret du case
			}
		}
	}
	fclose(fichier); // Fermeture du fichier
}

void tab_afficheur(char **A){
	struct winsize w; // Taille console
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);// Taille console
	int I,J;
	
	for (I=0; I<w.ws_row; I++)  // Deplacement du curseur sur les colones
	  {
	   for (J=0; J<w.ws_col; J++)  //Deplacement du curseur sur les lignes
	          printf("%c", A[I][J]); // Impression du caractere
	
	   printf("\n"); // Retour a la ligne       
	  }
}

void tab_clear(char **A){ // Remplacement de toute les cases par un espaces
	struct winsize w; // Taille console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Taille console
	int i,I,J;
	for (I=0; I<w.ws_row; I++) 
	for (J=0; J<w.ws_col; J++){
	          A[I][J] =' ';}
	      }
