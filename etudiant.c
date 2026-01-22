#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "etudiant.h"
//constantes a utiliser
#define MENU_WIDTH 80
#define INDENT "\t\t"
//fonction pour calculer l'age
int calculAge(const char*matricule,const char*nomFichier){
    FILE *f = fopen(nomFichier,"r");
    int age = -1;
    Etudiant e;
    Date today = dateActuelle();
    Date naissance;
    if (f!=NULL)
    {
        printf("Erreur lors de l'ouverture du fichier! ");
        return -1;
    }
    while (fscanf(f, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c",e.matricule,e.nom,e.prenom,&e.dateNaissance.jour,&e.dateNaissance.mois,&e.dateNaissance.annee,e.departement,e.filiere,e.region,&e.sexe)==10)
    {
        if (strcasecmp(e.matricule, matricule))
        {
            age = today.annee - e.dateNaissance.annee;
            if (today.mois < e.dateNaissance.mois || (today.mois == e.dateNaissance.mois && today.jour < e.dateNaissance.jour))
            {
                age--;
            }
            break;
        }
        
    }
    fclose(f);
    if (age == -1)
    {
        printf("Etudiant avec le matricule %s non trouve", matricule);
    }
    return age;
    

}
//fonction pour retourner le nombre d'etudiant
int nombreEtudiant(const char*nomFichier)
{
     FILE* f = fopen(nomFichier, "r");
    int count = 0;
   // Etudiant e;
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }
    char ligne[100];
    while (fgets(ligne,sizeof(ligne),f))
            {
               count++;
            }

    fclose(f);
    return count;   
}

//modifier etudiant
int modifierEtudiant(const char* matricule, const char* nomFichier) {
    int tailletab=nombreEtudiant(nomFichier);
    Etudiant etudiants[tailletab];
    int n = 0; 
    int trouve = 0;

    FILE* f = fopen(nomFichier, "r");
    if(f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while(fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                 etudiants[n].matricule, etudiants[n].nom, etudiants[n].prenom,
                 &etudiants[n].dateNaissance.jour, &etudiants[n].dateNaissance.mois, &etudiants[n].dateNaissance.annee,
                 etudiants[n].departement, etudiants[n].filiere, etudiants[n].region, &etudiants[n].sexe) != EOF) {
        n++;
        if(n >= tailletab) break;
    }
    fclose(f);
    for(int i = 0; i < n; i++) {
        if(strcasecmp(etudiants[i].matricule, matricule) == 0) {
            trouve = 1;
        char rep;
        do
        {
            entete("Menu des modifications");
            menu_item("1","Modifier le nom");
            menu_item("2", "Modifier le prenom");
            menu_item("3","Modifier la date de naissance");
            menu_item("4","Modifier le departement");
            menu_item("5","Modifier la filiere");
            menu_item("6","Modifier la region");
            menu_item("7","Modifier le sexe");
            menu_item("0","Revenir au menu principal");
            menu_footer();
            printf("Que voulez-vous modifier exactement ? ");
            scanf(" %c", &rep);
            switch (rep) {
                case '1':
                    //printf("Modification des informations pour l'etudiant avec le matricule %s:\n", matricule);
                    printf("Nouveau Nom (actuel: %s): ", etudiants[i].nom);
                    scanf("%19s", etudiants[i].nom);
                    break;
                case '2':
                    printf("Nouveau Prenom (actuel: %s): ", etudiants[i].prenom);
                    scanf("%19s", etudiants[i].prenom);
                    break;
                case '3':
                    printf("Nouvelle Date de Naissance (actuelle: %02d/%02d/%04d):\n",
                           etudiants[i].dateNaissance.jour, etudiants[i].dateNaissance.mois, etudiants[i].dateNaissance.annee);
                    etudiants[i].dateNaissance = creerDate();
                    break;
                case '4':
                    printf("Nouveau Departement (actuel: %s): ", etudiants[i].departement);
                    scanf("%19s", etudiants[i].departement);
                    break;
                case '5':
                    printf("Nouvelle Filiere (actuelle: %s): ", etudiants[i].filiere);
                    scanf("%29s", etudiants[i].filiere);
                    break;
                case '6':
                    printf("Nouvelle Region (actuelle: %s): ", etudiants[i].region);
                    scanf("%19s", etudiants[i].region);
                case '7':
                    do {
                        printf("Nouveau Sexe (actuel: %c) (M/F): ", etudiants[i].sexe);
                        scanf(" %c", &etudiants[i].sexe);
                    } while(etudiants[i].sexe != 'M' && etudiants[i].sexe != 'F');
                    break;
                case '0':
                    break;
            }

        } while (rep != '0');
        
        }
    }

    if(!trouve) {
        printf("Etudiant avec le matricule %s non trouve.\n", matricule);
        return 0;
    }
    f = fopen(nomFichier, "w");
    if(f == NULL) {
        printf("Erreur d'ouverture du fichier pour ecriture\n");
        return 0;
    }

    for(int i = 0; i < n; i++) {
        fprintf(f, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
                etudiants[i].matricule, etudiants[i].nom, etudiants[i].prenom,
                etudiants[i].dateNaissance.jour, etudiants[i].dateNaissance.mois, etudiants[i].dateNaissance.annee,
                etudiants[i].departement, etudiants[i].filiere, etudiants[i].region, etudiants[i].sexe);
    }
    fclose(f);
    alert("Fin de modification");
    return 1;
}

//supprimer etudiant
int supprimerEtudiant(const char*matricule,const char*nomFichier){
/*    if (!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0;
    }*/
    FILE*f=fopen(nomFichier, "r");
    FILE* temp = fopen("temp.txt", "w");
    Etudiant e;
    int trouve = 0;

    if (f == NULL || temp == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while (fscanf(f, "%s\t%s\t%s\t%d/%d/%d\t%s\t%s\t%s\t%c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        if (strcasecmp(e.matricule, matricule) == 0) {
            trouve = 1;
        } else {
           fprintf(temp, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
                e.matricule, e.nom, e.prenom,
                e.dateNaissance.jour, e.dateNaissance.mois, e.dateNaissance.annee,
                e.departement, e.filiere, e.region, e.sexe);
        }

    }

    fclose(f);
    fclose(temp);
    remove(nomFichier);
    rename("temp.txt", nomFichier);

    if (trouve) {
        alert("SUPRESSION DE REUSSIE !");
        //printf("Etudiant avec le matricule %s supprime avec succes.\n", matricule);
    } else {
        alert("ETUDIANT INTROUVABLE !");
        //printf("Etudiant avec le matricule %s non trouve.\n", matricule);
    }
    return trouve;
}

//fichier de stockage des etudiants
//fonction pour creer une date conforme
Date creerDate()
{
    Date d;
    int valide;

    do
    {
        valide = 1;

        printf("veuillez entrer convenablement la date dans le cas contraire vous serrez contraint de recommencer \n");

       char buf[20];

        printf("\tJour : ");
        fgets(buf, sizeof(buf), stdin);
        d.jour = atoi(buf);
        printf("\tMois : ");
        fgets(buf, sizeof(buf), stdin);
        d.mois = atoi(buf);
        printf("\tAnnee : ");
        fgets(buf, sizeof(buf), stdin);
        d.annee = atoi(buf);
        if (d.annee <= 0)
        {
            printf("L'annee doit etre strictement positive\n");
            valide = 0;
        }

        /* Vérification du mois */
        if (d.mois < 1 || d.mois > 12)
        {
            printf("Le mois doit etre compris entre 1 et 12\n");
            valide = 0;
        }

        /* Vérification du jour selon le mois */
        if (valide)
        {
            if (d.jour < 1)
            {
                printf("Le jour doit etre superieur ou egal a 1\n");
                valide = 0;
            }
            else
            {
                switch (d.mois)
                {
                    case 1: case 3: case 5: case 7:
                    case 8: case 10: case 12:
                        if (d.jour > 31)
                        {
                            printf("Ce mois comporte 31 jours maximum\n");
                            valide = 0;
                        }
                        break;

                    case 4: case 6: case 9: case 11:
                        if (d.jour > 30)
                        {
                            printf("Ce mois comporte 30 jours maximum\n");
                            valide = 0;
                        }
                        break;

                    case 2://cas du mois de fevrier on a les annees bissectile et les annees ordinaires
                        // Annnee bissectile
                        if ((d.annee % 400 == 0) ||
                            (d.annee % 4 == 0 && d.annee % 100 != 0))
                        {
                            if (d.jour > 29)
                            {
                                printf("Fevrier ne comporte pas plus de 29 jours cette annee\n");
                                valide = 0;
                            }
                        }
                        else
                        {
                            if (d.jour > 28)
                            {
                                printf("Fevrier ne comporte pas plus de 28 jours cette annee\n");
                                valide = 0;
                            }
                        }
                        break;
                }
            }
        }

        printf("\n");

    } while (!valide);

    return d;
}
//fonction pour retourner la date actuelle
Date dateActuelle()
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    Date d;
    d.jour = t->tm_mday;
    d.mois = t->tm_mon + 1;
    d.annee = t->tm_year + 1900;

    return d;    
}
//definition de la fonction pour gerer automatiquement le matricule
char *generate_matricule(const char *etablissement, int index) {
    char *code = malloc(30);
    if (!code)
    {
      printf("Erreur d'allocation\n");
      exit(1);
    }
    Date dateActu = dateActuelle();
    int chriffresAnnee = dateActu.annee % 100;//deux derniers chiffres qui compose l'annee
    snprintf(code,30, "%d%s%04d",chriffresAnnee,etablissement,index);
    return code;
}

//enregistrement d'un etudiant dans le fichier
void enregistrerEtudiant(const char *nomFichier,Etudiant student)
{
    FILE *f = fopen(nomFichier, "a");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }
    fprintf(f,"%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",student.matricule,student.nom,student.prenom,student.dateNaissance.jour,student.dateNaissance.mois,student.dateNaissance.annee,student.departement,student.filiere,student.region,student.sexe);
    fclose(f);
}

