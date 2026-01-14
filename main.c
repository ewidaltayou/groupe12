#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "etudiant.h"
#ifdef _WIN32
#include <windows.h>
#endif
 int main(){
    char choice;
    //entete("BIENVENU DANS LE PROGRAMME DE GESTION DES ETUDIANTS");
   ic2: do{
        //cleanConsole();
        entete(" BIENVENU DANS LE PROGRAMME DE GESTION DES ETUDIANTS ");
        menu_header("MENU DE GESTION DES ETUDIANTS");
        menu_item("1","Faire un tri de la liste des etudiants");
        menu_item("2","Rechercher un etudiant");
        menu_item("3","suppprimer un etudiant");
        menu_item("4","Modifier les donnees d'un etudiant");
        menu_item("5", "Inscrire un nouvel etudiant dans la base de donnees");
        menu_item("6","Afficher tous les etudiants");
        menu_item("7","Calculer l'age d'un etudiant");
        menu_item("8", "Quitter");
        menu_footer();
        int n=nombreEtudiant("etudiants.txt");
        printf("\n\t\tNous avons comptabilise %d etudiants dans la base de donnees.\n\n",n);
        printf("Quelle opertation aimeriez-vous effectuer ? ");
        scanf(" %c",&choice);
        cleanConsole();
    switch(choice){
        case '1':
        char c;
        do
        {
            
            //cleanConsole();
            menu_header("MENU DES TRIS");
            menu_item("1","Trier par matricule");
            menu_item("2","Trier par nom");
            menu_item("3","Trier par region");
            menu_item("4", "Trier par filiere");
            menu_item("0","Revenir au menu principal");
            menu_footer();
            printf(">> comment voulez-vous trier les etudiants? "); 
            scanf(" %c",&c);
            switch(c){
                case '1':
                    trierParMatricule("etudiants.txt");
                    break;
                case '2':
                    trierParNom("etudiants.txt");
                    break;
                case '3':
                    trierParRegion("etudiants.txt");
                    break;
                case '4':
                    trierParFiliere("etudiants.txt");
                    break;
                case '0':
                    goto ic2;
                    break;
                default:
                    alert("choix invalide");
                    break;
            }            
        } while (c != '0');
         break;
        case '2':
        char r;
        do{
            menu_header("MENU DE RECHERCHE");
            menu_item("1", "Recherche par matricule");
            menu_item("2","Recherche par nom");
            menu_item("3","Recherche par dichotomique (par matricule)");
            menu_item("0", "Revenir au menu principal");
            menu_footer();
            printf(">> Comment voulez-vous rechercher l'etudiant? ");
            scanf(" %c",&r);
            switch(r){
                case '1':
                    char mat[20];
                    printf("entrez le matricule de l'etudiant:");
                    scanf("%s",mat);
                    rechercherEtudiantParMatricule(mat,"etudiants.txt");
                    break;
                case '2':
                    char nom[20];
                    printf("entrez le nom de l'etudiant:");
                    scanf("%s",nom);
                    rechercherEtudiantParNom(nom,"etudiants.txt");
                    break;
                case '3':
                    {
                        char mat[20];
                        printf("entrez le matricule de l'etudiant:");
                        scanf("%s",mat);
                        rechercherEtudiantPardichotomique(mat,"etudiants.txt");
                        break;
                    }
                case '0':
                    goto ic2;
                    break;       
                default:
                    alert("choix invalide");
                    break;
            }
        }while(r != '0');    
            break;
        case '3':
            {
                entete("Suppression d'un etudiant");
                char mat[20];
                printf("entrez le matricule de l'etudiant a supprimer: ");
                scanf("%s",mat);
                supprimerEtudiant(mat,"etudiants.txt");
                break;
            }
        case '4':
            {
                entete("Modification des donnees d'un etudiant");
                char mat[20];
                printf("entrez le matricule de l'etudiant a modifier: ");
                scanf("%s",mat);
                viderBuffer();
                modifierEtudiant(mat,"etudiants.txt");
                break;
            }
        case '5':
            {
                entete("Enregistrement d'un etudiant");
                creerEtudiant("etudiants.txt");
                break;
            }
        case '6':
            {
                entete("Liste des etudiants");
                afficherTousLesEtudiant("etudiants.txt");
                alert("Agrandir la console si l'affichage est non coherant :) ");
                break;
            }
        case '7':
            {
                entete("calcul de l'age d'un etudiant");
                char mat[20];
                printf("entrez le matricule de l'etudiant: ");
                scanf("%s",mat);
                printf("\n");
                int age = calculAge(mat,"etudiants.txt");
                if (age != -1) {
                    printf("> L'etudiant avec le matricule %s a: %d ans\n", mat, age);
                }
                break;
            }
        case '8':
            {
                alert("Au revoir!");
                exit(0);
                break;
            }
        default:
            alert("choix invalide");
    }
  }while(choice !='8');
 }