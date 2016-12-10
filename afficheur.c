#include <stdio.h>
#include <string.h>

void print_ligne(char *txt){
	int i;

	for (i = 0; i < strlen(txt);i++){
		if (txt[i]=='0') printf(" "); 
		if (txt[i]=='1') printf("%c", 'x');
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	FILE *fichier;
	char ligne[100];
	int v;
	for (v=0; v<255; v++){
		printf("%d : %c\n", v,v);
	}
	return 0;
	fichier = fopen("avion haut.pbm", "r");
	if (fichier == NULL) return 1;
	fgets(ligne,100,fichier);
	fgets(ligne,100,fichier);
	fgets(ligne,100,fichier);
	while(fgets(ligne,100,fichier)!= NULL) print_ligne(ligne);
	fclose(fichier);
	return 0;
}