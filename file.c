#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "file.h"


int trouverRoleParId(int idRecherche) {
    FILE *fichier = fopen("employes.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier employes.txt\n");
        return -1; // Échec
    }

    char ligne[256];
    int idDansFichier, roleDansFichier;

    // Lit le fichier ligne par ligne
    while ((fgets(ligne, sizeof(ligne), fichier))) {
        // sscanf pour extraire uniquement l'ID et le rôle.
        // %*[^;] signifie "lire et ignorer une chaîne de caractères jusqu'au prochain ';'"
        int itemsLus = sscanf(ligne, "%d;%*[^;];%*[^;];%*[^;];%*[^;];%d;",
                              &idDansFichier, &roleDansFichier); // on stock seulement id et role (* pour ne stocke pas )

        // On s'assure qu'on a bien lu l'ID et le rôle
        if (itemsLus == 2 && idDansFichier == idRecherche) {
            fclose(fichier);
            return roleDansFichier; // Succès : on retourne le rôle trouvé
        }
    }

    // L'employé n'a pas été trouvé après avoir lu tout le fichier
    fclose(fichier);
    return -1; // Échec
}

void chargerTousLesEmployes(Employe **listeEmployes, int *nbEmployes) {
    FILE *fichier = fopen("employes.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir employes.txt\n");
        *nbEmployes = 0;
        *listeEmployes = NULL;
        return;
    }

    char ligne[256];
    *nbEmployes = 0;
    
    // Compter les lignes non-vides
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] != '\n' && ligne[0] != '\0') {
            (*nbEmployes)++;
        }
    }

    if (*nbEmployes == 0) {
        printf("Fichier employes.txt vide.\n");
        fclose(fichier);
        return;
    }

    // Allouer la mémoire
    *listeEmployes = (Employe*)malloc(*nbEmployes * sizeof(Employe));
    if (*listeEmployes == NULL) {
        printf("Erreur : Allocation mémoire échouée\n");
        fclose(fichier);
        return;
    }

    // Revenir au début et remplir le tableau
    rewind(fichier);
    int index = 0;
    while (fgets(ligne, sizeof(ligne), fichier) && index < *nbEmployes) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        int resultat = sscanf(ligne, "%d;%49[^;];%49[^;];%99[^;];%49[^;];%d",
                   &(*listeEmployes)[index].ID, 
                   (*listeEmployes)[index].nom, 
                   (*listeEmployes)[index].prenom,
                   (*listeEmployes)[index].email, 
                   (*listeEmployes)[index].Departement, 
                   &(*listeEmployes)[index].Role);
        
        if (resultat == 6) {
            index++;
        }
    }
    
    *nbEmployes = index;
    fclose(fichier);
    printf("Fichier employes.txt chargé. %d employés trouvés.\n", *nbEmployes);
}

void chargerTousLesSoldes(SoldeConge **listeSoldes, int *nbSoldes) {
    FILE *fichier = fopen("soldes.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir soldes.txt\n");
        *nbSoldes = 0;
        *listeSoldes = NULL;
        return;
    }

    char ligne[128];
    *nbSoldes = 0;
    
    // Compter les lignes non-vides
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] != '\n' && ligne[0] != '\0') {
            (*nbSoldes)++;
        }
    }

    if (*nbSoldes == 0) {
        printf("Fichier soldes.txt vide.\n");
        fclose(fichier);
        return;
    }

    *listeSoldes = (SoldeConge*)malloc(*nbSoldes * sizeof(SoldeConge));
    if (*listeSoldes == NULL) {
        printf("Erreur : Allocation mémoire échouée pour les soldes\n");
        fclose(fichier);
        return;
    }

    rewind(fichier);
    int index = 0;
    while (fgets(ligne, sizeof(ligne), fichier) && index < *nbSoldes) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        char typeCongeStr[20];
        int resultat = sscanf(ligne, "%d;%19[^;];%d",
               &(*listeSoldes)[index].idEmp, 
               typeCongeStr,
               &(*listeSoldes)[index].joursRestants);
        
        if (resultat == 3) {
            // Convertir le type de congé (Annuel/Maladie)
            if (strcmp(typeCongeStr, "Annuel") == 0) {
                (*listeSoldes)[index].typeConge = 0;
            } else if (strcmp(typeCongeStr, "Maladie") == 0) {
                (*listeSoldes)[index].typeConge = 1;
            } else {
                (*listeSoldes)[index].typeConge = 0;
            }
            index++;
        }
    }
    
    *nbSoldes = index;
    fclose(fichier);
    printf("Fichier soldes.txt chargé. %d soldes trouvés.\n", *nbSoldes);
}

void chargerTousLesConges(Conge **listeConges, int *nbConges) {
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir conges.txt\n");
        *nbConges = 0;
        *listeConges = NULL;
        return;
    }

    char ligne[512];
    *nbConges = 0;
    
    // Compter les lignes non-vides
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] != '\n' && ligne[0] != '\0') {
            (*nbConges)++;
        }
    }

    if (*nbConges == 0) {
        printf("Fichier conges.txt vide.\n");
        fclose(fichier);
        *listeConges = NULL;
        return;
    }

    *listeConges = (Conge*)malloc(*nbConges * sizeof(Conge));
    if (*listeConges == NULL) {
        printf("Erreur : Allocation mémoire échouée pour les congés\n");
        fclose(fichier);
        return;
    }

    rewind(fichier);
    int index = 0;
    while (fgets(ligne, sizeof(ligne), fichier) && index < *nbConges) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        int resultat = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%199[^\n]",
               &(*listeConges)[index].id, 
               &(*listeConges)[index].idEmploye,
               &(*listeConges)[index].dateDebut.jour, 
               &(*listeConges)[index].dateDebut.mois, 
               &(*listeConges)[index].dateDebut.annee,
               &(*listeConges)[index].dateFin.jour, 
               &(*listeConges)[index].dateFin.mois, 
               &(*listeConges)[index].dateFin.annee,
               &(*listeConges)[index].type, 
               &(*listeConges)[index].status,
               (*listeConges)[index].motif);
        
        if (resultat == 11) {
            index++;
        }
    }
    
    *nbConges = index;
    fclose(fichier);
    printf("Fichier conges.txt chargé. %d congés trouvés.\n", *nbConges);
}