//fonction pour creer un etudiant en l'ajoutant au fichier par la fonction enregistrerEtudiant
Etudiant creerEtudiant(const char *nomFichier) 
{
    FILE *indexFile = fopen("index.txt", "r");
    int i = 0;
    Etudiant student;
    if (indexFile != NULL) {
        fscanf(indexFile, "%d", &i);
        fclose(indexFile);
    }
    printf("Entrer les informations de l'etudiant a enregistrer:\n");
    printf("Nom : ");
    int c;
    while((c = getchar()) != '\n' && c!= EOF);
    fgets(student.nom, sizeof(student.nom), stdin);
    student.nom[strcspn(student.nom, "\n")] = '\0';
    printf("Prenom : ");
    fgets(student.prenom, sizeof(student.prenom), stdin);
    student.prenom[strcspn(student.prenom, "\n")] = '\0';

    Date present = dateActuelle();
    do
    {
        printf("Date de naissance :\n");
        student.dateNaissance = creerDate();
    } while (student.dateNaissance.annee > present.annee);
    printf("Departement : ");
    fgets(student.departement, sizeof(student.departement), stdin);
    student.departement[strcspn(student.departement, "\n")] = '\0';

    printf("Filiere : ");
    fgets(student.filiere, sizeof(student.filiere), stdin);
    student.filiere[strcspn(student.filiere, "\n")] = '\0';

    printf("Region : ");
    fgets(student.region, sizeof(student.region), stdin);
    student.region[strcspn(student.region, "\n")] = '\0';
    do
    {
        printf("Quel est le sexe ? (M/F)");
        scanf(" %c",&student.sexe);       
    } while ((student.sexe != 'm' && student.sexe != 'M') && (student.sexe != 'f' && student.sexe!='F'));
    char *matricule = generate_matricule("ENSPM", i);
    strncpy(student.matricule, matricule, sizeof(student.matricule)-1);
    student.matricule[sizeof(student.matricule)-1] = '\0';
    free(matricule);

    enregistrerEtudiant(nomFichier,student);
    printf("Un etudiant vient d'etre ajoute avec succes.\n");
    FILE *increment = fopen("index.txt", "w");
    fprintf(increment, "%d", i + 1);
    fclose(increment);
    return student;
}
// fonction pour afficher les donnees d'un etudiant
void afficherEtudiant(Etudiant e)
{
    printf("Matricule: %s\nNom: %s\nprenom: %s\nDate de Naissance: %02d/%02d/%04d\nDepartement: %s\nFiliere: %s\nRegion:%s\nSexe: %c\n",e.matricule,e.nom,e.prenom,e.dateNaissance.jour,e.dateNaissance.mois,e.dateNaissance.annee,e.departement,e.filiere,e.region,e.sexe);
}
//fonction pour afficher la liste des etudiants dans la console
void afficherTousLesEtudiant(const char *nomFichier) 
{
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        printf("Erreur d'ouverture ou aucun etudiant present dans la base de donnees.\n");
        return;
    }
    const int tailleBuffer = 100;
    char buffer[tailleBuffer], matricule[tailleBuffer], nom[tailleBuffer], prenom[tailleBuffer], departement[tailleBuffer], filiere[tailleBuffer],region[tailleBuffer];
    int jj , mm ,aaaa;
    Etudiant e;
    int i =1;

    printf("\n----------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-12s | %-20s | %-15s | %-20s | %-15s | %-15s | %-15s | %-5s |\n",
           "MATRICULE", "NOM", "PRENOM", "DATE DE NAISSANCE", "DEPARTEMENT", "FILIERE", "REGION", "SEXE");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
    while (fgets(buffer,tailleBuffer,f))
    {
        strcpy(matricule,strtok(buffer,"\t"));
        strcpy(nom,strtok(NULL,"\t"));
        strcpy(prenom,strtok(NULL,"\t"));
        jj = atoi(strtok(NULL,"/"));
        mm = atoi(strtok(NULL,"/"));
        aaaa = atoi(strtok(NULL,"/\t"));
        strcpy(departement, strtok(NULL,"\t"));
        strcpy(filiere, strtok(NULL,"\t"));
        strcpy(region,strtok(NULL,"\t"));
        char *tokenSexe = strtok(NULL, "\t\n");
        char sexe = tokenSexe[0];

        printf("| %-12s | %-20s | %-15s | %02d/%02d/%04d           | %-15s | %-15s | %-15s | %-5c |\n",
        matricule,
        nom,
        prenom,
        jj,
        mm,
        aaaa,
        departement,
        filiere,
        region,
        sexe);
        printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
        i++;
    }
    if (i == 1)
    {
        printf("|                                            AUCUN ETUDIANT PRESENT DANS LA BD                                                               |\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
    }

    fclose(f);
}


