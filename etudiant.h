#ifndef ETUDIANT_H
#define ETUDIANT_H
// structure de la date
typedef struct{
    int jour;
    int mois;
    int annee;
}Date;
// structure de l'etudiant
typedef struct{
   char matricule[20];
   char nom[20];
   char prenom[20];
   Date dateNaissance;
   char departement[15];
   char filiere[30];
   char region[10];
   char sexe;
}Etudiant;
// gestion de la date
int anneeCourante();
Date creerDate();
int verifierDate(Date d);
// operations de CRUD des etudiants
Etudiant creerEtudiant(const char*nomFichier);
int modifierEtudiant(const char*matricule,const char*nomFichier);
int supprimerEtudiant(const char*matricule,const char*nomFichier);
void afficherEtudint(Etudiant e);
// operations supplementaires
int calculAge(const char*matricule,const char*nomFichier);
int nombreEtudiant(const char*nomFichier);
int verifierMatricule(const char*matricule);
void afficherTousLesEtudiant(const char*nomFichier);
void genererMatricule(const char matricule[], int annee, int numero);
int prochainNumero();
//pour les differentes recherches
int rechercherEtudiantParMatricule(const char*matricule,const char*nomFichier);
int rechercherEtudiantPardichotomique(const char*matricule,const char*nomFichier);
int rechercherEtudiantParNom(const char*matricule,const char*nomFichier);
// pour les differents tri
void trierParNom(const char*nomFichier);
void trierParMatricule(const char*nomFichier);
void trierParRegion(const char*nomFichier);
void trierParFiliere(const char*nomFichier);
#endif



