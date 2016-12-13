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
    int i,I,J,last;
    char entry;
	struct pos Pos;
	int posx = strtol(argv[1], NULL, 10);
	int posy = strtol(argv[2], NULL, 10);
	Pos.x = posx;
	Pos.y = posy;

/*CREATION DU TABLEAU A DEUX DIMENSIONS */

	char **A=malloc(sizeof(char*)*  w.ws_col*w.ws_row);
	for(i=0; i<w.ws_row; i++)
    {
        A[i] = malloc(w.ws_col *  w.ws_row * sizeof(char));}
    printf("pass first");
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
    printf("pass"); 
	while (1){
    printf("x:%d y:%d", Pos.x, Pos.y);
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



