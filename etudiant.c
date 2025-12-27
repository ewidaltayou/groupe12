#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "Etudiant.h"


int dateValide(Date *d) {
    if (d->annee < 1900 || d->annee > anneeCourante()) return 0;
    if (d->mois < 1 || d->mois > 12) return 0;
    
    int joursDansMois[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Gestion des années bissextiles
    if ((d->annee % 4 == 0 && d->annee % 100 != 0) || (d->annee % 400 == 0)) {
        joursDansMois[1] = 29;
    }
    
    if (d->jour < 1 || d->jour > joursDansMois[d->mois - 1]) return 0;
    return 1;
}

int anneeCourante() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return tm->tm_year + 1900;
}

Date creerDate() {
    Date d;
    do {
        printf("Jour : ");
        scanf("%d", &d.jour);
        printf("Mois : ");
        scanf("%d", &d.mois);
        printf("Annee : ");
        scanf("%d", &d.annee);
        
        if (!dateValide(&d)) {
            printf("Date invalide. Veuillez reessayer.\n");
        }
    } while (!dateValide(&d));
    
    // Vider le buffer d'entrée
    while (getchar() != '\n');
    
    return d;
}

int verifierDate(Date d) {
    return dateValide(&d);
}


void sauvegarderEtudiant(const Etudiant *e, FILE *fichier) {
    fprintf(fichier, "%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",
            e->matricule,
            e->nom,
            e->prenom,
            e->dateNaissance.jour,
            e->dateNaissance.mois,
            e->dateNaissance.annee,
            e->departement,
            e->filiere,
            e->region,
            e->sexe);
}

Etudiant lireEtudiant(FILE *fichier) {
    Etudiant e;
    char ligne[200];
    
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        // Retourner un étudiant vide si fin de fichier
        e.matricule[0] = '\0';
        return e;
    }
    
    // Parser la ligne avec les tabulations
    char *token = strtok(ligne, "\t");
    if (token) strncpy(e.matricule, token, sizeof(e.matricule) - 1);
    
    token = strtok(NULL, "\t");
    if (token) strncpy(e.nom, token, sizeof(e.nom) - 1);
    
    token = strtok(NULL, "\t");
    if (token) strncpy(e.prenom, token, sizeof(e.prenom) - 1);
    
    token = strtok(NULL, "\t");
    if (token) sscanf(token, "%d/%d/%d", 
                      &e.dateNaissance.jour, 
                      &e.dateNaissance.mois, 
                      &e.dateNaissance.annee);
    
    token = strtok(NULL, "\t");
    if (token) strncpy(e.departement, token, sizeof(e.departement) - 1);
    
    token = strtok(NULL, "\t");
    if (token) strncpy(e.filiere, token, sizeof(e.filiere) - 1);
    
    token = strtok(NULL, "\t");
    if (token) strncpy(e.region, token, sizeof(e.region) - 1);
    
    token = strtok(NULL, "\n");
    if (token) e.sexe = token[0];
    
    return e;
}


void genererMatricule(char matricule[], int annee, int numero) {
    sprintf(matricule, "%04d%05d", annee, numero);
}

int prochainNumero(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) return 1;
    
    Etudiant e;
    int maxNum = 0;
    
    while (1) {
        e = lireEtudiant(fichier);
        if (e.matricule[0] == '\0') break;
        
        int num;
        sscanf(e.matricule + 4, "%d", &num);
        if (num > maxNum) maxNum = num;
    }
    
    fclose(fichier);
    return maxNum + 1;
}

Etudiant creerEtudiant(const char *nomFichier) {
    Etudiant e;
    
    printf("\n=== CREATION D'UN NOUVEL ETUDIANT ===\n");
    
    // Générer matricule
    int numero = prochainNumero(nomFichier);
    genererMatricule(e.matricule, anneeCourante(), numero);
    printf("Matricule genere : %s\n", e.matricule);
    
    // Saisie des informations
    printf("Nom : ");
    fgets(e.nom, sizeof(e.nom), stdin);
    e.nom[strcspn(e.nom, "\n")] = '\0';
    
    printf("Prenom : ");
    fgets(e.prenom, sizeof(e.prenom), stdin);
    e.prenom[strcspn(e.prenom, "\n")] = '\0';
    
    printf("Date de naissance :\n");
    e.dateNaissance = creerDate();
    
    printf("Departement : ");
    fgets(e.departement, sizeof(e.departement), stdin);
    e.departement[strcspn(e.departement, "\n")] = '\0';
    
    printf("Filiere : ");
    fgets(e.filiere, sizeof(e.filiere), stdin);
    e.filiere[strcspn(e.filiere, "\n")] = '\0';
    
    printf("Region : ");
    fgets(e.region, sizeof(e.region), stdin);
    e.region[strcspn(e.region, "\n")] = '\0';
    
    do {
        printf("Sexe (M/F) : ");
        scanf("%c", &e.sexe);
        e.sexe = toupper(e.sexe);
        while (getchar() != '\n');
    } while (e.sexe != 'M' && e.sexe != 'F');
    
    // Enregistrer dans le fichier texte
    FILE *fichier = fopen(nomFichier, "a");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return e;
    }
    
    sauvegarderEtudiant(&e, fichier);
    fclose(fichier);
    
    printf("Etudiant cree avec succes !\n");
    return e;
}

