#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void print_ligne(char *txt){
	int i;

	for (i = 0; i < strlen(txt);i++){
		if (txt[i]=='0') printf(" "); 
		if (txt[i]=='1') printf("%c", 'x');
	}
	printf("\n");
}

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

int main(int argc, char *argv[]){
	FILE *fichier;
	char ligne[100];
	system("clear");
	fichier = fopen("screen1.pbm", "r");
	if (fichier == NULL) return 1;
	fgets(ligne,100,fichier);
	fgets(ligne,100,fichier);
	fgets(ligne,100,fichier);
	while(fgets(ligne,100,fichier)!= NULL) print_ligne(ligne);
	fclose(fichier);
	getch();
	return 0;
}