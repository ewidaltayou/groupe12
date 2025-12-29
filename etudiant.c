#include <stdio.h>
#include <time.h>
#include "etudiant.h"
#include <stdlib.h>
#include <string.h>
//fichier de stockage des etudiants
//fonction pour creer une date conforme
Date creerDate()
{
    Date d;
    int valide;

    do
    {
        valide = 1;

        printf("veuillez entrer convenablement la date dans le cas contraire vous serrez contraint de recommencer \n");

       char buf[20];

        printf("\tJour : ");
        fgets(buf, sizeof(buf), stdin);
        d.jour = atoi(buf);
        printf("\tMois : ");
        fgets(buf, sizeof(buf), stdin);
        d.mois = atoi(buf);
        printf("\tAnnee : ");
        fgets(buf, sizeof(buf), stdin);
        d.annee = atoi(buf);
        if (d.annee <= 0)
        {
            printf("L'annee doit etre strictement positive\n");
            valide = 0;
        }

        /* Vérification du mois */
        if (d.mois < 1 || d.mois > 12)
        {
            printf("Le mois doit etre compris entre 1 et 12\n");
            valide = 0;
        }

        /* Vérification du jour selon le mois */
        if (valide)
        {
            if (d.jour < 1)
            {
                printf("Le jour doit etre superieur ou egal a 1\n");
                valide = 0;
            }
            else
            {
                switch (d.mois)
                {
                    case 1: case 3: case 5: case 7:
                    case 8: case 10: case 12:
                        if (d.jour > 31)
                        {
                            printf("Ce mois comporte 31 jours maximum\n");
                            valide = 0;
                        }
                        break;

                    case 4: case 6: case 9: case 11:
                        if (d.jour > 30)
                        {
                            printf("Ce mois comporte 30 jours maximum\n");
                            valide = 0;
                        }
                        break;

                    case 2://cas du mois de fevrier on a les annees bissectile et les annees ordinaires
                        // Annnee bissectile
                        if ((d.annee % 400 == 0) ||
                            (d.annee % 4 == 0 && d.annee % 100 != 0))
                        {
                            if (d.jour > 29)
                            {
                                printf("Fevrier ne comporte pas plus de 29 jours cette annee\n");
                                valide = 0;
                            }
                        }
                        else
                        {
                            if (d.jour > 28)
                            {
                                printf("Fevrier ne comporte pas plus de 28 jours cette annee\n");
                                valide = 0;
                            }
                        }
                        break;
                }
            }
        }

        printf("\n");

    } while (!valide);

    return d;
}
//fonction pour retourner la date actuelle
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
//definition de la fonction pour gerer automatiquement le matricule
char *generate_matricule(const char *etablissement, int index) {
    char *code = malloc(30);
    if (!code)
    {
      printf("Erreur d'allocation\n");
      exit(1);
    }
    Date dateActu = dateActuelle();
    int chriffresAnnee = dateActu.annee % 100;//deux derniers chiffres qui compose l'annee
    snprintf(code,30, "%d%s%04d",chriffresAnnee,etablissement,index);
    return code;
}

//enregistrement d'un etudiant dans le fichier
void enregistrerEtudiant(const char *nomFichier,Etudiant student)
{
    FILE *f = fopen(nomFichier, "a");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }
    fprintf(f,"%s\t%s\t%s\t%02d/%02d/%04d\t%s\t%s\t%s\t%c\n",student.matricule,student.nom,student.prenom,student.dateNaissance.jour,student.dateNaissance.mois,student.dateNaissance.annee,student.departement,student.filiere,student.region,student.sexe);
    fclose(f);
}