int nombreEtudiant(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) return 0;
    
    int count = 0;
    char ligne[200];
    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strlen(ligne) > 1) {  // Ignorer les lignes vides
            count++;
        }
    }
    
    fclose(fichier);
    return count;
}

void afficherTousLesEtudiants(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Aucun etudiant enregistre.\n");
        return;
    }
    
    Etudiant e;
    int count = 0;
    
    printf("\n=== LISTE DE TOUS LES ETUDIANTS ===\n");
    printf("Matricule\tNom\tPrenom\tFiliere\tRegion\tSexe\n");
    printf("-----------------------------------------------------\n");
    
    while (1) {
        e = lireEtudiant(fichier);
        if (e.matricule[0] == '\0') break;
        
        printf("%s\t%s\t%s\t%s\t%s\t%c\n", 
               e.matricule, e.nom, e.prenom, e.filiere,e.region,e.sexe);
        count++;
    }
    
    fclose(fichier);
    
    if (count == 0) {
        printf("Aucun etudiant trouve.\n");
    } else {
        printf("\nTotal : %d etudiant(s)\n", count);
    }
}



// ==================== FONCTIONS DE TRI (affichage seulement) ====================

// Fonctions de comparaison
int comparerParNom(const Etudiant *a, const Etudiant *b) {
    int cmpNom = strcasecmp(a->nom, b->nom);
    if (cmpNom != 0) return cmpNom;
    return strcasecmp(a->prenom, b->prenom);
}

int comparerParMatricule(const Etudiant *a, const Etudiant *b) {
    return strcmp(a->matricule, b->matricule);
}

int comparerParRegion(const Etudiant *a, const Etudiant *b) {
    int cmpRegion = strcasecmp(a->region, b->region);
    if (cmpRegion != 0) return cmpRegion;
    return comparerParNom(a, b);
}

int comparerParFiliere(const Etudiant *a, const Etudiant *b) {
    int cmpFiliere = strcasecmp(a->filiere, b->filiere);
    if (cmpFiliere != 0) return cmpFiliere;
    return comparerParNom(a, b);
}

// Fonction générique pour trier et afficher
void trierEtAfficher(const char *nomFichier, int (*comparer)(const Etudiant*, const Etudiant*), const char *critere) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Aucun etudiant enregistre.\n");
        return;
    }
    
    // Compter les étudiants
    int n = nombreEtudiant(nomFichier);
    if (n == 0) {
        printf("Aucun etudiant a trier.\n");
        fclose(fichier);
        return;
    }
    
    // Allouer et lire les étudiants
    Etudiant *etudiants = malloc(n * sizeof(Etudiant));
    if (etudiants == NULL) {
        fclose(fichier);
        return;
    }
    
    rewind(fichier);
    for (int i = 0; i < n; i++) {
        etudiants[i] = lireEtudiant(fichier);
    }
    fclose(fichier);
    
    // Trier avec tri à bulles
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (comparer(&etudiants[j], &etudiants[j + 1]) > 0) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
    
    // Afficher la liste triée
    printf("\n=== LISTE TRIEE PAR %s ===\n", critere);
    printf("Matricule\tNom\tPrenom\t");
    if (strcmp(critere, "REGION") == 0) {
        printf("Region\tFiliere\n");
        printf("--------------------------------------------------------\n");
        for (int i = 0; i < n; i++) {
            printf("%s\t%s\t%s\t%s\t%s\t%c\n",
                   etudiants[i].matricule,
                   etudiants[i].nom,
                   etudiants[i].prenom,
                   etudiants[i].filiere,
                   etudiants[i].region,
                   etudiants[i].sexe);
        }
    } else if (strcmp(critere, "FILIERE") == 0) {
        printf("Filiere\tRegion\n");
        printf("--------------------------------------------------------\n");
        for (int i = 0; i < n; i++) {
            printf("%s\t%s\t%s\t%s\t%s\t%c\n",
                   etudiants[i].matricule,
                   etudiants[i].nom,
                   etudiants[i].prenom,
                   etudiants[i].filiere,
                   etudiants[i].region,
                   etudiants[i].sexe);
        }
    } else {
        printf("Filiere\tRegion\tSexe\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < n; i++) {
            printf("%s\t%s\t%s\t%s\t%s\t%c\n",
                   etudiants[i].matricule,
                   etudiants[i].nom,
                   etudiants[i].prenom,
                   etudiants[i].filiere,
                   etudiants[i].region,
                   etudiants[i].sexe);
        }
    }
    
    printf("\nTotal : %d etudiant(s)\n", n);
    
    free(etudiants);
}

void trierParNom(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParNom, "NOM");
}

void trierParMatricule(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParMatricule, "MATRICULE");
}

void trierParRegion(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParRegion, "REGION");
}

void trierParFiliere(const char *nomFichier) {
    trierEtAfficher(nomFichier, comparerParFiliere, "FILIERE");
}