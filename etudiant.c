#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "etudiant.h"
#include <time.h>
#include <ctype.h>
//creer date
Date creerDate(){
    Date d;
    do{
    printf("entrez le jour de naissance:");
    scanf("%d",&d.jour);
    printf("entrez le mois de naissance:");
    scanf("%d",&d.mois);
    printf("entrez l'annee de naissance:");
    scanf("%d",&d.annee);
}while(!verifierDate(d));
    return d;
}
//verifier date
int verifierDate(Date d) {
    if(d.annee < 1900 || d.annee > anneeCourante() ||
       d.mois < 1 || d.mois > 12 ||
       d.jour < 1 || d.jour > 31) {
        printf("Date invalide, reessayez\n");
        return 0;
    }
    if ((d.mois == 4 || d.mois == 6 || d.mois == 9 || d.mois == 11) && d.jour > 30)
        return 0;

    if (d.mois == 2) {
        int bissextile = (d.annee % 4 == 0 && d.annee % 100 != 0) || (d.annee % 400 == 0);
        if ((bissextile && d.jour > 29) || (!bissextile && d.jour > 28))
            return 0;
    }
}

Etudiant creerEtudiant(const char *nomFichier) {
    Etudiant e;
    FILE *f;

    int annee = anneeCourante();
    int numero = prochainNumero();

    genererMatricule(e.matricule, annee, numero);

    printf("\n=========== CREATION D'UN ETUDIANT ===========\n");
   do{
    printf("Nom  : ");
    fgets(e.nom, sizeof(e.nom), stdin);
    e.nom[strcspn(e.nom, "\n")] = 0;
    printf("Prénom : ");
    fgets(e.prenom, sizeof(e.prenom), stdin);
    e.prenom[strcspn(e.prenom, "\n")] = 0;
   do {
        printf("Région : ");
        fgets(e.region, sizeof(e.region), stdin);
        e.region[strcspn(e.region, "\n")] = 0;
    } while (!verifierRegion(e.region));
    printf("Département : ");
    fgets(e.departement, sizeof(e.departement), stdin);
    e.departement[strcspn(e.departement, "\n")] = 0;
    printf("Filière : ");
    fgets(e.filiere, sizeof(e.filiere), stdin);
    e.filiere[strcspn(e.filiere, "\n")] = 0;
    do {
        printf("Sexe (M/F) : ");
        scanf(" %c", &e.sexe);
        e.sexe = toupper(e.sexe);
        getchar(); 
    } while (e.sexe != 'M' && e.sexe != 'F');
     do {
        e.dateNaissance = creerDate();
    } while (!verifierDate(e.dateNaissance));
    viderBuffer();
    }while(videChamps(e));
    f = fopen(nomFichier, "a");
    if (f == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(f,
       "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
        e.matricule,
        e.nom,
        e.prenom,
        e.dateNaissance.jour,
        e.dateNaissance.mois,
        e.dateNaissance.annee,
        e.departement,
        e.filiere,
        e.region,
        e.sexe
    );

    fclose(f);

    printf(" Étudiant créé avec succès !\n");
    printf("Matricule : %s\n", e.matricule);

    return e;
}
//afficher etudiant
void afficherEtudint(Etudiant e){
    printf("Matricule: %s\n",e.matricule);
    printf("Nom: %s\n",e.nom);
    printf("Prenom: %s\n",e.prenom);
    printf("Date de naissance: %02d/%02d/%04d\n",e.dateNaissance.jour,e.dateNaissance.mois,e.dateNaissance.annee);
    printf("Departement: %s\n",e.departement);
    printf("Filiere: %s\n",e.filiere);
    printf("Region: %s\n",e.region);
    printf("Sexe: %c\n",e.sexe);
}
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
//afficher tous les etudiants
void afficherTousLesEtudiant(const char* nomFichier) {      
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;

    if (f == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        return;
    }

    printf("\n=========== LISTE DES ETUDIANTS ===========\n");

    while (fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\n",
                  e.matricule,
                  e.nom,
                  e.prenom,
                  &e.dateNaissance.jour,
                  &e.dateNaissance.mois,
                  &e.dateNaissance.annee,
                  e.departement,
                  e.filiere,
                  e.region,
                  &e.sexe) != EOF) {
        afficherEtudint(e);
        printf("-------------------------------------------\n");
    }

    fclose(f);
}   
//calculer age
int calculAge(const char* matricule, const char* nomFichier) {  
    if(!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return -1;
    }
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;
    int age = -1;

    if (f == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        return age;
    }

    while (fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\n",
                  e.matricule,
                  e.nom,
                  e.prenom,
                  &e.dateNaissance.jour,
                  &e.dateNaissance.mois,
                  &e.dateNaissance.annee,
                  e.departement,
                  e.filiere,
                  e.region,
                  &e.sexe) != EOF) {
        if (strcmp(e.matricule, matricule) == 0) {
            int currentYear = anneeCourante();
            age = currentYear - e.dateNaissance.annee;
            break;
        }
    }

    fclose(f);
    return age;
}
//nombre etudiant
int nombreEtudiant(const char* nomFichier) {    
    FILE* f = fopen(nomFichier, "r");
    int count = 0;
    Etudiant e;

    if (f == NULL) {
        printf("aucun etudiant enregistrer.\n");
        return count;
    }

    while (fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\n",
                  e.matricule,
                  e.nom,
                  e.prenom,
                  &e.dateNaissance.jour,
                  &e.dateNaissance.mois,
                  &e.dateNaissance.annee,
                  e.departement,
                  e.filiere,
                  e.region,
                  &e.sexe) != EOF) {
        count++;
    }

    fclose(f);
    return count;
}
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
//supprimer etudiant
int supprimerEtudiant(const char* matricule, const char* nomFichier) { 
    if(!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0;
    }
    FILE* f = fopen(nomFichier, "r");
    FILE* temp = fopen("temp.txt", "w");
    Etudiant e;
    int found = 0;

    if (f == NULL || temp == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        return 0;
    }

    while (fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\n",
                  e.matricule,
                  e.nom,
                  e.prenom,
                  &e.dateNaissance.jour,
                  &e.dateNaissance.mois,
                  &e.dateNaissance.annee,
                  e.departement,
                  e.filiere,
                  e.region,
                  &e.sexe) != EOF) {
        if (strcmp(e.matricule, matricule) == 0) {
            found = 1;
            continue; 
        }
        fprintf(temp,
               "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
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
    }

    fclose(f);
    fclose(temp);

    remove(nomFichier);
    rename("temp.txt", nomFichier);

    if (found) {
        printf("Étudiant avec le matricule %s supprimé avec succès.\n", matricule);
    } else {
        printf("Étudiant avec le matricule %s non trouvé.\n", matricule);
    }

    return found;
}
//modifier etudiant
int modifierEtudiant(const char *matricule, const char *nomFichier) {
    if(!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0;
    }
    int n = nombreEtudiant(nomFichier);
    if (n <= 0) {
        printf("Aucun étudiant trouvé.\n");
        return 0;
    }

    Etudiant *tab = malloc(n * sizeof(Etudiant));
    if (!tab) return 0;

    FILE *f = fopen(nomFichier, "r");
    if (!f) {
        free(tab);
        printf("Erreur: ouverture du fichier impossible.\n");
        return 0;
    }

    int i;
    for (i = 0; i < n; i++) {
        fscanf(f,
            "%18[^\t]\t%29[^\t]\t%29[^\t]\t%d/%d/%d\t%29[^\t]\t%29[^\t]\t%29[^\t]\t %c\n",
            tab[i].matricule,
            tab[i].nom,
            tab[i].prenom,
            &tab[i].dateNaissance.jour,
            &tab[i].dateNaissance.mois,
            &tab[i].dateNaissance.annee,
            tab[i].departement,
            tab[i].filiere,
            tab[i].region,
            &tab[i].sexe
        );
    }
    fclose(f);

    int trouve = 0;
    char buffer[50];

    for (i = 0; i < n; i++) {
        if (strcmp(tab[i].matricule, matricule) == 0) {
            trouve = 1;
            do {
            printf("\nModification des informations pour %s\n", matricule);

            printf("entrez Nouveau nom (actuel:%s): ", tab[i].nom);
            fgets(tab[i].nom, sizeof(tab[i].nom), stdin);
            tab[i].nom[strcspn(tab[i].nom, "\n")] = 0;

            printf("entrez Nouveau prénom (actuel:%s): ", tab[i].prenom);
            fgets(tab[i].prenom, sizeof(tab[i].prenom), stdin);
            tab[i].prenom[strcspn(tab[i].prenom, "\n")] = 0;

            printf("entrez Nouveau département (actuel:%s): ", tab[i].departement);
            fgets(tab[i].departement, sizeof(tab[i].departement), stdin);
            tab[i].departement[strcspn(tab[i].departement, "\n")] = 0;

            printf("entrez Nouvelle filière (actuelle:%s): ", tab[i].filiere);
            fgets(tab[i].filiere, sizeof(tab[i].filiere), stdin);
            tab[i].filiere[strcspn(tab[i].filiere, "\n")] = 0;
            do {
                      printf("entrez Nouvelle région (actuelle:%s): ", tab[i].region);
                      fgets(tab[i].region, sizeof(tab[i].region), stdin);
                      tab[i].region[strcspn(tab[i].region, "\n")] = 0;
           } while (!verifierRegion(tab[i].region));
            do {
                printf("entrez le Sexe (M/F):(actuel:%c) ", tab[i].sexe);
                fgets(buffer, sizeof(buffer), stdin);
                tab[i].sexe = toupper(buffer[0]);
            } while (tab[i].sexe != 'M' && tab[i].sexe != 'F');
            
            do {
                printf("Nouvelle date de naissance:(actuelle:%02d/%02d/%04d)\n", tab[i].dateNaissance.jour, tab[i].dateNaissance.mois, tab[i].dateNaissance.annee);
                tab[i].dateNaissance = creerDate();
            } while (!verifierDate(tab[i].dateNaissance));
            }while(videChamps(tab[i]));
        }
    }

    if (!trouve) {
        free(tab);
        printf("Étudiant non trouvé.\n");
        return 0;
    }

    f = fopen(nomFichier, "w");
    for (i = 0; i < n; i++) {
        fprintf(f,
            "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
            tab[i].matricule,
            tab[i].nom,
            tab[i].prenom,
            tab[i].dateNaissance.jour,
            tab[i].dateNaissance.mois,
            tab[i].dateNaissance.annee,
            tab[i].departement,
            tab[i].filiere,
            tab[i].region,
            tab[i].sexe
        );
    }

    fclose(f);
    free(tab);

    printf("Étudiant avec le matricule %s modifié avec succès.\n", matricule);
    return 1;
}

