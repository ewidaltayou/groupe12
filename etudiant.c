#include<stdio.h>
#include<stdlib.h>
#include"etudiant.h"

int rechercherEtudiantParMatricule(const char *matricule, const char *nomFichier){
    FILE *f = fopen(nomFichier, "r");

    if(!f){
        printf("Erreur lors de l'ouverture du fichier\n");
        return -1; //Erreur d'ouverture du fichier
    }

    if (nombreEtudiant(nomFichier) == 0){
        printf("Aucun étudiant dans le fichier\n");
        return -2; //Fichier vide
    }

    //On s'assure que le matricule est valide et que le fichier s'ouvre correctement
    if(!verifierMatricule(matricule)){
        printf("Matricule invalide\n");
        return -3; //Matricule invalide
    }

    Etudiant e;
    char ligne[256];
    int etudiant_trouver = 0 ; 

    while(fgets(ligne, sizeof(ligne), f)){
        int champslus = sscanf(ligne, "%s\t%s\t%s\t%f\t%s\t%s\t%s\t%c", e.matricule, e.nom, e.prenom, &e.dateNaissance, e.departement, e.filiere, e.region, &e.sexe);
        if(champslus == 8){
            if(strcmp(e.matricule, matricule) == 0){
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

    else if(!verifierMatricule(matricule)){
        printf("Matricule invalide\n");     //On s'assure que le matricule est valide et que le fichier s'ouvre correctement
        return -3; //Matricule invalide
    }
    trierparMatricule(nomFichier); //On trie le fichier par matricule avant de faire la recherche dichotomique
 
    Etudiant *etudiants = NULL;
    int count = 0;
    char ligne[256];

    rewind(f);
    while(fgets(ligne, sizeof(ligne), f)){
        Etudiant e;
        if(sscanf(ligne, "%s\t%s\t%s\t%f\t%s\t%s\t%s\t%c", e.matricule, e.nom, e.prenom, &e.dateNaissance, e.departement, e.filiere, e.region, &e.sexe) == 8){
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
        int comparaison = strcmp(etudiants[milieu].matricule, matricule);

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