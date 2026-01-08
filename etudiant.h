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
   char matricule[19];
   char nom[30];
   char prenom[30];
   Date dateNaissance;
   char departement[30];
   char filiere[30];
   char region[30];
   char sexe;
}Etudiant;
// gestion de la date
int anneeCourante();
Date creerDate();
int verifierDate(Date d);
void viderBuffer();
// operations de CRUD des etudiants
Etudiant creerEtudiant(const char*nomFichier);
int modifierEtudiant(const char*matricule,const char*nomFichier);
int supprimerEtudiant(const char*matricule,const char*nomFichier);
void afficherEtudint(Etudiant e);
// operations supplementaires
int videChamps(Etudiant e);
int verifierRegion(const char*region);
int calculAge(const char*matricule,const char*nomFichier);
int nombreEtudiant(const char*nomFichier);
int verifierMatricule(const char*matricule);
void afficherTousLesEtudiant(const char*nomFichier);
void genererMatricule( char matricule[], int annee, int numero);
int prochainNumero();
//pour les differentes recherches
int rechercherEtudiantParMatricule(const char*matricule,const char*nomFichier);
int rechercherEtudiantPardichotomique(const char*matricule,const char*nomFichier);
int rechercherEtudiantParNom(const char*nom,const char*nomFichier);
// pour les differents tri
void trierParNom(const char*nomFichier);
void trierParMatricule(const char*nomFichier);
void trierParRegion(const char*nomFichier);
void trierParFiliere(const char*nomFichier);
//fonctions statistiques
void nombreEtudiantsParRegion(const char*nomFichier);
void nomreEtudiantsParFiliere(const char*nomFichier);
#endif 
