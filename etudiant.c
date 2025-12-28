#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include"etudiant.h"
// fonction de verification de la date
int verifierDate(Date d) {
    if(d.annee < 1900 || d.annee > anneeCourante() ||
       d.mois < 1 || d.mois > 12 ||
       d.jour < 1 || d.jour > 31) {
        printf("Date invalide, reessayez\n");
        return 0;
    }
    if ((d.mois == 4 || d.mois == 6 || d.mois == 9 || d.mois == 11) && d.jour > 30)
        printf("Date invalide, reessayez\n");
        return 0;

    if (d.mois == 2) {
        int bissextile = (d.annee % 4 == 0 && d.annee % 100 != 0) || (d.annee % 400 == 0);
        if ((bissextile && d.jour > 29) || (!bissextile && d.jour > 28))
         printf("Date invalide, reessayez\n");
            return 0;
    }

    return 1;
}

   // fonction de creation de la date
Date creerDate(){
    Date d;
    do{
        printf("entrez votre jour de naissance:");
        scanf("%d",&d.jour);
        printf("entrez votre mois de naissance:");
        scanf("%d",&d.mois);
        printf("entrez votre annee de naissance:");
        scanf("%d",&d.annee);
    }while (!verifierDate(d));
    return d;
}
//fonction pour obtenir l'annee courante

int anneeCourante() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900;
}

//determiner le prochain matricule
 int prochainNumero() {
    FILE* f = fopen("compteur.txt", "r");
    int num = 0;

    if (f != NULL) {
        fscanf(f, "%d", &num);
        fclose(f);
    }

    num++;

    f = fopen("compteur.txt", "w");
    fprintf(f, "%d", num);
    fclose(f);

    return num;
}
//generer le matricule
void genererMatricule(char matricule[], int annee, int numero) {
    sprintf(matricule, "cm-uma-%02denspm%04d", annee % 100, numero);
}


//foncton creation etudiant
Etudiant creerEtudiant(const char* nomFichier) {
    FILE* f = fopen(nomFichier, "a");
    Etudiant e;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return e; 
    }

    int numero = prochainNumero();
    int annee = anneeCourante();
    genererMatricule(e.matricule, annee, numero);
    printf("Matricule genere : %s\n", e.matricule);

    printf("Nom : ");
    scanf("%19s", e.nom);

    printf("Prenom : ");
    scanf("%19s", e.prenom);

    e.dateNaissance = creerDate();

    printf("Departement : ");
    scanf("%19s", e.departement);

    printf("Filiere : ");
    scanf("%29s", e.filiere);

    printf("Region : ");
    scanf("%19s", e.region);

    do {
        printf("Sexe (M/F) : ");
        scanf(" %c", &e.sexe);
    } while (e.sexe != 'M' && e.sexe != 'F');

    fprintf(f, "%s %s %s %02d/%02d/%04d %s %s %s %c\n",
            e.matricule, e.nom, e.prenom,
            e.dateNaissance.jour, e.dateNaissance.mois, e.dateNaissance.annee,
            e.departement, e.filiere, e.region, e.sexe);

    fclose(f);
    return e;
}
//fonction pour verifier le matticule
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

//fonction pour afficher un etudiant
void afficherEtudint(Etudiant e) {
    printf("Matricule: %s\n", e.matricule);
    printf("Nom: %s\n", e.nom);
    printf("Prenom: %s\n", e.prenom);
    printf("Date de Naissance: %02d/%02d/%04d\n", e.dateNaissance.jour, e.dateNaissance.mois, e.dateNaissance.annee);
    printf("Departement: %s\n", e.departement);
    printf("Filiere: %s\n", e.filiere);
    printf("Region: %s\n", e.region);
    printf("Sexe: %c\n", e.sexe);
}
//afficher tous les etudiants
void afficherTousLesEtudiant(const char* nomFichier) {
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        afficherEtudint(e);
        printf("-----------------------\n");
    }

    fclose(f);
}
// recherche etuidiant par matricule