// //fonctions pour mieux presenter l'affichage du menu

//tracer une ligne
void print_line(char c, int width)
{
    printf(INDENT);
    for(int i = 0; i < width; i++)
        putchar(c);
    putchar('\n');
}
//centrer le message
void print_centered(char *text, int width)
{
    int len = strlen(text);
    int left = (width - len) / 2;
    int right = width - len - left;

    for(int i = 0; i < left; i++) putchar(' ');
    printf("%s", text);
    for(int i = 0; i < right; i++) putchar(' ');
}
//l'entete d tableau
void menu_header(char *header)
{
    printf("\n");
    print_line('=', MENU_WIDTH);
    printf(INDENT"|");
    print_centered(header, MENU_WIDTH - 2);
    printf("|\n");
    print_line('=', MENU_WIDTH);
}
//ca c'est pour les differentes options du menu
void menu_item(char *key, char *value)
{
    printf(INDENT"|  %s. %-72s |\n", key, value);
}
//ligne de fin du tableau du menu
void menu_footer()
{
    print_line('=', MENU_WIDTH);
    printf("\n");
}
//Formatage du message d'alerte
void alert(char *message)
{
    int width = strlen(message) + 10;

    printf("\n");
    printf(INDENT INDENT);
    for(int i = 0; i < width-2; i++) putchar('-');
    printf("\n");

   printf(INDENT INDENT"|   %s   |\n", message);

    printf(INDENT INDENT);
    for(int i = 0; i < width -2; i++) putchar('-');
    printf("\n");
}
//message d'entete genre 'enregistrement d'un nouvel etudiant'
void entete(char *message)
{
    printf("\n");
    print_line('=', MENU_WIDTH);
    printf(INDENT"|");
    print_centered(message, MENU_WIDTH - 2);
    printf("|\n");
    print_line('=', MENU_WIDTH);
    printf("\n");

}
// nettoyage de la console
void cleanConsole()
{
    
    #ifdef _WIN32
        system("cls");
        //printf("La console a ete nettoye");
    #else
        system("clear");
        //printf("La console a ete nettoye");
    #endif
}


