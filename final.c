#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>


int tab_editeur(char **A, int Xinit, int Yinit, char *file){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
	int X,Y, i=0, compteur=0;
	char nb_magique[1024]="", ligne[1024]="", cara, *token, longueur[1024]="", largeur[1024]="";
	X = Xinit;
	Y = Yinit;
	FILE* fichier = NULL;
	fichier = fopen(file, "r");

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
    			break;
    		default:
    			if(Y<=0){
    				for(Y; Y<=0; Y++){
    					fgets(ligne,1024,fichier);
    				}
    			}
    			token = strtok(ligne," ");
    			printf("%s",token);
    			while(token != NULL){
    				if(X<=w.ws_col || Y<=w.ws_row){
    					if(Y>=w.ws_row) return 0;
	    				if(strncmp(token, "0",1) == 0){
	    					printf("pass 0");
	    					A[Y][X]=' ';
	    					X++;
	    					i++;
	    				}
	    				if(strncmp(token, "1",1) == 0){
	    					A[Y][X]='X';
	    					X++;
	    					i++;
	    				}
	    				if(i>=longu){
	    					Y++;
	    					X=Xinit;
	    					i=0;
	    				}
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
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
	int I,J;
		/*/* Pour chaque ligne ... */
	for (I=0; I<w.ws_row; I++)  
	  {
	   /* ... considérer chaque composante */
	   for (J=0; J<w.ws_col; J++)  
	          printf("%c", A[I][J]);
	   /* Retour à la ligne */
	   printf("\n");         
	  }
}




int main(int argc, char *argv[]){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
    int i,I,J;

/*CREATION DU TABLEAU A DEUX DIMENSIONS */

	char **A=malloc(sizeof(char*)*  w.ws_col*w.ws_row);
	for(i=0; i<w.ws_row; i++)
    {
        A[i] = malloc(w.ws_col *  w.ws_row * sizeof(char));}

	/* Pour chaque ligne ... */
	for (I=0; I<w.ws_row; I++)
	  /* ... considérer chaque composante */
	  for (J=0; J<w.ws_col; J++){
	          A[I][J] =' ';}
	system("clear");
	// tab_afficheur(A);
	tab_editeur(A,10,10,"screen1.pbm");
	tab_editeur(A,100,-5,"screen1.pbm");
	tab_afficheur(A);





	return 0;
}



