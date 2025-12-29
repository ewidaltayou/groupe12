# Gestion des Étudiants
Ce projet en C permet de gérer les informations d'étudiants dans un fichier texte. Il inclut des fonctionnalités pour créer, lire, modifier, supprimer et trier des étudiants, ainsi que des recherches et calculs associés.
## Fonctionnalités
- **CRUD sur les étudiants** :
  - Ajouter un étudiant
  - Modifier les informations d'un étudiant
  - Supprimer un étudiant
  - Afficher les informations d'un étudiant ou de tous les étudiants
- **Gestion des dates** :
  - Vérification de la validité d'une date
  - Calcul de l'âge à partir de la date de naissance
- **Recherches** :
  - Recherche par matricule (linéaire et dichotomique)
  - Recherche par nom
- **Tri des étudiants** :
  - Tri par nom
  - Tri par matricule
  - Tri par région
  - Tri par filière
- **Gestion automatique des matricules**
  - Génération automatique du matricule suivant
## Structure du projet
- `main.c` : fichier principal pour tester les différentes fonctionnalités.
- `etudiant.c` : implémentation des fonctions de gestion des étudiants.
- `etudiant.h` : définition des structures et prototypes des fonctions.
- `etudiants.txt` : fichier de stockage des étudiants (créé dynamiquement).
## Compilation et exécution
1. Ouvrir un terminal dans le dossier du projet.
2. Compiler les fichiers C :

```bash
gcc main.c etudiant.c -o gestion_etudiants
