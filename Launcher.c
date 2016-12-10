#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
 
int main (int agrc, char * argv[])
{
    char *fichier;

    if (agrc>1)
    {
        if (strcmp(argv[1],"stats"))
        {
            system("clear");
            printf("Erreur de paramètres\n");
        }
        else
        {
            system("clear");
            printf("Lancement des stats...\n");
        }
    }  
    else
    {
        system("clear");
        printf("Lancement du screensaver...\n");
        int f;
        int random;
        int i;
        srand(time(NULL));      /*Graine permettant d'éviter la repetition*/
        random = rand();
        random = (random%3);    
        if (random==0)
        {
            f=fork();
            if (f==0)
            {
                printf("Demarrage du statique\n");
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
                fichier="avion";
                printf("Demarrage de l'interactif\n");
                execl(fichier,fichier,NULL);
                exit(1);
            }
            else
            {
                wait();
            }
        }
    }
   
    return 0;
}