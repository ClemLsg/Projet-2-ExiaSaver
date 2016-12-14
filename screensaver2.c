#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>
#include "tab.h"

void pbm_switch(char **A, int ctime, int Xpos, int tail){ // Fonction permettant de choisir le PBM correspondant au chiffre

	char pbm_0[256] = "Images/0.pbm"; // Chargement des PBM
	char pbm_1[256] = "Images/1.pbm";
	char pbm_2[256] = "Images/2.pbm";
	char pbm_3[256] = "Images/3.pbm";
	char pbm_4[256] = "Images/4.pbm";
	char pbm_5[256] = "Images/5.pbm";
	char pbm_6[256] = "Images/6.pbm";
	char pbm_7[256] = "Images/7.pbm";
	char pbm_8[256] = "Images/8.pbm";
	char pbm_9[256] = "Images/9.pbm";

	switch(ctime){ // Switch de séléction du PBM en fonction de la valeur de ctime
			case 0:
				tab_editeur(A,Xpos,666,pbm_0, tail);
				break;
			case 1:
				tab_editeur(A,Xpos,666,pbm_1, tail);
				break;
			case 2:
				tab_editeur(A,Xpos,666,pbm_2, tail);
				break;
			case 3:
				tab_editeur(A,Xpos,666,pbm_3, tail);
				break;
			case 4:
				tab_editeur(A,Xpos,666,pbm_4, tail);
				break;
			case 5:
				tab_editeur(A,Xpos,666,pbm_5, tail);
				break;
			case 6:
				tab_editeur(A,Xpos,666,pbm_6, tail);
				break;
			case 7:
				tab_editeur(A,Xpos,666,pbm_7, tail);
				break;
			case 8:
				tab_editeur(A,Xpos,666,pbm_8, tail);
				break;
			case 9:
				tab_editeur(A,Xpos,666,pbm_9, tail);
				break;
		}
}

int main(int argc, char *argv[])
{
	int dizh, unith, dizm, unitm, dizs, units;
	int  Dur = 10; // Variable de secondes entre chaque rafraichissemtn
	int tail = 2; // Variable donnant la taille du PBM (1 pour 5x3 et 2 pour 10x6)
	char pbm;

	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
    int i,I,J;

	int hour, min, sec; // Variables pour l'heure
	char pbm_deux[256] = "Images/deuxpoints.pbm"; // Chargement des deux points

/*CREATION DU TABLEAU A DEUX DIMENSIONS */

	char **A=malloc(sizeof(char*)*  w.ws_col*w.ws_row);
	for(i=0; i<w.ws_row; i++)
    {
        A[i] = malloc(w.ws_col *  w.ws_row * sizeof(char));}

	tab_clear(A); // Initialisation du tableau a vide

	while(1){ // Boucle infinie
		time_t t = time(NULL); // Récupération de l'heure systeme
		struct tm tm = *localtime(&t);

    	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure

		hour = tm.tm_hour; // Définition des variables a l'heure
		min = tm.tm_min;
		sec = tm.tm_sec;

		dizh = hour/10; // Pour chaque heure minute et secondes on récupère la dizaine et l'unité
		unith = hour%10;
		dizm = min/10;
		unitm = min%10;
		dizs = sec/10;
		units = sec%10;

		pbm_switch(A,dizh,((w.ws_col-3)/2)-(tail*14), tail); // Lancement de la fonction pbm_switch
		pbm_switch(A,unith,((w.ws_col-3)/2)-(tail*10), tail);

		tab_editeur(A,((w.ws_col-3)/2)-(tail*7),666,pbm_deux, tail); // Ajout des deux points

		pbm_switch(A,dizm,((w.ws_col-3)/2)-(tail*4), tail);
		pbm_switch(A,unitm,((w.ws_col-3)/2), tail);

		tab_editeur(A,((w.ws_col-3)/2)+(tail*3),666,pbm_deux, tail); // Ajout des deux points

		pbm_switch(A,dizs,((w.ws_col-3)/2)+(tail*7), tail);
		pbm_switch(A,units,((w.ws_col-3)/2)+(tail*12), tail);

		tab_afficheur(A); // affichage du tableau
		printf("Cet écran sera actualisé dans quelques secondes");
		for(i = 0;i < Dur;i++){ // Chaque seconde afficher un point
	        printf(".");
	        fflush(stdout); // Force l'affichage des caractères placés en mémoire tampon par le sleep
	        sleep(1); // Antente de 1 seconde
	    }
	   tab_clear(A); // Réinitialisation du tableau
	}
    return 0;
}
