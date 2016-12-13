#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>


struct pos
    {
        int x;
        int y;
};

char getch()
{
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
}

int tab_editeur(char **A, int Xinit, int Yinit, char *file){

	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
	int X,Y, i=0, compteur=0, stop=0;
	char nb_magique[1024]="", ligne[1024]="", cara, *token, longueur[1024]="", largeur[1024]="";
	FILE* fichier = NULL;
	fichier = fopen(file, "r");
	X = Xinit;
	Y = Yinit;

    while(fgets(ligne,1024,fichier)!= NULL){

    	if (strchr(ligne, '#') == NULL){
    		compteur++;
    		//printf("%d\n",compteur);
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
    				//printf("%d", X);

    			}
    			if(Yinit == 666 && stop==0){
    				Y = (w.ws_row-larg)/2;
    				//printf("%d", Y);
    				stop =1;
    			}
    			break;
    		default:
    			// printf("hey");
    			// printf(" x: %d\n", X);
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
	    					i++;
	    				}
	    				if(strncmp(token, "1",1) == 0){
	    					A[Y][X]='X';
	    					X++;
	    					i++;
	    				}
	    				if(strncmp(token, "2",1) == 0){
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

void tab_clear(char **A){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
	int i,I,J;
	for (I=0; I<w.ws_row; I++)
	  /* ... considérer chaque composante */
	for (J=0; J<w.ws_col; J++){
	          A[I][J] =' ';}
	      }



int main(int argc, char * argv[]){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
    int i,I,J,last;
    char entry;
	struct pos Pos;
	int fff = strtol(argv[1], NULL, 10);
	int ddd = strtol(argv[2], NULL, 10);
	Pos.x = fff;
	Pos.y = ddd;

/*CREATION DU TABLEAU A DEUX DIMENSIONS */

	char **A=malloc(sizeof(char*)*  w.ws_col*w.ws_row);
	for(i=0; i<w.ws_row; i++)
    {
        A[i] = malloc(w.ws_col *  w.ws_row * sizeof(char));}
	tab_clear(A);
	/* Pour chaque ligne ... */

	system("clear");
	// tab_afficheur(A);
	if(strcmp(argv[3],"0")==0){
		tab_editeur(A,Pos.x,Pos.y,"avion bas.pbm");
	}
	if(strcmp(argv[3],"1")==0){
		tab_editeur(A,Pos.x,Pos.y,"avion haut.pbm");
	}
	if(strcmp(argv[3],"2")==0){
		tab_editeur(A,Pos.x,Pos.y,"avion gauche.pbm");
	}
	if(strcmp(argv[3],"3")==0){
		tab_editeur(A,Pos.x,Pos.y,"avion droite.pbm");
	}

	tab_editeur(A,0,0,"nuage.pbm");
	tab_editeur(A,27,0,"nuage.pbm");
	tab_editeur(A,54,0,"nuage.pbm");
	tab_editeur(A,81,0,"nuage.pbm");
	tab_editeur(A,108,0,"nuage.pbm");
    tab_afficheur(A);

	while (1){
    printf("x:%d y:%d and max x%d max y:%d", Pos.x, Pos.y, w.ws_col, w.ws_row);
	entry = getch();
        switch (entry)
        {

        	case 's':
        		Pos.y++;
        		last = 0;
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1;  
				tab_clear(A); 
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion bas.pbm");        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion bas.pbm");
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion bas.pbm");
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion bas.pbm");        		
        		tab_editeur(A,Pos.x,Pos.y,"avion bas.pbm");
        		tab_editeur(A,0,0,"nuage.pbm");
        		tab_editeur(A,27,0,"nuage.pbm");
        		tab_editeur(A,54,0,"nuage.pbm");
        		tab_editeur(A,81,0,"nuage.pbm");
        		tab_editeur(A,108,0,"nuage.pbm");
        		tab_afficheur(A);
        		break;
        	case 'q':
        		Pos.x--;
        		last = 1;
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1; 
				tab_clear(A);  
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion gauche.pbm");        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion gauche.pbm");
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion gauche.pbm");
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion gauche.pbm");        		
        		tab_editeur(A,Pos.x,Pos.y,"avion gauche.pbm");
        		tab_editeur(A,0,0,"nuage.pbm");
        		tab_editeur(A,27,0,"nuage.pbm");
        		tab_editeur(A,54,0,"nuage.pbm");
        		tab_editeur(A,81,0,"nuage.pbm");
        		tab_editeur(A,108,0,"nuage.pbm");
        		tab_afficheur(A);
        		break;
        	case 'z':
        		Pos.y--;
        		last = 2;
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1;
				tab_clear(A); 
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion haut.pbm");        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion haut.pbm");
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion haut.pbm");
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion haut.pbm");
        		tab_editeur(A,Pos.x,Pos.y,"avion haut.pbm");
        		tab_editeur(A,0,0,"nuage.pbm");
        		tab_editeur(A,27,0,"nuage.pbm");
        		tab_editeur(A,54,0,"nuage.pbm");
        		tab_editeur(A,81,0,"nuage.pbm");
        		tab_editeur(A,108,0,"nuage.pbm");
        		tab_afficheur(A);
        		break;
        	case 'd':
        		Pos.x++;
        		last = 3;
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1; 
				tab_clear(A);  
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion droite.pbm");        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion droite.pbm");
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion droite.pbm");
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion droite.pbm");
        		tab_editeur(A,Pos.x,Pos.y,"avion droite.pbm");
        		tab_editeur(A,0,0,"nuage.pbm");
        		tab_editeur(A,27,0,"nuage.pbm");
        		tab_editeur(A,54,0,"nuage.pbm");
        		tab_editeur(A,81,0,"nuage.pbm");
        		tab_editeur(A,108,0,"nuage.pbm");
        		tab_afficheur(A);
        		break;
        	case '\n':
        		if(last == 0){
    			    Pos.y++;
	        		last = 0;
	        		if(Pos.x == w.ws_col) Pos.x = 1;
	        		if(Pos.y == w.ws_row) Pos.y = 1;
	        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
	        		if(Pos.y == 0) Pos.y = w.ws_row-1;  
					tab_clear(A); 
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion bas.pbm");        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion bas.pbm");
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion bas.pbm");
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion bas.pbm");        		
	        		tab_editeur(A,Pos.x,Pos.y,"avion bas.pbm");
	        		tab_editeur(A,0,0,"nuage.pbm");
	        		tab_editeur(A,27,0,"nuage.pbm");
	        		tab_editeur(A,54,0,"nuage.pbm");
	        		tab_editeur(A,81,0,"nuage.pbm");
	        		tab_editeur(A,108,0,"nuage.pbm");
	        		tab_afficheur(A);
        		}
        		if(last == 1){
	        		Pos.x--;
	        		last = 1;
	        		if(Pos.x == w.ws_col) Pos.x = 1;
	        		if(Pos.y == w.ws_row) Pos.y = 1;
	        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
	        		if(Pos.y == 0) Pos.y = w.ws_row-1; 
					tab_clear(A);  
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion gauche.pbm");        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion gauche.pbm");
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion gauche.pbm");
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion gauche.pbm");        		
	        		tab_editeur(A,Pos.x,Pos.y,"avion gauche.pbm");
	        		tab_editeur(A,0,0,"nuage.pbm");
	        		tab_editeur(A,27,0,"nuage.pbm");
	        		tab_editeur(A,54,0,"nuage.pbm");
	        		tab_editeur(A,81,0,"nuage.pbm");
	        		tab_editeur(A,108,0,"nuage.pbm");
	        		tab_afficheur(A);
        		}
        		if(last == 2){
	        		Pos.y--;
	        		last = 2;
	        		if(Pos.x == w.ws_col) Pos.x = 1;
	        		if(Pos.y == w.ws_row) Pos.y = 1;
	        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
	        		if(Pos.y == 0) Pos.y = w.ws_row-1;
					tab_clear(A); 
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion haut.pbm");        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion haut.pbm");
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion haut.pbm");
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion haut.pbm");
	        		tab_editeur(A,Pos.x,Pos.y,"avion haut.pbm");
	        		tab_editeur(A,0,0,"nuage.pbm");
	        		tab_editeur(A,27,0,"nuage.pbm");
	        		tab_editeur(A,54,0,"nuage.pbm");
	        		tab_editeur(A,81,0,"nuage.pbm");
	        		tab_editeur(A,108,0,"nuage.pbm");
	        		tab_afficheur(A);
        		}
        		if(last ==3){
	        		Pos.x++;
	        		last = 3;
	        		if(Pos.x == w.ws_col) Pos.x = 1;
	        		if(Pos.y == w.ws_row) Pos.y = 1;
	        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
	        		if(Pos.y == 0) Pos.y = w.ws_row-1; 
					tab_clear(A);  
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"avion droite.pbm");        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"avion droite.pbm");
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"avion droite.pbm");
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"avion droite.pbm");
	        		tab_editeur(A,Pos.x,Pos.y,"avion droite.pbm");
	        		tab_editeur(A,0,0,"nuage.pbm");
	        		tab_editeur(A,27,0,"nuage.pbm");
	        		tab_editeur(A,54,0,"nuage.pbm");
	        		tab_editeur(A,81,0,"nuage.pbm");
	        		tab_editeur(A,108,0,"nuage.pbm");
	        		tab_afficheur(A);
        		}
        		break;

        	case 'x':
        		exit(0);


}

}

	return 0;
}



