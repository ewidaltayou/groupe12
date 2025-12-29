#include<stdio.h>
#include<stdlib.h>
#include"etudiant.h"

int rechercherEtudiantParMatricule(const char *matricule, const char *nomFichier){
    File *f = fopen(nomFichier, "rb");
    //On s'assure que le matricule est valide et que le fichier s'ouvre correctement
    if(!verifierMatricule(matricule)){
        printf("Matricule invalide\n");
        return -3; //Matricule invalide
    }
    else if(!f){
        printf("Erreur lors de l'ouverture du fichier\n");
        return -2;
    }
    Etudiant e;
    while(fread(&e, sizeof(Etudiant), 1, f)){
        if(strcmp(e.matricule, matricule) == 0){
            fclose(f);
            afficherEtudiant(e);
            return 0; //On retourne l'étudiant trouver
        }
    }
    fclose(f);
    printf("L'étudiant n'a pas été trouver");
    return -1; //Etudiant non trouvé
}


int rechercherEtudiantPardichotomique(const char *matricule, const char *nomFichier){
    File *f = fopen(nomFichier, "rb");
    trierparMatricule(nomFichier); //On trie le fichier par matricule avant de faire la recherche dichotomique

    if(!verifierMatricule(matricule)){
        printf("Matricule invalide\n");     //On s'assure que le matricule est valide et que le fichier s'ouvre correctement
        return -3; //Matricule invalide
    }
    
    else if(!f){
        printf("Erreur lors de l'ouverture du fichier\n");
        return -2;
    }
    //Compter le nombre d'étudiant dans le fichier
    fseek(f, 0, SEEK_END);
    long taille = ftell(f);
    int count = taille / sizeof(Etudiant);
    fseek(f, 0, SEEK_SET);
    
    int gauche = 0, droite = count - 1;
    Etudiant e;
    
    while(gauche <= droite){
        int milieu = (gauche + droite) / 2;
        fseek(f, milieu * sizeof(Etudiant), SEEK_SET);
        fread(&e, sizeof(Etudiant), 1, f);
        
        int comparaison = strcmp(e.matricule, matricule);
        if(comparaison == 0){
            fclose(f);
            afficherEtudiant(e);
            return 0; //On retourne l'étudiant trouvé
        } else if(comparaison < 0){
            gauche = milieu + 1;
        } else {
            droite = milieu - 1;
        }
    }
    
    fclose(f);
    printf("L'étudiant n'a pas été trouver");
    return -1; //Etudiant non trouvé
}