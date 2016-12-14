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
        int head;
    };


struct image{
    char nb_magique;
    int longueur;
    int largeur;
};

/* Afficheur */
 
void print_pbm(char *file, char *xpos, char *ypos)
{
    FILE* fichier = NULL;
    fichier = fopen(file, "r");
    int compteur = 0;
    int i = 0;
    int cara = 0;
    char longueur[256];
    char largeur[256];
    char nb_magique[256];
    char chaine[1000] = "";
    char line[1000] = "";
    char *token;
    char *p;
 
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int col = w.ws_col;
    int row = w.ws_row;
    int rowi;
 
    struct image Image;
 
    while (fgets(chaine, sizeof(chaine), fichier) != NULL){
        if (strchr(chaine, '#') == NULL){
            switch(compteur){
                case 0 :
                    strcpy(nb_magique, chaine);
                    Image.nb_magique = *nb_magique;
                    break;
                case 1:
                    token = strtok(chaine, " ");
                    strcpy(longueur, token);
                    int longu = atoi(longueur);
                    Image.longueur = longu;
                    if(strcmp(xpos, "center")==0){
                    col = (col-longu)/2;
                    } else {
                        int xposi = atoi(xpos);
                        col = xposi;
                    }
 
                    token = strtok(NULL, " ");
                    strcpy(largeur, token);
                    int larg = atoi(largeur);
                    Image.largeur = larg;
                    if(strcmp(ypos, "center")==0){
                    row = (row-larg)/2;
                    } else {
                      int yposi = atoi(ypos);
                      row = yposi;
                      rowi =  w.ws_row-(larg+yposi);  
                    }
                    for(i = 0; i < col; i++){
                        strcat(line, " ");
                    }
                    for(i = 0; i < longu*2*(larg+1); i++){
                        cara = fgetc(fichier);
                        switch(cara){
                            case '0':
                              strcat(line, " ");
                              break;
                            case '1':
                              strcat(line, "X");
                              break;
                            case '\n':
                              strcat(line, "\n");
                                  for(i = 0; i < col; i++){
                                        strcat(line, " ");
                                    }
                              break;
                            default:
                                break;
                        }
 
                    }
                        system("clear");
                        if(strcmp(ypos, "center")==0){
                            for(i = 0; i < row; i++){
                                printf("\n");
                                strcat(line, "\n");
                            }
                        } else {
                           for(i = 0; i < row; i++){
                                printf("\n");
                            }
                            for(i = 0; i < rowi; i++){
                                strcat(line, "\n");
                            }
                        }
                    printf("%s", line);
 
                break;
                }
            compteur++;
        }
    }
}

/* Voici la fonction getch() qui va détecté l'utilisation du clavier */

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


/* Boucle principal */

int main(int argc, char *argv[])
{ 
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	char avx[256];
	char avy[256];	
	struct pos Pos;
	char c;
	while(1)
    {
        printf("x:%d y:%d and max x%d max y:%d", Pos.x, Pos.y, w.ws_col, w.ws_row);
        c = getch();
        switch (c)
        {
            case 'z':
                Pos.y--;
                if(Pos.x == w.ws_col){Pos.x = 0;}
                if(Pos.y == w.ws_row){Pos.y = 0;}
                if(Pos.x == -1){Pos.x = w.ws_col;}
                if(Pos.y == -1){Pos.y = w.ws_row;}
                sprintf(avx, "%d", Pos.x);
                sprintf(avy, "%d", Pos.y);
                print_pbm("avion haut.pbm", avx, avy);
                break;
            case 'd':
                Pos.x++;
                if(Pos.x == w.ws_col){Pos.x = 0;}
                if(Pos.y == w.ws_row){Pos.y = 0;}
                if(Pos.x == -1){Pos.x = w.ws_col;}
                if(Pos.y == -1){Pos.y = w.ws_row;}
                sprintf(avx, "%d", Pos.x);
                sprintf(avy, "%d", Pos.y);
                print_pbm("avion droite.pbm", avx, avy);
                break;
            case 's':
                Pos.y++;
                if(Pos.x == w.ws_col){Pos.x = 0;}
                if(Pos.y == w.ws_row){Pos.y = 0;}
                if(Pos.x == -1){Pos.x = w.ws_col;}
                if(Pos.y == -1){Pos.y = w.ws_row;}
                sprintf(avx, "%d", Pos.x);
                sprintf(avy, "%d", Pos.y);
                print_pbm("avion bas.pbm", avx, avy);
                break;
            case 'q':
                Pos.x--;
                if(Pos.x == -1){Pos.x = w.ws_col;}
                if(Pos.y == -1){Pos.y = w.ws_row;}
                if(Pos.x == w.ws_col){Pos.x = 0;}
                if(Pos.y == w.ws_row){Pos.y = 0;}
                sprintf(avx, "%d", Pos.x);
                sprintf(avy, "%d", Pos.y);
                print_pbm("avion gauche.pbm", avx, avy);
                break;
            case 'x':
                exit(0);
                break;
        }
}
}




