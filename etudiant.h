#ifndef ETUDIANT_H
#define ETUDIANT_H
// structure de la date
typedef struct{
    int jour;
    int mois;
    int annee;
}Date;

// structure de donnees pour representer un etudiant
typedef struct{
   char matricule[20];
   char nom[20];
   char prenom[20];
   Date dateNaissance;
   char departement[20];
   char filiere[30];
   char region[20];
   char sexe;
   
}Etudiant;

// gestion de la date

//fonction pour creer une date
void  saisirDate(Date *d);
// int dateValide(Date*d);

Date dateActuelle();
//declation d'une fonction pour les messages d'alerte
void alert(char* message);
//declaration de la fonction pour formater les entetes
void entete(char* message);
// operations de CRUD des etudiants
void saisirEtudiant();
//Etudiant* creerEtudiant(const char*nomFichier);
int modifierEtudiant(const char*matricule,const char*nomFichier);
int supprimerEtudiant(const char*matricule,const char*nomFichier);
void afficherEtudiant(Etudiant*e);
// operations supplementaires
int calculAge(const char*matricule);


int nombreEtudiant(const char*nomFichier);

//int verifierMatriclule(const char*matricule);
// declaration d'une fonction pour generer automatiquement les matricules a partir d'un fichier index.txt
char *generate_matricule(char *structure, int index);

void afficherTousLesEtudiant(const char*nomFichier);
//pour les differentes recherches
int rechercherEtudiantParMatricule(const char*matricule,const char*nomFichier);
int rechercherEtudiantPardichotomique(const char*matricule,const char*nomFichier);
int rechercherEtudiantParNom(const char*matricule,const char*nomFichier);
// pour les differents tri
void tierParNom(const char*nomFichier);
void trierParMatricule(const char*nomFichier);
void trierParRegion(const char*nomFichier);
#endif