//fonction pour creer un etudiant en l'ajoutant au fichier par la fonction enregistrerEtudiant
Etudiant creerEtudiant(const char *nomFichier) 
{
    //printf("\tENREGISTREMENT D'UN ETUDIANT\n\n");
    FILE *indexFile = fopen("index.txt", "r");
    int i = 0;
    Etudiant student;
    if (indexFile != NULL) {
        fscanf(indexFile, "%d", &i);
        fclose(indexFile);
    }
    
    printf("Entrer les informations de l'etudiant a enregistrer:\n");

    printf("Nom : ");
    fgets(student.nom, sizeof(student.nom), stdin);
    student.nom[strcspn(student.nom, "\n")] = '\0';
    printf("Prenom : ");
    fgets(student.prenom, sizeof(student.prenom), stdin);
    student.prenom[strcspn(student.prenom, "\n")] = '\0';

    Date present = dateActuelle();
    do
    {
        printf("Date de naissance :\n");
        student.dateNaissance = creerDate();
        //char dateChaine[11];
        //sprintf(dateChaine,"%02d/%02d/%04d",student.dateNaissance.jour,student.dateNaissance.mois,student.dateNaissance.annee);
    } while (student.dateNaissance.annee > present.annee);
    printf("Departement : ");
    fgets(student.departement, sizeof(student.departement), stdin);
    student.departement[strcspn(student.departement, "\n")] = '\0';

    printf("Filiere : ");
    fgets(student.filiere, sizeof(student.filiere), stdin);
    student.filiere[strcspn(student.filiere, "\n")] = '\0';

    printf("Region : ");
    fgets(student.region, sizeof(student.region), stdin);
    student.region[strcspn(student.region, "\n")] = '\0';
    do
    {
        printf("Quel est le sexe ? (M/F)");
        scanf(" %c",&student.sexe);       
    } while ((student.sexe != 'm' && student.sexe != 'M') && (student.sexe != 'f' && student.sexe!='F'));
    char *matricule = generate_matricule("ENSPM", i);
    strncpy(student.matricule, matricule, sizeof(student.matricule)-1);
    student.matricule[sizeof(student.matricule)-1] = '\0';
    free(matricule);

    enregistrerEtudiant(nomFichier,student);
    printf("Un etudiant vient d'etre ajoute avec succes.\n");
    FILE *increment = fopen("index.txt", "w");
    fprintf(increment, "%d", i + 1);
    fclose(increment);
    return student;
}
// fonction pour afficher les donnees d'un etudiant
void afficherEtudiant(Etudiant e)
{
    printf("Matricule: %s\nNom: %s\nprenom: %s\nDate de Naissance: %02d/%02d/%04d\nDepartement: %s\nFiliere: %s\nRegion:%s\nSexe: %c\n",e.matricule,e.nom,e.prenom,e.dateNaissance.jour,e.dateNaissance.mois,e.dateNaissance.annee,e.departement,e.filiere,e.region,e.sexe);
}
//fonction pour afficher la liste des etudiants dans la console
void afficherTousLesEtudiant(const char *nomFichier) 
{
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        printf("Erreur d'ouverture ou aucun etudiant present dans la base de donnees.\n");
        return;
    }
    const int tailleBuffer = 100;
    char buffer[tailleBuffer], matricule[tailleBuffer], nom[tailleBuffer], prenom[tailleBuffer], departement[tailleBuffer], filiere[tailleBuffer],region[tailleBuffer];
    int jj , mm ,aaaa;
    Etudiant e;
    int i =1;

    printf("\n----------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-12s | %-20s | %-15s | %-20s | %-15s | %-15s | %-15s | %-5s |\n",
           "MATRICULE", "NOM", "PRENOM", "DATE DE NAISSANCE", "DEPARTEMENT", "FILIERE", "REGION", "SEXE");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
    while (fgets(buffer,tailleBuffer,f))
    {
        strcpy(matricule,strtok(buffer,"\t"));
        strcpy(nom,strtok(NULL,"\t"));
        strcpy(prenom,strtok(NULL,"\t"));
        jj = atoi(strtok(NULL,"/"));
        mm = atoi(strtok(NULL,"/"));
        aaaa = atoi(strtok(NULL,"/\t"));
        strcpy(departement, strtok(NULL,"\t"));
        strcpy(filiere, strtok(NULL,"\t"));
        strcpy(region,strtok(NULL,"\t"));
        char *tokenSexe = strtok(NULL, "\t\n");
        char sexe = tokenSexe[0];

        printf("| %-12s | %-20s | %-15s | %02d/%02d/%04d           | %-15s | %-15s | %-15s | %-5c |\n",
        matricule,
        nom,
        prenom,
        jj,
        mm,
        aaaa,
        departement,
        filiere,
        region,
        sexe);
        printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
        i++;
    }
        
    fclose(f);
}