void trierParRegion(const char*nomFichier)
{
    int n = nombreEtudiant(nomFichier);
    if (n == 0) {
        printf("Aucun étudiant à trier.\n");
        return;
    }

    Etudiant* etudiants = (Etudiant*)malloc(n * sizeof(Etudiant));
    if (etudiants == NULL) {
        printf("Erreur : mémoire insuffisante.\n");
        return;
    }
    FILE* f = fopen(nomFichier, "r");
    if (f == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        free(etudiants);
        return;
    }

    for (int i = 0; i < n; i++) {
        fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\n",
               etudiants[i].matricule,
               etudiants[i].nom,
               etudiants[i].prenom,
               &etudiants[i].dateNaissance.jour,
               &etudiants[i].dateNaissance.mois,
               &etudiants[i].dateNaissance.annee,
               etudiants[i].departement,
               etudiants[i].filiere,
               etudiants[i].region,
               &etudiants[i].sexe);
    }
    fclose(f);

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcasecmp(etudiants[j].region, etudiants[j + 1].region) > 0) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
   char regionactuelle[10]="";
    printf("Étudiants triés par région :\n");
    for (int i = 0; i < n; i++) {
        if (strcasecmp(regionactuelle, etudiants[i].region) != 0) {
            strcpy(regionactuelle, etudiants[i].region);
            printf("\n--- Région : %s ---\n", regionactuelle);
        }
        afficherEtudiant(etudiants[i]);
        printf("-------------------------------------------\n");
    }
    f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        free(etudiants);
        return;
    }
    
    for (int i = 0; i < n; i++) {
        fprintf(f,
                "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
                etudiants[i].matricule,
                etudiants[i].nom,
                etudiants[i].prenom,
                etudiants[i].dateNaissance.jour,    
                etudiants[i].dateNaissance.mois,    
                etudiants[i].dateNaissance.annee,
                etudiants[i].departement,
                etudiants[i].filiere,
                etudiants[i].region,
                etudiants[i].sexe); 
    }
    fclose(f);  
    free(etudiants);
    printf("Étudiants triés par région avec succès.\n");
}

