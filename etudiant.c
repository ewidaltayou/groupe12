#include <stdio.h>
#include <time.h>
#include "etudiant.h"
#include <stdlib.h>
#include <string.h>

void saisirDate(Date d)
{
    do
    {
        printf("veuillez entrer convenablement la date dans le cas contraire vous serrez contraint de recommencer \n");
        printf("\tJour: ");
        scanf("%d", &d.jour);

        printf("\tMois: ");
        scanf("%d", &d.mois);

        printf("\tAnnee: ");
        scanf("%d", &d.annee);

        if (d.mois == 2 && d.jour > 29)
        {
            printf("Le mois de fevrier n'a jamais eu plus de 29 jours\n");
    
        }
        if (d.mois > 12 && d.jour > 31)
        {
          printf("La date entree est non valide\n");
        }
        
        printf("\n");
    } while ((d.jour>31 || d.mois > 12) || (d.mois == 2 && d.jour > 29));
}

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

//definition d'un fonction pour pour les messages d'alertes
void alert(char* message)
{
    int i=0;
    printf("\n\n\t\t\t\t-------");
    for(i=0; i<strlen(message); i++) printf("-");
    printf("-------\n");
    printf("\t\t\t\t|      %s      |\n", message);
    printf("\t\t\t\t-------");
    for(i=0; i<strlen(message); i++) printf("-");
    printf("-------\n");
}
//definition de la fonction pour formater les entetes

void entete(char* message)
{
    int i;
    printf("\n\n\t----------------------------------");
    for(i=0; i<strlen(message); i++) printf("-");
    printf("----------------------------------\n");

    printf("\t|                                 %s                                 |\n", message);

    printf("\t----------------------------------");
    for(i=0; i<strlen(message); i++) printf("-");
    printf("----------------------------------\n\n");
}

char *generate_matricule(char *structure, int index) {
    char *code = malloc(30);
    if (!code)
    {
      printf("Erreur d'allocation\n");
      exit(1);
    }
    
    Date dateActu = dateActuelle();
    int chrifreAns = dateActu.annee % 100;//deux derniers chiffres qui compose l'annee
    snprintf(code,30, "%d%s%04d",chrifreAns,structure,index);
    return code;
}
//enregistrement d'un etudiant dans le fichier
void enregistrerEtudiant(Etudiant student)
{
    FILE *f = fopen("student.txt", "a");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }
    fprintf(f,"\n%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c",student.matricule,student.nom,student.prenom,student.dateNaissance.jour,student.dateNaissance.mois,student.dateNaissance.annee,student.departement,student.filiere,student.region,student.sexe);
    fclose(f);
}
//creation d'un nouvel etudiant
void saisirEtudiant(Etudiant student) {
    entete("Enregistrement d'un etudiant");
    FILE *indexFile = fopen("index.txt", "r");
    int i = 0;

    if (indexFile != NULL) {
        fscanf(indexFile, "%d", &i);
        fclose(indexFile);
    }
    
    printf("Entrer les informations de l'etudiant a enregistrer:\n");

    printf("Nom : ");
    scanf("%19s", student.nom);

    printf("Prenom : ");
    scanf("%19s", student.prenom);

    printf("Date de naissance :\n");
    saisirDate(student.dateNaissance);

    printf("Departement : ");
    scanf("%19s", student.departement);

    printf("Filiere : ");
    scanf("%19s", student.filiere);

    printf("Region : ");
    scanf("%19s", student.region);
    printf("Quel est le sexe ? (M/F)");
    scanf(" %c",&student.sexe);

    char *matricule = generate_matricule("ENSPM", i);
    strncpy(student.matricule, matricule, 19);
    student.matricule[19] = '\0';
    free(matricule);

    enregistrerEtudiant(student);
    alert("Un etudiant vient d'etre ajoute avec succes.");
    FILE *increment = fopen("index.txt", "w");
    fprintf(increment, "%d", i + 1);
    fclose(increment);
}

void afficherTousLesEtudiant() {
    FILE *f = fopen("student.txt", "r");
    if (f == NULL) {
        alert("Aucun etudiant present dans la base de donnees.\n");
        return;
    }

    Etudiant e;

    printf("\n------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-12s | %-10s | %-10s | %-20s | %-12s | %-10s | %-10s | %-5s |\n",
           "MATRICULE", "NOM", "PRENOM", "DATE DE NAISSANCE", "DEPARTEMENT", "FILIERE", "REGION", "SEXE");
    printf("------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(f,
        "%19s\t%19s\t%19s\t%d/%d/%d\t%19s\t%19s\t%19s\t%c",
        e.matricule,
        e.nom,
        e.prenom,
        &e.dateNaissance.jour,
        &e.dateNaissance.mois,
        &e.dateNaissance.annee,
        e.departement,
        e.filiere,
        e.region,
        &e.sexe) == 10) {

        printf("| %-12s | %-10s | %-10s | %02d/%02d/%04d           | %-12s | %-10s | %-10s | %-5c |\n",
               e.matricule,
               e.nom,
               e.prenom,
               e.dateNaissance.jour,
               e.dateNaissance.mois,
               e.dateNaissance.annee,
               e.departement,
               e.filiere,
               e.region,
               e.sexe);
            printf("------------------------------------------------------------------------------------------------------------------\n");
        }

   // printf("--------------------------------------------------------------------------------------------------------\n");
    fclose(f);
}