int rechercherEtudiantParMatricule(const char* matricule, const char* nomFichier) {
    if(!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0; 
    }

    FILE* f = fopen(nomFichier, "r");
    Etudiant e;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        if (strcmp(e.matricule, matricule) == 0) {
            afficherEtudint(e);
            fclose(f);
            return 1; 
        }
    }

    fclose(f);
    printf("Etudiant avec le matricule %s non trouve.\n", matricule);
    return 0; 
}
// recherche etuidiant par nom
int rechercherEtudiantParNom(const char* nom, const char* nomFichier) {
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;
    int trouve = 0;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        if (strcmp(e.nom, nom) == 0) {
            afficherEtudint(e);
            printf("-----------------------\n");
            trouve = 1;
        }
    }

    fclose(f);
    if (!trouve) {
        printf("Aucun etudiant trouve avec le nom %s.\n", nom);
    }
    return trouve;
}
// recherche etuidiant par dichotomique
int rechercherEtudiantPardichotomique(const char* matricule, const char* nomFichier) {
    if(!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0; 
    }
    int n=nombreEtudiant(nomFichier);
    FILE* f = fopen(nomFichier, "r");
    Etudiant e[n];
    int count = 0;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e[count].matricule, e[count].nom, e[count].prenom,
                  &e[count].dateNaissance.jour, &e[count].dateNaissance.mois, &e[count].dateNaissance.annee,
                  e[count].departement, e[count].filiere, e[count].region, &e[count].sexe) != EOF) {
        count++;
    }

    fclose(f);

    int left = 0, right = count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(e[mid].matricule, matricule);
        if (cmp == 0) {
            afficherEtudint(e[mid]);
            return 1; 
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    printf("Etudiant avec le matricule %s non trouve.\n", matricule);
    return 0; 
}
//fonction du nombre d etudiant
int nombreEtudiant(const char*nomFichier){
    FILE* f = fopen(nomFichier, "r");
    int count = 0;
    Etudiant e;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        count++;
    }

    fclose(f);
    return count;
}
//fonction pour calculer l'age
int calculAge(const char*matricule,const char*nomFichier){
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;
    int age = -1;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return -1;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        if (strcmp(e.matricule, matricule) == 0) {
            int currentYear = anneeCourante();
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
//modifier etudiant
int modifierEtudiant(const char* matricule, const char* nomFichier) {
    int tailletab=nombreEtudiant(nomFichier);
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

    while(fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
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
            printf("Modification des informations pour l'etudiant avec le matricule %s:\n", matricule);
            printf("Nouveau Nom (actuel: %s): ", etudiants[i].nom);
            scanf("%19s", etudiants[i].nom);
            printf("Nouveau Prenom (actuel: %s): ", etudiants[i].prenom);
            scanf("%19s", etudiants[i].prenom);
            printf("Nouvelle Date de Naissance (actuelle: %02d/%02d/%04d):\n",
                   etudiants[i].dateNaissance.jour, etudiants[i].dateNaissance.mois, etudiants[i].dateNaissance.annee);
            etudiants[i].dateNaissance = creerDate();
            printf("Nouveau Departement (actuel: %s): ", etudiants[i].departement);
            scanf("%19s", etudiants[i].departement);
            printf("Nouvelle Filiere (actuelle: %s): ", etudiants[i].filiere);
            scanf("%29s", etudiants[i].filiere);
            printf("Nouvelle Region (actuelle: %s): ", etudiants[i].region);
            scanf("%19s", etudiants[i].region);
            do {
                printf("Nouveau Sexe (actuel: %c) (M/F): ", etudiants[i].sexe);
                scanf(" %c", &etudiants[i].sexe);
            } while(etudiants[i].sexe != 'M' && etudiants[i].sexe != 'F');
            break;
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
        fprintf(f, "%s %s %s %02d/%02d/%04d %s %s %s %c\n",
                etudiants[i].matricule, etudiants[i].nom, etudiants[i].prenom,
                etudiants[i].dateNaissance.jour, etudiants[i].dateNaissance.mois, etudiants[i].dateNaissance.annee,
                etudiants[i].departement, etudiants[i].filiere, etudiants[i].region, etudiants[i].sexe);
    }
    fclose(f);
    printf("Etudiant modifie avec succes.\n");
    return 1;
}
//supprimer etudiant
int supprimerEtudiant(const char*matricule,const char*nomFichier){
    if (!verifierMatricule(matricule)) {
    printf("Matricule invalide.\n");
    return 0;
    }
    FILE*f=fopen(nomFichier, "r");
    FILE* temp = fopen("temp.txt", "w");
    Etudiant e;
    int trouve = 0;

    if (f == NULL || temp == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e.matricule, e.nom, e.prenom,
                  &e.dateNaissance.jour, &e.dateNaissance.mois, &e.dateNaissance.annee,
                  e.departement, e.filiere, e.region, &e.sexe) != EOF) {
        if (strcmp(e.matricule, matricule) == 0) {
            trouve = 1;
        } else {
           fprintf(temp, "%s %s %s %02d/%02d/%04d %s %s %s %c\n",
                e.matricule, e.nom, e.prenom,
                e.dateNaissance.jour, e.dateNaissance.mois, e.dateNaissance.annee,
                e.departement, e.filiere, e.region, e.sexe);
    }}

    fclose(f);
    fclose(temp);
    remove(nomFichier);
    rename("temp.txt", nomFichier);

    if (trouve) {
        printf("Etudiant avec le matricule %s supprime avec succes.\n", matricule);
    } else {
        printf("Etudiant avec le matricule %s non trouve.\n", matricule);
    }
    return trouve;
}
//tri par matricule
void trierParMatricule(const char*nomFichier){  
    int n = nombreEtudiant(nomFichier);
    if(n==0){
        printf("Aucun etudiant dans le fichier.\n");
        return;
    }
    FILE* f = fopen(nomFichier, "r");
    Etudiant e[n];
    int count = 0;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e[count].matricule, e[count].nom, e[count].prenom,
                  &e[count].dateNaissance.jour, &e[count].dateNaissance.mois, &e[count].dateNaissance.annee,
                  e[count].departement, e[count].filiere, e[count].region, &e[count].sexe) != EOF) {
        count++;
    }

    fclose(f);
    for (int i = 1; i < count; i++) {
        Etudiant key = e[i];
        int j = i - 1;
        while (j >= 0 && strcmp(e[j].matricule, key.matricule) > 0) {
            e[j + 1] = e[j];
            j--;
        }
        e[j + 1] = key;
    }
    for(int i=0;i<count;i++){
        printf("Etudiant %d:\n", i + 1);
        afficherEtudint(e[i]);
        printf("-----------------------\n");
    }

    f = fopen(nomFichier, "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %s %s %02d/%02d/%04d %s %s %s %c\n",
                e[i].matricule, e[i].nom, e[i].prenom,
                e[i].dateNaissance.jour, e[i].dateNaissance.mois, e[i].dateNaissance.annee,
                e[i].departement, e[i].filiere, e[i].region, e[i].sexe);
    }

    fclose(f);
}
//tri par nom

