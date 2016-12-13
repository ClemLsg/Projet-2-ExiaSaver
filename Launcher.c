#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>

#define TAILLE_MAX 40
#define TRUE 1
#define FALSE 0

FILE* fichierstats = NULL;                                              /*Déclare la variable fichierstats comme variable fichier*/
char chainehisto[40960][40];

int transformation(int n)
{
    switch(n){
        case 49:
            n=1;
            break;
        case 50:
            n=2;
            break;
        case 51:
            n=3;
            break;
    }
    return n;
}

void tri_bulle(int n)
{
    int j = 0;
    char tmp[2][40];
    int pastrie = 1;
    int p;
    int a;
    int b;
    char temp;
    while(pastrie)
    {
        pastrie=FALSE;
        for(j=0;j<n;j++)
        {
            a=(int) chainehisto[j][20];
            b=(int) chainehisto[j+1][20];
            a=transformation(a);
            b=transformation(b);
            if(a>b){
                strcpy(tmp[0],chainehisto[j+1]);
                strcpy(chainehisto[j+1],chainehisto[j]);
                strcpy(chainehisto[j],tmp[0]);
                pastrie=TRUE;
            }
            
        }
    }
    n=n+1;
    for (j=0; j<n; j++){
        printf("%s",chainehisto[j]);
    }
}
    

void statistiques()
{
    char chaine[TAILLE_MAX] = "r";
    int modetri;
    int i;
    int j;
    int *t;
    int n;
    fichierstats = fopen("historique", "r" );
    if (fichierstats==NULL){ 
                printf("Erreur d'ouverture fichier"); 
            }
    i=0;
    while (strcmp(chaine," ")!=0)
    {
        fgets(chaine, TAILLE_MAX, fichierstats);
        if (strcmp(chaine," ")!=0)
        {
            strcat(chainehisto[i], chaine);
        }
        i++;
    }
    fclose(fichierstats);
    i=i-1;
    while (modetri!=1 && modetri!=2 && modetri!=3)
    {
        printf("Pour trier par date tapez 1, par type tapez 2 et par paramètres tapez 3 :\n");
        scanf("%d",&modetri);
    }
    printf("Voici votre historique ");
    switch(modetri){
        case 1:
            printf("trié par date:\n\n");
            for (j=0;j<i;j++)
                printf("%s\n",chainehisto[j]);
            break;
        case 2:
            printf("trié par type:\n\n");
            tri_bulle(i);
            
            break;
        case 3:
            printf("trié par paramètres:\n\n");
            break;
    }

}

void entreehistorique(int level, char *info)                            /*Fonction pour ecrire dans le fichier historique en fonction du type de SS et les infos émises*/
{

    int t;
    char insertion[5];
    level=level+1;
    sprintf(insertion,"%d",level);

    fichierstats = fopen("historique", "r+" );                          /*Ouvre le fichier historique avec les droits ecriture et lecture*/

    fseek(fichierstats, -1, SEEK_END);                                   /*Place le curseur virtuel au bout du fichier*/ 
    time_t now = time (NULL);                                           /*Récupère le temps dans la variable now*/
    struct tm tm_now = *localtime (&now);                               /*Converti now en heure locale*/
    char s_now[sizeof "JJ/MM/AAAA HH:MM:SS"];                           /*Creer une chaine basique ...*/
    strftime (s_now, sizeof s_now, "%d/%m/%Y %H:%M:%S", &tm_now);       /*...pour l'affichage de l'heure*/

    if (fichierstats!=NULL)
    {
        fprintf(fichierstats,"%s",s_now);                                /*Ecrit le format du cahier des charges dans le fichier historique*/
        fputs(";", fichierstats);                                                               /*///////////*/
        fputs(insertion, fichierstats);                                                         /*///////////*/
        fputs(";", fichierstats);                                                               /*///////////*/
        fputs(info, fichierstats);                                                              /*///////////*/ 
        fputs("\n ", fichierstats);                                      /*Ecrit le format du cahier des charges dans le fichier historique*/
        fclose(fichierstats);                                            /*Ferme le fichier*/
    }
}

char getch()                                                            /*Fonction getch de windows, importée sur Linux. SNIPPET TROUVE SUR INTERNET*/
{
    char buf=0;                                                         /*Permet de capter l'utilisation du clavier par l'utilisateur*/
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

int main (int agrc, char * argv[])
{
    printf("Test\n");
    char data[16];
    char datata[16];
    char *fichier;
    int f;
    int random;
    int i;

    system("clear");
    if (agrc>1)
    {
        if (strcmp(argv[1],"-stats"))
        {
            printf("Erreur de paramètres\n");
        }
        else
        {
            printf("Lancement des stats...\n");
            /*while (getch()!='q')
            {

            }*/
            statistiques();
        }
    }  
    else
    {
        printf("Lancement du screensaver...\n");
        srand(time(NULL));                                      /*Graine permettant d'éviter la repetition*/
        random = rand();
        random = (random%3);
        if (random==0)
        {
            f=fork();
            if (f==0)
            {
                strcpy(data,"grenouille.pbm");
                entreehistorique(random,data);
                fichier="afficheur";
                printf("Demarrage du statique\n");
                execl(fichier,fichier,NULL);
                exit(1);
            }
            else
            {
                wait();
            }
        }
        if (random==1)
        {
            f=fork();
            if (f==0)
            {
                strcpy(data,"10x10");
                entreehistorique(random,data);
                printf("Demarrage du dynamique\n");
                exit(1);
            }
            else
            {
                wait();
            }
        }
        if (random==2)
        {
            f=fork();
            if (f==0)
            {
                strcpy(data,"20x20");
                entreehistorique(random,data);
                fichier="avion";
                printf("Demarrage de l'interactif\n");
                execl(fichier,fichier,NULL);
                exit(1);
            }
            else
            {
                wait(NULL);
            }
        }
    }
   
    return 0;
}