int rechercherEtudiantParMatricule(const char*matricule,const char*nomFichier)
{

    
    FILE *f = fopen(nomFichier, "r");

    if(!f){
        printf("Erreur lors de l'ouverture du fichier\n");
        return -1; //Erreur d'ouverture du fichier
    }

    if (nombreEtudiant(nomFichier) == 0){
        printf("Aucun étudiant enregistrer\n");
        return -2; //Fichier vide
    }

    Etudiant e;
    char ligne[256];
    int etudiant_trouver = 0 ; 

    while(fgets(ligne, sizeof(ligne), f)){
        int champslus = sscanf(ligne, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c", e.matricule, e.nom, e.prenom, &e.dateNaissance.jour,&e.dateNaissance.mois,&e.dateNaissance.annee, e.departement, e.filiere, e.region, &e.sexe);
        if(champslus == 10){
            if(strcasecmp(e.matricule, matricule) == 0){
                etudiant_trouver = 1;
                break ; 
        }
     } else {
        printf("Format de ligne invalide dans le fichier\n");
        }
    }

    fclose(f);
    if(etudiant_trouver){
        afficherEtudiant(e);
        return 0; //On retourne l'étudiant trouvé
        
    } else {
    printf("L'étudiant avec le matricule %s n'a pas été trouver\n", matricule);
    return -4; //Etudiant non trouvé
    }
}

// comparaison pour le tri par nom
int comparerParNom(const Etudiant *a, const Etudiant *b) {
    int cmpNom = strcasecmp(a->nom, b->nom);
    if (cmpNom != 0) return cmpNom;
    return strcasecmp(a->prenom, b->prenom);
}

// comparaison pour le tri par matricule
int comparerParMatricule(const Etudiant *a, const Etudiant *b) {
    return strcasecmp(a->matricule, b->matricule);
}

// comparaison pour le tri par région
int comparerParRegion(const Etudiant *a, const Etudiant *b) {
    int cmpRegion = strcasecmp(a->region, b->region);
    if (cmpRegion != 0) return cmpRegion;
    return comparerParNom(a, b);
}

// comparaison pour le tri par filière
int comparerParFiliere(const Etudiant *a, const Etudiant *b) {
    int cmpFiliere = strcasecmp(a->filiere, b->filiere);
    if (cmpFiliere != 0) return cmpFiliere;
    return comparerParNom(a, b);
}
// Fonction pour lire, trier et afficher les étudiants
void trierEtAfficher(const char *nomFichier, 
                          int (*comparer)(const Etudiant*, const Etudiant*), 
                          const char *critere) {
    int n = nombreEtudiant(nomFichier);
    if (n == 0) {
        printf("Aucun etudiant a trier.\n");
        return;
    }
    
    
    FILE *fichier = fopen(nomFichier, "r");// Ouvrir le fichier
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }
    
   
    Etudiant *etudiants = malloc(n * sizeof(Etudiant)); // Allouer un tableau pour stocker les étudiants
    if (etudiants == NULL) {
        printf("Erreur d'allocation memoire.\n");
        fclose(fichier);
        return;
    }
    
    // Lire les étudiants depuis le fichier
    int i = 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) && i < n) {
        if (sscanf(ligne, "%s %s %s %d/%d/%d %s %s %s %c",
                   etudiants[i].matricule,
                   etudiants[i].nom,
                   etudiants[i].prenom,
                   &etudiants[i].dateNaissance.jour,
                   &etudiants[i].dateNaissance.mois,
                   &etudiants[i].dateNaissance.annee,
                   etudiants[i].departement,
                   etudiants[i].filiere,
                   etudiants[i].region,
                   &etudiants[i].sexe) == 10) {
            i++;
        }
    }
    fclose(fichier);
    
    //Trier avec le tri à bulles
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (comparer(&etudiants[j], &etudiants[j + 1]) > 0) {
                // Échanger les étudiants
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
    
    // Afficher les étudiants triés
    printf("\n=== LISTE TRIEE PAR %s ===\n", critere);
    printf("Matricule\tNom\t\tPrenom\t\tNaissance\tDepartement\tFiliere\t\t\tRegion\tSexe\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%s\t%-12s\t%-12s\t%02d/%02d/%04d\t%-12s\t%-20s\t%-8s\t%c\n",
               etudiants[i].matricule,
               etudiants[i].nom,
               etudiants[i].prenom,
               etudiants[i].dateNaissance.jour,
               etudiants[i].dateNaissance.mois,
               etudiants[i].dateNaissance.annee,
               etudiants[i].departement,
               etudiants[i].filiere,
               etudiants[i].region,
               etudiants[i].sexe);
    }
    
    printf("\nTotal: %d etudiant(s)\n", n);
    
    //  Libérer la mémoire
    free(etudiants);
}

