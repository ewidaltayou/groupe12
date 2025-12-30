#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "etudiant.h"

//fonction pour verifier le matricule
int verifierMatricule(const char* matricule) {
    int i;
    if (strlen(matricule) != 18)
        return 0;
    const char prefix[] = "cm-uma-";
    for (i = 0; i < 7; i++) {
        if (tolower(matricule[i]) != prefix[i])
            return 0;
    }
    if (!isdigit(matricule[7]) || !isdigit(matricule[8]))
        return 0;

    const char enspm[] = "enspm";
    for (i = 9; i < 14; i++) {
        if (tolower(matricule[i]) != enspm[i - 9])
            return 0;
    }
    for (i = 14; i < 18; i++) {
        if (!isdigit(matricule[i]))
            return 0;
    }
    return 1;
}

//nombre total d'étudiants
int nombreEtudiant(const char* nomFichier){
    FILE* f = fopen(nomFichier, "r");
    int count = 0;
    Etudiant e;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while (fscanf(f, "%s\t%s\t%s\t%d/%d/%d\t%s\t%s\t%s\t%c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        count++;
    }

    fclose(f);
    return count;
}

//calcul age
int calculAge(const char* matricule,const char* nomFichier){
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;
    int age = -1;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return -1;
    }

    while (fscanf(f, "%s\t%s\t%s\t%d/%d/%d\t%s\t%s\t%s\t%c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        if (strcmp(e.matricule, matricule) == 0) {
            Date currentDate = dateActuelle();
            int currentYear = currentDate.annee;
            age = currentYear - e.dateNaissance.annee;
            break;
        }
    }

    fclose(f);
    if (age == -1) {
        printf("Etudiant avec le matricule %s non trouve.\n", matricule);
    }

    return age;
}

//modifier étudiant
int modifierEtudiant(const char* matricule, const char* nomFichier) {
    int tailletab = nombreEtudiant(nomFichier);
    Etudiant etudiants[tailletab];
    int n = 0; 
    int trouve = 0;

    if(!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0;
    }

    FILE* f = fopen(nomFichier, "r");
    if(f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while(fscanf(f, "%s\t%s\t%s\t%d/%d/%d\t%s\t%s\t%s\t%c",
                 etudiants[n].matricule, etudiants[n].nom, etudiants[n].prenom,
                 &etudiants[n].dateNaissance.jour, &etudiants[n].dateNaissance.mois, &etudiants[n].dateNaissance.annee,
                 etudiants[n].departement, etudiants[n].filiere, etudiants[n].region, &etudiants[n].sexe) != EOF) {
        n++;
        if(n >= tailletab) break;
    }
    fclose(f);

    for(int i = 0; i < n; i++) {
        if(strcmp(etudiants[i].matricule, matricule) == 0) {
            trouve = 1;
            printf("Modification des informations pour l'etudiant %s:\n", matricule);

            printf("Nouveau Nom (%s): ", etudiants[i].nom);         
            scanf("%19s", etudiants[i].nom);
            printf("Nouveau Prenom (%s): ", etudiants[i].prenom);   
            scanf("%19s", etudiants[i].prenom);
            printf("Nouvelle date de naissance:\n");               
             etudiants[i].dateNaissance = creerDate();
            printf("Nouveau Departement (%s): ", etudiants[i].departement); 
            scanf("%19s", etudiants[i].departement);
            printf("Nouvelle Filiere (%s): ", etudiants[i].filiere);        
            scanf("%29s", etudiants[i].filiere);
            printf("Nouvelle Region (%s): ", etudiants[i].region);         
             scanf("%19s", etudiants[i].region);

            do {
                printf("Nouveau Sexe (%c) (M/F): ", etudiants[i].sexe);
                scanf(" %c", &etudiants[i].sexe);
            } while(etudiants[i].sexe != 'M' && etudiants[i].sexe != 'F');
            break;
        }
    }

    if(!trouve) {
        printf("Etudiant %s non trouve.\n", matricule);
        return 0;
    }

    f = fopen(nomFichier, "w");
    if(f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    for(int i = 0; i < n; i++) {
        fprintf(f, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
                etudiants[i].matricule, etudiants[i].nom, etudiants[i].prenom,
                etudiants[i].dateNaissance.jour, etudiants[i].dateNaissance.mois, etudiants[i].dateNaissance.annee,
                etudiants[i].departement, etudiants[i].filiere, etudiants[i].region, etudiants[i].sexe);
    }

    fclose(f);
    printf("Modifications effectuees avec succes.\n");
    return 1;
}

//supprimer étudiant
int supprimerEtudiant(const char* matricule,const char* nomFichier){
    if (!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0;
    }
    FILE* f = fopen(nomFichier, "r");
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

        if (strcmp(e.matricule, matricule) == 0)
            trouve = 1;
        else
            fprintf(temp, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
                    e.matricule, e.nom, e.prenom,
                    e.dateNaissance.jour, e.dateNaissance.mois, e.dateNaissance.annee,
                    e.departement, e.filiere, e.region, e.sexe);
    }

    fclose(f);
    fclose(temp);
    remove(nomFichier);
    rename("temp.txt", nomFichier);
    if (trouve)
        printf("Etudiant %s supprime.\n", matricule);
    else
        printf("Aucun etudiant avec ce matricule.\n");

    return trouve;
}
