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

char getch() // Fonction de gestion des entrées
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

struct pos
    {
        int x;
        int y;
};

int main(int argc, char * argv[]){
	struct winsize w; // Création d'une structure contenant la taille de la console
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Ecriture de la taille de la console dans la structure "w"
	int i,I,J,last; // Initialisation des variables de type INT
	char entry; // Initialisation des variables de type CHAR
	struct pos Pos; // Initialisation de la structur pos
	char **A=malloc(sizeof(char*)*  w.ws_col*w.ws_row); // Initialisation du tableau
	for(i=0; i<w.ws_row; i++)
    {
        A[i] = malloc(w.ws_col *  w.ws_row * sizeof(char));}

	tab_clear(A);
	int posx = strtol(argv[1], NULL, 10); //Récuperation du premier argument
	int posy = strtol(argv[2], NULL, 10); //Récuperation du deuxieme argument
	Pos.x = posx; // Injection des arguments dans la structure pos
	Pos.y = posy;
	tab_editeur(A,Pos.x,Pos.y,argv[3]); //Edition du tableau
    tab_afficheur(A); //Affichage du tableau
    
    entry = getch(); // En attente d'une action clavier de la part de l'utilisateur
        switch (entry) //Si l'utilisateur utilise son clavier
        {
        	default: exit(0);} // Kill le programme
	}
