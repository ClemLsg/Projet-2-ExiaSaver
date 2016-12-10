#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
 
int main (int agrc, char * argv[])
{
    if (agrc>1)
    {
        if (strcmp(argv[1],"stats"))
        {
            printf("Erreur de paramètres\n");
        }
        else
        {
            printf("Lancement des stats...\n");
        }
    }  
    else
    {
        printf("Lancement du screensaver...\n");
        int f;
        int random;
        int i;
        system("clear");        /* Nettoye la console*/
        srand(time(NULL));      /*Graine permettant d'éviter la repetition*/
        random = rand();
        random = (random%3);    
        if (random==0)
        {
            f=fork();
            if (f==0)
            {
                printf("Demarrage du statique");
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
                printf("Demarrage du dynamique");
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
                printf("Demarrage de l'interactif");
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