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


int main(int argc, char * argv[]){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
    int i,I,J,last; //Déclaration des variables de type INT
    char entry; //Déclaration de la variables de type INT
	struct pos Pos;
	int posx = strtol(argv[1], NULL, 10); //Récuparation de la position X aléatoire
	int posy = strtol(argv[2], NULL, 10); //Récuparation de la position X aléatoire
	Pos.x = posx; // Injection des arguments dans la structure pos
	Pos.y = posy;

/*CREATION DU TABLEAU A DEUX DIMENSIONS */

	char **A=malloc(sizeof(char*)*  w.ws_col*w.ws_row);
	for(i=0; i<w.ws_row; i++)
    {
        A[i] = malloc(w.ws_col *  w.ws_row * sizeof(char));}
    printf("pass first");
	tab_clear(A);

	system("clear"); //Netoyage de la console

	if(strcmp(argv[3],"0")==0){ //Selection de la direction aléatoirement
		tab_editeur(A,Pos.x,Pos.y,"pbm/avion bas.pbm",1);
	}
	if(strcmp(argv[3],"1")==0){
		tab_editeur(A,Pos.x,Pos.y,"pbm/avion haut.pbm",1);
	}
	if(strcmp(argv[3],"2")==0){
		tab_editeur(A,Pos.x,Pos.y,"pbm/avion gauche.pbm",1);
	}
	if(strcmp(argv[3],"3")==0){
		tab_editeur(A,Pos.x,Pos.y,"pbm/avion droite.pbm",1);
	}
    //Edition du tableau avec l'ajout des nuages
	tab_editeur(A,0,0,"pbm/nuage.pbm",1);
	tab_editeur(A,27,0,"pbm/nuage.pbm",1);
	tab_editeur(A,54,0,"pbm/nuage.pbm",1);
	tab_editeur(A,81,0,"pbm/nuage.pbm",1);
	tab_editeur(A,108,0,"pbm/nuage.pbm",1);
    //Affichage du tableau
    tab_afficheur(A);

	while (1){ // Boucle infini
    printf("x:%d y:%d", Pos.x, Pos.y); //Affichage des coordonnées
	entry = getch(); // Quand l'utilisateur utilise le clavier
        switch (entry)
        {

        	case 's': // Si il a appuyer sur 's'
        		Pos.y++; // On augmente la position y
        		last = 0; // On dis que le dernier sens etait le bas

                // Quand on arrive hors de l'espace aérien, on est téléporter a l'opposé
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1; 

				tab_clear(A);

                //Ajout de la flotte mirroir au tableau
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion bas.pbm",1);        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion bas.pbm",1);
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion bas.pbm",1);
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion bas.pbm",1);        		
        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion bas.pbm",1);

                //Ajout de l'avion principal au tableau
        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);

                //Affichage du tableau
        		tab_afficheur(A);
        		break;

        	case 'q': //Meme chose avec un deplacement sur la gauche
        		Pos.x--;
        		last = 1;
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1; 
				tab_clear(A);  
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion gauche.pbm",1);        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion gauche.pbm",1);
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion gauche.pbm",1);
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion gauche.pbm",1);        		
        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion gauche.pbm",1);
        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);
        		tab_afficheur(A);
        		break;
        	case 'z': //Meme chose avec un deplacement sur le haut
        		Pos.y--;
        		last = 2;
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1;
				tab_clear(A); 
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion haut.pbm",1);        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion haut.pbm",1);
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion haut.pbm",1);
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion haut.pbm",1);
        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion haut.pbm",1);
        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);
        		tab_afficheur(A);
        		break;
        	case 'd': //Meme chose avec un deplacement sur la droite
        		Pos.x++;
        		last = 3;
        		if(Pos.x == w.ws_col) Pos.x = 1;
        		if(Pos.y == w.ws_row) Pos.y = 1;
        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
        		if(Pos.y == 0) Pos.y = w.ws_row-1; 
				tab_clear(A);  
        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion droite.pbm",1);        		
        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion droite.pbm",1);
        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion droite.pbm",1);
        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion droite.pbm",1);
        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion droite.pbm",1);
        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);
        		tab_afficheur(A);
        		break;
        	case '\n': // Si l'on appuye sur ENTER, refait le dernier mouvement
        		if(last == 0){
    			    Pos.y++;
	        		last = 0;
	        		if(Pos.x == w.ws_col) Pos.x = 1;
	        		if(Pos.y == w.ws_row) Pos.y = 1;
	        		if(Pos.x == 0) Pos.x = w.ws_col-1;  
	        		if(Pos.y == 0) Pos.y = w.ws_row-1;  
					tab_clear(A); 
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion bas.pbm",1);        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion bas.pbm",1);
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion bas.pbm",1);
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion bas.pbm",1);        		
	        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion bas.pbm",1);
	        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);
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
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion gauche.pbm",1);        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion gauche.pbm",1);
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion gauche.pbm",1);
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion gauche.pbm",1);        		
	        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion gauche.pbm",1);
	        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);
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
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion haut.pbm",1);        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion haut.pbm",1);
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion haut.pbm",1);
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion haut.pbm",1);
	        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion haut.pbm",1);
	        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);
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
	        		tab_editeur(A,Pos.x,Pos.y+w.ws_row,"pbm/avion droite.pbm",1);        		
	        		tab_editeur(A,Pos.x,Pos.y-w.ws_row,"pbm/avion droite.pbm",1);
	        		tab_editeur(A,Pos.x+w.ws_col,Pos.y,"pbm/avion droite.pbm",1);
	        		tab_editeur(A,Pos.x-w.ws_col,Pos.y,"pbm/avion droite.pbm",1);
	        		tab_editeur(A,Pos.x,Pos.y,"pbm/avion droite.pbm",1);
	        		tab_editeur(A,0,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,27,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,54,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,81,0,"pbm/nuage.pbm",1);
	        		tab_editeur(A,108,0,"pbm/nuage.pbm",1);
	        		tab_afficheur(A);
        		}
        		break;

        	case 'x':
        		exit(0); // Si l'on appuye sur x, le programme s'arrete


}

}

	return 0;
}