void trierParFiliere(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParFiliere, "FILIERE");
}
void trierParMatricule(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParMatricule, "MATRICULE");
}


void trierParNom(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParNom, "NOM");
}

int rechercherEtudiantPardichotomique(const char *matricule, const char *nomFichier){
    FILE *f = fopen(nomFichier, "r");
    if(!f){
    printf("Erreur lors de l'ouverture du fichier\n");
    return -1; //Erreur d'ouverture du fichier
    }

    else if(nombreEtudiant(nomFichier) == 0){
        printf("Aucun étudiant dans le fichier\n");
        return -2; //Fichier vide
    }

    // else if(!verifierMatricule(matricule)){
    //     printf("Matricule invalide\n");     //On s'assure que le matricule est valide et que le fichier s'ouvre correctement
    //     return -3; //Matricule invalide
    // }
    trierParMatricule(nomFichier); //On trie le fichier par matricule avant de faire la recherche dichotomique
 
    Etudiant *etudiants = NULL;
    int count = 0;
    char ligne[256];

    rewind(f);
    while(fgets(ligne, sizeof(ligne), f)){
        Etudiant e;
        if(sscanf(ligne, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c", e.matricule, e.nom, e.prenom, &e.dateNaissance.jour,&e.dateNaissance.mois,&e.dateNaissance.annee, e.departement, e.filiere, e.region, &e.sexe )==10){
            etudiants = realloc(etudiants, (count + 1) * sizeof(Etudiant));
            etudiants[count] = e;
            count++;
        }
    }
    fclose(f);

    if(count == 0){
        free(etudiants);
        return -4 ;
    }
    
    int gauche = 0, droite = count - 1;

    while(gauche <= droite){
        int milieu = (gauche + droite) / 2;
        int comparaison = strcasecmp(etudiants[milieu].matricule, matricule);

        if(comparaison == 0){
            afficherEtudiant(etudiants[milieu]);
            free(etudiants);
            return 0; //On retourne l'étudiant trouvé

        } else if(comparaison < 0){
            gauche = milieu + 1;
        } else {
            droite = milieu - 1;
        }
    }
    
    free(etudiants);
    printf("L'étudiant n'a pas été trouver\n");
    return -5; //Etudiant non trouvé
}

int rechercherEtudiantParNom(const char*nom, const char*nomFichier) {
    
    FILE *f = fopen(nomFichier, "r");

    if(!f){
        printf("Erreur lors de l'ouverture du fichier\n");
        return -1; //Erreur d'ouverture du fichier
    }

    if (nombreEtudiant(nomFichier) == 0){
        printf("Aucun étudiant enregistrer\n");
        return -2; //Fichier vide
    }

    Etudiant e;
    char ligne[256];
    int etudiant_trouver = 0 ; 

    while(fgets(ligne, sizeof(ligne), f)){
        int champslus = sscanf(ligne, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c", e.matricule, e.nom, e.prenom, &e.dateNaissance.jour,&e.dateNaissance.mois,&e.dateNaissance.annee, e.departement, e.filiere, e.region, &e.sexe);
        if(champslus == 10){
            if(strcasecmp(e.nom, nom) == 0){
                etudiant_trouver = 1;
                break ; 
        }
     } else {
        printf("Format de ligne invalide dans le fichier\n");
        }
    }

    fclose(f);
    if(etudiant_trouver){
        afficherEtudiant(e);
        return 0; //On retourne l'étudiant trouvé
        
    } else {
    printf("L'étudiant avec le nom %s n'a pas été trouver\n", nom);
    return -4; //Etudiant non trouvé
    }
}


  