void trierParNom(const char*nomFichier){  
    int n = nombreEtudiant(nomFichier);
    if(n==0){
        printf("Aucun etudiant dans le fichier.\n");
        return;
    }
    FILE* f = fopen(nomFichier, "r");
    Etudiant e[n];
    int count = 0;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e[count].matricule, e[count].nom, e[count].prenom,
                  &e[count].dateNaissance.jour, &e[count].dateNaissance.mois, &e[count].dateNaissance.annee,
                  e[count].departement, e[count].filiere, e[count].region, &e[count].sexe) != EOF) {
        count++;
    }

    fclose(f);
    for (int i = 1; i < count; i++) {
        Etudiant key = e[i];
        int j = i - 1;
        while (j >= 0 && strcmp(e[j].nom, key.nom) > 0) {
            e[j + 1] = e[j];
            j--;
        }
        e[j + 1] = key;
    }
    for(int i=0;i<count;i++){
        printf("Etudiant %d:\n", i + 1);
        afficherEtudint(e[i]);
        printf("-----------------------\n");
    }

    f = fopen(nomFichier, "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %s %s %02d/%02d/%04d %s %s %s %c\n",
                e[i].matricule, e[i].nom, e[i].prenom,
                e[i].dateNaissance.jour, e[i].dateNaissance.mois, e[i].dateNaissance.annee,
                e[i].departement, e[i].filiere, e[i].region, e[i].sexe);
    }

    fclose(f);
}
//tri par region    
void trierParRegion(const char*nomFichier){  
    int n = nombreEtudiant(nomFichier);
    if(n==0){
        printf("Aucun etudiant dans le fichier.\n");
        return;
    }
    FILE* f = fopen(nomFichier, "r");
    Etudiant e[n];
    int count = 0;

    if (f == NULL) {
        printf("aucun etudiant dans le fichier.\n");
        return;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e[count].matricule, e[count].nom, e[count].prenom,
                  &e[count].dateNaissance.jour, &e[count].dateNaissance.mois, &e[count].dateNaissance.annee,
                  e[count].departement, e[count].filiere, e[count].region, &e[count].sexe) != EOF) {
        count++;
    }

    fclose(f);
    if(count==0){
        printf("Aucun etudiant dans le fichier.\n");
        return;
    }
    for (int i = 1; i < count; i++) {
        Etudiant key = e[i];
        int j = i - 1;
        while (j >= 0 && strcmp(e[j].region, key.region) > 0) {
            e[j + 1] = e[j];
            j--;
        }
        e[j + 1] = key;
    }
    char regionActuelle[20];
    strcpy(regionActuelle, e[0].region);
    printf("Region: %s\n", regionActuelle);
    for(int i=0;i<count;i++){
        if(strcmp(regionActuelle,e[i].region)!=0){
            strcpy(regionActuelle,e[i].region);
            printf("Region: %s\n",regionActuelle);
        }
        printf("Etudiant %d:\n", i + 1);
        afficherEtudint(e[i]);
        printf("-----------------------\n");
    }

    f = fopen(nomFichier, "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %s %s %02d/%02d/%04d %s %s %s %c\n",
                e[i].matricule, e[i].nom, e[i].prenom,
                e[i].dateNaissance.jour, e[i].dateNaissance.mois, e[i].dateNaissance.annee,
                e[i].departement, e[i].filiere, e[i].region, e[i].sexe);
    }

    fclose(f);
}
//tri par filiere
 void trierParFiliere(const char*nomFichier){  
    int n = nombreEtudiant(nomFichier);
    if(n==0){
        printf("Aucun etudiant enregistre.\n");
        return;
    }
    FILE*f = fopen(nomFichier, "r");
    Etudiant e[n];
    int count = 0;

    if (f == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    while (fscanf(f, "%s %s %s %d/%d/%d %s %s %s %c",
                  e[count].matricule, e[count].nom, e[count].prenom,
                  &e[count].dateNaissance.jour, &e[count].dateNaissance.mois, &e[count].dateNaissance.annee,
                  e[count].departement, e[count].filiere, e[count].region, &e[count].sexe) != EOF) {
        count++;
    }

    fclose(f);
    for (int i = 1; i < count; i++) {
        Etudiant key = e[i];
        int j = i - 1;
        while (j >= 0 && strcmp(e[j].filiere, key.filiere) > 0) {
            e[j + 1] = e[j];
            j--;
        }
        e[j + 1] = key;
    }
    char filiereActuelle[30];
    strcpy(filiereActuelle, e[0].filiere);
    printf("Filiere: %s\n", filiereActuelle);
    for(int i=0;i<count;i++){
        if(strcmp(filiereActuelle,e[i].filiere)!=0){
            strcpy(filiereActuelle,e[i].filiere);
            printf("Filiere: %s\n",filiereActuelle);
        }
        printf("Etudiant %d:\n", i + 1);
        afficherEtudint(e[i]);
        printf("-----------------------\n");
    }

    f = fopen(nomFichier, "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %s %s %02d/%02d/%04d %s %s %s %c\n",
                e[i].matricule, e[i].nom, e[i].prenom,
                e[i].dateNaissance.jour, e[i].dateNaissance.mois, e[i].dateNaissance.annee,
                e[i].departement, e[i].filiere, e[i].region, e[i].sexe);
    }

    fclose(f);
 }