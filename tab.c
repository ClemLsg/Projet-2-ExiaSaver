#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>

#include "tab.h"


int tab_editeur(char **A, int Xinit, int Yinit, char *file){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
	int X,Y, i=0, compteur=0, stop=0;
	char nb_magique[1024]="", ligne[1024]="", cara, *token, longueur[1024]="", largeur[1024]="";
	FILE* fichier = NULL;

	pid_t pid = fork();
    if (pid == 0)
    {	
    	fichier = fopen(file, "r");
    	exit(0);
    }
    else if (pid > 0)
    {	
    	wait(NULL);

    }
    else
    {
        // fork failed
        return 1;
    }

    fichier = fopen(file, "r");

	X = Xinit;
	Y = Yinit;

    while(fgets(ligne,1024,fichier)!= NULL){

    	if (strchr(ligne, '#') == NULL){
    		compteur++;
    		switch(compteur){
    		case 1:
    			strcpy(nb_magique,ligne);
    			break;
    		case 2:
    			token = strtok(ligne," ");
	    		strcpy(longueur,token);
	    		int longu = atoi(longueur);
	    		token = strtok(NULL," ");
    			strcpy(largeur,token);
    			int larg = atoi(largeur);

    			if(Xinit == 666){
    				Xinit = (w.ws_col-longu)/2;
    				X = Xinit;


    			}
    			if(Yinit == 666 && stop==0){
    				Y = (w.ws_row-larg)/2;

    				stop =1;
    			}
    			break;
    		default:
    			if(Y<=0){
    				for(Y; Y<=0; Y++){
    					fgets(ligne,1024,fichier);
    				}
    			}
    			token = strtok(ligne," ");
    			while(token != NULL){
    				if(X<=w.ws_col || Y<=w.ws_row){
    					if(Y>=w.ws_row) return 0;
	    				if(strncmp(token, "0",1) == 0){
	    					A[Y][X]=' ';
	    					X++;
	    					i++;}
	    				if(strncmp(token, "1",1) == 0){
	    					A[Y][X]='X';
	    					X++;
	    					i++;}
	    				if(strncmp(token, "2",1) == 0){
	    					X++;
	    					i++;}
	    				if(i>=longu){
	    					Y++;
	    					X=Xinit;
	    					i=0;}
	    			token = strtok(NULL," ");

    				}
    			}
    			break;
			}
		}
	}
	fclose(fichier); 
}

void tab_afficheur(char **A){
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int I,J;
	
	for (I=0; I<w.ws_row; I++)  
	  {
	   for (J=0; J<w.ws_col; J++)  
	          printf("%c", A[I][J]);
	
	   printf("\n");         
	  }
}

void tab_clear(char **A){
	struct winsize w; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); 
	int i,I,J;
	for (I=0; I<w.ws_row; I++)
	for (J=0; J<w.ws_col; J++){
	          A[I][J] =' ';}
	      }