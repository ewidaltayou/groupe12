#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Etudiant.h"

void afficherMenu() {
    printf("\n=== GESTION DES ETUDIANTS ===\n");
    printf("1. Ajouter un etudiant\n");
    printf("2. Afficher tous les etudiants\n");
    printf("3. Nombre d'etudiants\n");
    printf("4. Trier par nom\n");
    printf("5. Trier par matricule\n");
    printf("6. Trier par region\n");
    printf("7. Trier par filiere\n");
    printf("0. Quitter\n");
    printf("Choix : ");
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

int main() {
    const char *nomFichier = "etudiants.txt";
    int choix;
    char matricule[20];
    char nom[20];
    
    do {
        afficherMenu();
        scanf("%d", &choix);
        clearInputBuffer();
        
        switch (choix) {
            case 1:
                creerEtudiant(nomFichier);
                break;
                                
            case 2:
                afficherTousLesEtudiants(nomFichier);
                break;
                
            
            case 3:
                printf("Nombre d'etudiants : %d\n", nombreEtudiant(nomFichier));
                break;
                
            case 4:
                trierParNom(nomFichier);
                break;
                
            case 5:
                trierParMatricule(nomFichier);
                break;
                
            case 6:
                trierParRegion(nomFichier);
                break;
                
            case 7:
                trierParFiliere(nomFichier);
                break;
                
            case 0:
                printf("Au revoir !\n");
                break;
                
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }
        
        if (choix != 0) {
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();
        }
        
    } while (choix != 0);
    
    return 0;
}