#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "etudiant.h"
 int main(){
    int choice;
    printf("\nBIENVENU DANS LE DEVOIR DU GROUPE12\n");
    do{
    int n=nombreEtudiant("etudiants.txt");
    printf("\nLe nombre d'etudiant est: %d\n",n);
    printf("que voulez-vous faire?\n");
    printf("1- Trier les etudiants\n");
    printf("2- Rechercher un etudiant \n");
    printf("3-suppprimer un etudiant\n");
    printf("4-modifier un etudiant\n");
    printf("5-ajouter un etudiant\n");
    printf("6-afficher tous les etudiants\n");
    printf("7-calculer l'age d'un etudiant\n");
    printf("8-quitter\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            int c;
            printf("comment voulez-vous trier les etudiants?\n");
            printf("1- par matricule\n");
            printf("2- par nom\n");
            printf("3- par region\n");
            printf("4- par filiere\n"); 
            scanf("%d",&c);
            switch(c){
                case 1:
                    trierParMatricule("etudiants.txt");
                    break;
                case 2:
                    trierParNom("etudiants.txt");
                    break;
                case 3:
                    trierParRegion("etudiants.txt");
                    break;
                case 4:
                    trierParFiliere("etudiants.txt");
                    break;
                default:
                    printf("choix invalide\n");
                    break;
            }
            break;
        case 2:
            printf("comment voulez-vous rechercher l'etudiant?\n");
            printf("1- par matricule\n");
            printf("2- par nom\n");
            printf("3-par dichotomique (par matricule)\n");
            int r;
            scanf("%d",&r);
            switch(r){
                case 1:
                    char mat[20];
                    printf("entrez le matricule de l'etudiant:");
                    scanf("%s",mat);
                    rechercherEtudiantParMatricule(mat,"etudiants.txt");
                    break;
                case 2:
                    char nom[20];
                    printf("entrez le nom de l'etudiant:");
                    scanf("%s",nom);
                    rechercherEtudiantParNom(nom,"etudiants.txt");
                    break;
                case 3:
                    {
                        char mat[20];
                        printf("entrez le matricule de l'etudiant:");
                        scanf("%s",mat);
                        rechercherEtudiantPardichotomique(mat,"etudiants.txt");
                        break;
                    }   
                default:
                    printf("choix invalide\n");
                    break;
            }
            break;
        case 3:
            {
                char mat[20];
                printf("entrez le matricule de l'etudiant a supprimer:");
                scanf("%s",mat);
                supprimerEtudiant(mat,"etudiants.txt");
                break;
            }
        case 4:
            {
                char mat[20];
                printf("entrez le matricule de l'etudiant a modifier:");
                scanf("%s",mat);
                modifierEtudiant(mat,"etudiants.txt");
                break;
            }
        case 5:
            {
                creerEtudiant("etudiants.txt");
                break;
            }
        case 6:
            {
                afficherTousLesEtudiant("etudiants.txt");
                break;
            }
        case 7:
            {
                char mat[20];
                printf("entrez le matricule de l'etudiant:");
                scanf("%s",mat);
                int age = calculAge(mat,"etudiants.txt");
                if (age != -1) {
                    printf("L'age de l'etudiant avec le matricule %s a: %d ans\n", mat, age);
                }
                break;
            }
        case 8:
            {
                printf("Au revoir!\n");
                exit(0);
                break;
            }
        default:
            printf("choix invalide\n");
    }
  }while(choice !=8);
 }