#include <stdlib.h>
#include <stdio.h>
#include <time.h>                                                       //Bibliothèque permettant de gerer le temps
#include <string.h>                                                     //Bibliothèque permettant de gerer les chaines de charactère
#include <unistd.h>                                                     //Bibliothèque permettant de gerer les symbols standard
#include <sys/types.h>                                                  //Bibliothèque permettant de gerer les définitions de temps pour UNIX
#include <termios.h>                                                    //Bibliothèque permettant de gerer les définitions des valeurs du termios

#define TRUE 1                                                          //Définit TRUE comme 1
#define FALSE 0                                                         //Définit FALSE comme 0

FILE* fichierstats = NULL;                                              /*Déclare la variable fichierstats comme variable globale fichier*/
char chainehisto[40960][40];                                            //Donne a chainehisto des valeurs globales

int transformation(int n)                                               //Fonction permettant la transformation du code uni en valeur
{                                                                       //La récupération du char en int donne des code uni
    switch(n){                                                          //Ici on les transforme en entier, ce qui...
        case 49:                                                        //Leur redonne la valeur d'origine
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

void tri_bulle(int n)                                                   //Fonction de tri à bulle ( Se ref. wikipedia tri a bulle)
{
    int j = 0;                                                          //Déclaration d'un compteur entier
    char tmp[2][40];                                                    //Déclaration d'un tableau de char en 2D de 2x40
    int pastrie = 1;                                                    //Déclaration d'un int pastrie, si 1 alors le tri n'est pas finit                                                              //
    int a;                                                              //Déclaration d'un entier prenant le code UNI d'un char
    int b;                                                              //Déclaration d'un entier prenant le code UNI d'un char
    char temp;                                                          //Déclaration d'une variable char tempon, contient la chaine en cours de transfert dans le tri à bulle
    int typea=0;                                                        //Déclaration de variable contenant le nombre de fois qu'est utilisé l'ES pour les statiques
    int typeb=0;                                                        //Déclaration de variable contenant le nombre de fois qu'est utilisé l'ES pour les dynamiques
    int typec=0;
    float typeap=0;                                                        //Déclaration de variable contenant le pourcentage de fois qu'est utilisé l'ES pour les statiques
    float typebp=0;                                                        //Déclaration de variable contenant le pourcentage de fois qu'est utilisé l'ES pour les dynamiques
    float typecp=0;                                                        //Déclaration de variable contenant le pourcentage de fois qu'est utilisé l'ES pour les intéractifs

    while(pastrie)                                                      //Boucle qui tourne tant que le tri n'est pas finis
    {
        pastrie=FALSE;                                                  //Définit de base le tri comme non finit (juste pour être sur)
        for(j=0;j<n;j++)                                                //Boucle qui tourne jusqua que toute les lignes du char chainehisto soient traitées
        {
            a=(int) chainehisto[j][20];                                 //Prend la valeur UNI du char étant à la ligne J au rand 20, emplacement exacte du numéro de mode dans le fichier histo
            b=(int) chainehisto[j+1][20];                               //Prend la valeur UNI du char étant à la ligne J+1 (la ligne suivante la varible a ) au rand 20
            a=transformation(a);                                        //Ici on appelle la fonction transformation pour transformer le code UNI en int
            b=transformation(b);

            if(a>b){                                                    //Si, grace a la transformation on comprend que a est supérieur à b, alos...
                strcpy(tmp[0],chainehisto[j+1]);                        //On effectue le tri à bulle c-a-d on prend le trop grand, on le met dans une variable temporaire
                strcpy(chainehisto[j+1],chainehisto[j]);                //Ensuite on met le plus petit à la place du plus grand et on met le plus grand a la place du plus petit.
                strcpy(chainehisto[j],tmp[0]);                          //Grace a ça on ecrase aucune donnée.
                pastrie=TRUE;                                           //On signale le tri comme terminé mais ne sera effectife que quand le boucle for sera finie
            }
            
        }
    }
    n=n+1;                                                              //On incrémente n, car le 0 étant compté precedement pour le déplacement dans les tableaux, pour avoir un affichage plus propre pour l'utilisateur
    for (j=0; j<n; j++){                                                //Ici on affiche la chainehisto trié ligne par ligne
        printf("%s",chainehisto[j]);                                    //Tant que toutes les lignes ne sont pas passées
        a=(int) chainehisto[j][20];                                     //On récupère de nouveau la valeur UNI
        a=transformation(a);                                            //On la traite
        switch(a){                                                      //Et en fonction du résultat on incrément des variables pour
                case 1:                                                 //donner à l'utilisateur les statistiques et le
                    typea++;                                            // nombre de fois que chaque ES à été lancé.
                    break;
                case 2:
                    typeb++;
                    break;
                case 3:
                    typec++;
                    break;
            }
    }
    typec--;
    n--;
    typeap=(((float)typea/(float)n)*100);
    typebp=(((float)typeb/(float)n)*100);
    typecp=(((float)typec/(float)n)*100);
    printf("\n\nVous avez lancé %d fois (%.2f %%) l'ExiaSaver statique, %d fois le dynamique (%.2f %%) et %d fois l'interactif (%.2f %%) ! \n\n",typea,typeap,typeb,typebp,typec,typecp);
}
    

void statistiques()                                                 //Fonction permettant l'affichage des statistiques
{
    char chaine[255] = "r";                                  //Variable chaine définie avec une taille par DEFINE qui contiendra la chaine en cours d'utilisation
    int modetri;                                                    //Varibale qui contient le mode de triage choisit par l'utilisateur
    int i=0;                                                          //Variable qui sert de compteur pour le nombre de ligne dans le fichier historique
    int j;                                                          //Compteur
    int n;                                                          //Compteur
    fichierstats = fopen("/home/enzo/Documents/ExiaSaver/historique", "r" );                       //Ouvre le fichier d'historique avec les droits de lecture
    if (fichierstats==NULL){ 
                printf("Erreur d'ouverture fichier");               //Affiche une erreur si le fichier n'est pas ouvrable
            }                                                        
    while (strcmp(chaine," ")!=0)                                   //Tant que la variable chaine ne contient pas un blanc....
    {
        fgets(chaine, 255, fichierstats);                    //On récupère ligne par ligne le fichier historique et....
        if (strcmp(chaine," ")!=0)                                  //Si ce qu'on recupère n'est toujours pas un blanc
        {
            strcat(chainehisto[i], chaine);                         //On colle ce qu'on vient de récuperer dans une variable char tampon qui contiendra alors l'ensemble du fichier historique sous forme de varibale, le tout dans le bon format (saut de lignes inclus)
        }
        i++;                                                        //On incrément i pour ajouter chaine dans une autre ligne du tableau chainehisto ( pour garder un format lisible)
    }
    fclose(fichierstats);                                           //On ferme le fichier historique
    i=i-1;                                                          //On decrémente i une fois car la boucle while nous compte une ligne de trop
    while (modetri!=1 && modetri!=2 && modetri!=3)                  //Ici tant que l'utilisateur rentre pas un mode de tri valide on continue de demander
    {
        printf("Pour trier par date tapez 1, par type tapez 2 et par dates inversées tapez 3 :\n");
        scanf("%d",&modetri);                                       //On place dans la varible modetri le choix de l'utilisateur
    }
    system("clear");
    printf("Voici votre historique ");
    switch(modetri){                                                //Ici en fonction du choix de l'utilisateur, on lance nos différents tri
        case 1:                                                     //Si modetri=1 ....
            printf("trié par date:\n\n");
            for (j=0;j<i;j++){                                      //Ici on affiche ligne par ligne avec le compteur j qu'on incrémente...
                printf("Utilisation N°%d:  ",j+1);                  //les différentes lignes du tableau chainehisto
                printf("%s\n",chainehisto[j]);
            }
            printf("\n\nVous avez lancé %d fois ExiaSaver !\n",i);      //On indique à l'utilisateur combien de fois il a utilisé l'ES
            break;                                                  //Met fin au cas 1
        case 2:                                                     //Si modetri=2 ....
            printf("trié par type:\n\n");
            tri_bulle(i);                                           //On appelle la fonction tri à bulle avec l'argument i, qui est le nombre de lignes
            
            break;
        case 3:                                                     //Si modetri=3 ....
            i--;
            printf("trié par dates inversées:\n\n");
             for (j=i;j>=0;j--){                                    //On inverse la boucle du mode 1
                printf("Utilisation N°%d:  ",j+1);                  //ce qui a pour effet d'afficher dans l'autre sens
                printf("%s\n",chainehisto[j]);                      //les entrées de chainehisto
            }
            printf("\n\nVous avez lancé %d fois ExiaSaver !\n",i);      //On indique à l'utilisateur combien de fois il a lancé l'ES
            break;
    }

}

void entreehistorique(int level, char *info)                            /*Fonction pour ecrire dans le fichier historique en fonction du type de SS et les infos émises*/
{

    int t;
    char insertion[5];
    level=level+1;
    sprintf(insertion,"%d",level);

    fichierstats = fopen("/home/enzo/Documents/ExiaSaver/historique", "r+" );                          /*Ouvre le fichier historique avec les droits ecriture et lecture*/

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

int main (int agrc, char * argv[])
{
    char data[16]=" ";                                              //Variable contenant le pramètre de lancement du ScreenSaver
    char *fichier;                                              //Variable contenant le nom du fichier à executer pour le ScreenSaver
    int f;                                                      //Variable pour le Fork. Contient 0 si fils et le PID du processus si père
    int random;
    int randomizing;                                                 //Variable contenant une valeur aléatoire en 1 et 3 pour le lancement du SS

    char Exiasaver_home[128];
    char Exiasaver1_Pbm[128];
    char Exiasaver2_Pbm[128];
    int Exiasaver2_taille;
    int Exiasaver2_sleep;
    char Exiasaver2_taille_c[128];
    char Exiasaver2_sleep_c[128];
    char Exiasaver3_Pbm[128];
    char Exiasaver32_Pbm[128];

    int positionx;
    int positiony;

    int sens;

    char positionxc[5];
    char positionyc[5];

    char sensc[10];

    char arghaut[128];
    char argbas[128];
    char arggauche[128];
    char argdroit[128];
    char argnuage[128];


    system("clear");                                            //Commande vidant la console, affiche plus propre
    if (agrc>1)                                                 //Condition valide si agrc (nombre de paramètres lancés avec ES) est supérieur à 1
    {
        if (strcmp(argv[1],"-stats"))                           //Condition valide si argv (valeur des pramètres lancés avec ES) vaut -stats
        {
            printf("Erreur de paramètres\n");                   
        }
        else
        {
            printf("Lancement des stats...\n");
            statistiques();                                     //Lancement de la fonction statistiques
        }
    }  
    else                                                        //Arrive ici si le ES n'est pas lancé avec des arguments
    {
        if (getenv("EXIASAVER_HOME")!=NULL) strcpy(Exiasaver_home,getenv("EXIASAVER_HOME"));
        else printf("Variable non trouvé\n");

        if (getenv("EXIASAVER1_PBM")!=NULL) strcpy(Exiasaver1_Pbm,getenv("EXIASAVER1_PBM"));
        else printf("Variable non trouvé\n");

        if (getenv("EXIASAVER2_PBM")!=NULL) strcpy(Exiasaver2_Pbm,getenv("EXIASAVER2_PBM"));
        else printf("Variable non trouvé\n");

        if (getenv("EXIASAVER2_TAILLE")!=NULL) Exiasaver2_taille=atoi(getenv("EXIASAVER2_TAILLE"));
        else printf("Variable non trouvé\n");

        if (getenv("EXIASAVER2_SLEEP")!=NULL) Exiasaver2_sleep=atoi(getenv("EXIASAVER2_SLEEP"));
        else printf("Variable non trouvé\n");

        if (getenv("EXIASAVER3_PBM")!=NULL) strcpy(Exiasaver3_Pbm,getenv("EXIASAVER3_PBM"));
        else printf("Variable non trouvé\n");

        printf("Lancement du screensaver...\n");
        srand(time(NULL));                                      //Graine permettant d'éviter la repetition lors du randomize
        random = rand();                                        //Place dans random une valeur aléatoire sans limites à partir de la seed
        random = (random%3);                                    //%3 réduit le résultat à une plage en 0 et 2 soit 3 valeurs
        if (random==0)                                          //Si le nombre genere est 0 alors...
        {
            f=fork();                                           //Lance un processus fils 
            if (f==0)                                           //Uniquement le processus fils avec un PID de 0 peut entrer dans la boucle
            {
                randomizing = rand();                                
                randomizing = (randomizing%5);
                switch(randomizing){
                    case 0:
                        strcpy(data,"nuage.pbm");               //On met la chaîne de caractère grenouille.pbm dans la variable data
                        break;
                    case 1:
                        strcpy(data,"grenouille.pbm");
                        break;
                    case 2:
                        strcpy(data,"feep.pbm");
                        break;
                    case 3:
                        strcpy(data,"avionbas.pbm");
                        break;
                    case 4:
                        strcpy(data,"aviondroite.pbm");
                        break;
                }
                strcat(Exiasaver1_Pbm,data);                     
                entreehistorique(random,data);                  //On entre dans la fonction entreehistorique avec les arguments random et data
                fichier="screensaver1";                         //On donne à fichier la chaine de caratcère afficheur qui correspond au fichier à ouvrir
                strcat(Exiasaver_home,fichier);
                printf("Demarrage du statique\n");
                execl(Exiasaver_home,fichier,"666","666",Exiasaver1_Pbm,NULL);   //On execute le fichier afficheur avec l'argument 666 qui position au centre de la console l'image
                exit(1);                                        //On tue le processus fils
            }
            else                                                
            {
                wait();                                         //Sinon le père attend l'arrivée ou la mort du fils
            }
        }
        if (random==1)
        {
            f=fork();
            if (f==0)
            {
                if(Exiasaver2_taille==1) strcpy(data,"5x3");                         //On rentre dans data des valeurs
                else strcpy(data,"10x6");

                fichier="screensaver2";                         //On donne à fichier la chaine de caratcère afficheur qui correspond au fichier à ouvrir
                strcat(Exiasaver_home,fichier);

                printf("%s\n",Exiasaver_home);
                printf("%s\n",fichier);
                printf("%d\n",Exiasaver2_taille);
                printf("%d\n",Exiasaver2_sleep);
                printf("%s\n",Exiasaver2_Pbm);

                sprintf(Exiasaver2_taille_c,"%d",Exiasaver2_taille);
                sprintf(Exiasaver2_sleep_c,"%d",Exiasaver2_sleep);

                strcat(data,"/");
                strcat(data,Exiasaver2_sleep_c);
                entreehistorique(random,data);                  
                
                printf("Demarrage du dynamique\n");
                execl(Exiasaver_home,fichier,Exiasaver2_taille_c,Exiasaver2_sleep_c,Exiasaver2_Pbm,NULL);
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
                fichier="screensaver3";
                strcat(Exiasaver_home,fichier);

                srand(time(NULL));
                positionx = rand();                                
                positionx = (positionx%80);

                srand(time(NULL));
                positiony = rand();                                
                positiony = (positiony%24);

                srand(time(NULL));
                sens = rand();                                
                sens = (sens%3);

                sprintf(positionxc,"%d",positionx);
                sprintf(positionyc,"%d",positiony);
                sprintf(sensc,"%d",sens);

                strcat(data,positionxc);
                strcat(data,":");
                strcat(data,positionyc);
                strcat(data,"/");
                strcat(data,sensc);

                entreehistorique(random,data);

                strcpy(Exiasaver32_Pbm,Exiasaver3_Pbm);
                strcpy(argdroit,strcat(Exiasaver32_Pbm,"aviondroite.pbm"));

                strcpy(Exiasaver32_Pbm,Exiasaver3_Pbm);
                strcpy(arggauche,strcat(Exiasaver32_Pbm,"aviongauche.pbm"));

                strcpy(Exiasaver32_Pbm,Exiasaver3_Pbm);
                strcpy(arghaut,strcat(Exiasaver32_Pbm,"avionhaut.pbm"));

                strcpy(Exiasaver32_Pbm,Exiasaver3_Pbm);
                strcpy(argbas,strcat(Exiasaver32_Pbm,"avionbas.pbm"));

                strcpy(Exiasaver32_Pbm,Exiasaver3_Pbm);
                strcpy(argnuage,strcat(Exiasaver32_Pbm,"nuage.pbm"));

                printf("Demarrage de l'interactif\n");

                printf("%s\n",Exiasaver_home);
                printf("%s\n",fichier);
                printf("%s\n",arghaut);
                printf("%s\n",argbas);
                printf("%s\n",arggauche);
                printf("%s\n",argdroit);
                printf("%s\n",argnuage);

                execl(Exiasaver_home,fichier,positionxc,positionyc,sensc,arghaut,argbas,arggauche,argdroit,argnuage,NULL);
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