//rechercher etudiant par matricule
int rechercherEtudiantParMatricule(const char* matricule, const char* nomFichier) {    
    if(!verifierMatricule(matricule)) {
        printf("Matricule invalide.\n");
        return 0;
    }
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;
    int found = 0;

    if (f == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        return found;
    }

    while (fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\n",
                  e.matricule,
                  e.nom,
                  e.prenom,
                  &e.dateNaissance.jour,
                  &e.dateNaissance.mois,
                  &e.dateNaissance.annee,
                  e.departement,
                  e.filiere,
                  e.region,
                  &e.sexe) != EOF) {
        if (strcmp(e.matricule, matricule) == 0) {
            afficherEtudint(e);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Étudiant avec le matricule %s non trouvé.\n", matricule);
    }

    fclose(f);
    return found;
}
//rechercher etudiant par nom
int rechercherEtudiantParNom(const char* nom, const char* nomFichier) {   
    FILE* f = fopen(nomFichier, "r");
    Etudiant e;
    int found = 0;

    if (f == NULL) {
        printf("Erreur : ouverture du fichier impossible.\n");
        return found;
    }

    while (fscanf(f, "%[^\t]\t%[^\t]\t%[^\t]\t%d/%d/%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\n",
                  e.matricule,
                  e.nom,
                  e.prenom,
                  &e.dateNaissance.jour,
                  &e.dateNaissance.mois,
                  &e.dateNaissance.annee,
                  e.departement,
                  e.filiere,
                  e.region,
                  &e.sexe) != EOF) {
        if (strcmp(e.nom, nom) == 0) {
            afficherEtudint(e);
            printf("-------------------------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("Aucun étudiant trouvé avec le nom %s.\n", nom);
    }

    fclose(f);
    return found;
}
//trier par nom
void trierParNom(const char* nomFichier) {  
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
            if (strcmp(etudiants[j].nom, etudiants[j + 1].nom) > 0) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
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
    printf("Étudiants triés par nom avec succès.\n");
}       
//trier par matricule
void trierParMatricule(const char* nomFichier) {    
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
            if (strcmp(etudiants[j].matricule, etudiants[j + 1].matricule) > 0) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
     for(int i=0;i<n;i++){
        printf("Etudiant %d:\n", i + 1);
        afficherEtudint(etudiants[i]);
        printf("-----------------------\n");
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
    printf("Étudiants triés par matricule avec succès.\n");
}
//trier par region
void trierParRegion(const char* nomFichier) {
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
            if (strcmp(etudiants[j].region, etudiants[j + 1].region) > 0) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
   char regionactuelle[10]="";
    printf("Étudiants triés par région :\n");
    for (int i = 0; i < n; i++) {
        if (strcmp(regionactuelle, etudiants[i].region) != 0) {
            strcpy(regionactuelle, etudiants[i].region);
            printf("\n--- Région : %s ---\n", regionactuelle);
        }
        afficherEtudint(etudiants[i]);
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
//trier par filiere
void trierParFiliere(const char* nomFichier) {    
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
            if (strcmp(etudiants[j].filiere, etudiants[j + 1].filiere) > 0) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
    char filiereActuelle[30]="";
    strcpy(filiereActuelle, etudiants[0].filiere);
    printf("Filiere: %s\n", filiereActuelle);
    for(int i=0;i<n;i++){
        if(strcmp(filiereActuelle,etudiants[i].filiere)!=0){
            strcpy(filiereActuelle,etudiants[i].filiere);
            printf("Filiere: %s\n",filiereActuelle);
        }
        printf("Etudiant %d:\n", i + 1);
        afficherEtudint(etudiants[i]);
        printf("-----------------------\n");
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
    printf("Étudiants triés par filière avec succès.\n");
}
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//verifier region
int verifierRegion(const char* region) {
    const char* regionsValides[] = {
        "adamaoua", "centre", "est", "extrême-nord",
        "littoral", "nord", "nord-ouest", "ouest",
        "sud", "sud-ouest"
    };
    int n = sizeof(regionsValides) / sizeof(regionsValides[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(region, regionsValides[i]) == 0) {
            return 1; 
        }
    }
    printf("Région invalide. Régions valides sont:\n");
    for (int i = 0; i < n; i++) {
        printf("- %s\n", regionsValides[i]);
    }       
    return 0; 
}
int videChamps(Etudiant e) {
    if (strlen(e.matricule) == 0 || strlen(e.nom) == 0 || strlen(e.prenom) == 0 ||
           strlen(e.region) == 0 || strlen(e.departement) == 0 || strlen(e.filiere) == 0) {
           printf("Tous les champs doivent être remplis.\n");
        return 1;
    }
    return 0;
}
