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

void pbm_switch(char **A, int ctime, int Xpos, int tail, char *idir){ // Fonction permettant de choisir le PBM correspondant au chiffre

	int i =0;
	char p0[254]="", p1[254]="", p2[254]="", p3[254]="", p4[254]="", p5[254]="", p6[254]="", p7[254]="", p8[254]="", p9[254]="";

	switch(ctime){ // Switch de séléction du PBM en fonction de la valeur de ctime
			case 0:
				strcpy(p0,idir);
				strcat(p0,"/0.pbm");
				tab_editeur(A,Xpos,666,p0, tail);
				break;
			case 1:
				strcpy(p1,idir);
				strcat(p1,"/1.pbm");
				tab_editeur(A,Xpos,666,p1, tail);
				break;
			case 2:
				strcpy(p2,idir);
				strcat(p2,"/2.pbm");
				tab_editeur(A,Xpos,666,p2, tail);
				break;
			case 3:
				strcpy(p3,idir);
				strcat(p3,"/3.pbm");
				tab_editeur(A,Xpos,666,p3, tail);
				break;
			case 4:
				strcpy(p4,idir);
				strcat(p4,"/4.pbm");
				tab_editeur(A,Xpos,666,p4, tail);
				break;
			case 5:
				strcpy(p5,idir);
				strcat(p5,"/5.pbm");
				tab_editeur(A,Xpos,666,p5, tail);
				break;
			case 6:
				strcpy(p6,idir);
				strcat(p6,"/6.pbm");
				tab_editeur(A,Xpos,666,p6, tail);
				break;
			case 7:
				strcpy(p7,idir);
				strcat(p7,"/7.pbm");
				tab_editeur(A,Xpos,666,p7, tail);
				break;
			case 8:
				strcpy(p8,idir);
				strcat(p8,"/8.pbm");
				tab_editeur(A,Xpos,666,p8, tail);
				break;
			case 9:
				strcpy(p9,idir);
				strcat(p9,"/9.pbm");
				tab_editeur(A,Xpos,666,p9, tail);
				break;
		}


}

int main(int argc, char *argv[])
{
	int dizh, unith, dizm, unitm, dizs, units;
	int tail = strtol(argv[1], NULL, 10); //Récuparation de la position X aléatoire
	int Dur = strtol(argv[2], NULL, 10); //Récuparation de la position X aléatoire
	char idir[254]="", pbm_deux[254]="";
	strcpy(idir,argv[3]);

	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
    int i,I,J;

	int hour, min, sec; // Variables pour l'heure
	strcpy(pbm_deux,argv[3]); // Chargement des deux points
	strcat(pbm_deux,"/deuxpoints.pbm");

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

		pbm_switch(A,dizh,((w.ws_col-3)/2)-(tail*14), tail, idir); // Lancement de la fonction pbm_switch
		pbm_switch(A,unith,((w.ws_col-3)/2)-(tail*10), tail, idir);

		tab_editeur(A,((w.ws_col-3)/2)-(tail*7),666,pbm_deux, tail); // Ajout des deux points

		pbm_switch(A,dizm,((w.ws_col-3)/2)-(tail*4), tail, idir);
		pbm_switch(A,unitm,((w.ws_col-3)/2), tail, idir);

		tab_editeur(A,((w.ws_col-3)/2)+(tail*3),666,pbm_deux, tail); // Ajout des deux points

		pbm_switch(A,dizs,((w.ws_col-3)/2)+(tail*7), tail, idir);
		pbm_switch(A,units,((w.ws_col-3)/2)+(tail*12), tail, idir);

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
