#include <stdio.h>
#include <stdlib.h>
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
        return;
    }

    // 1. Compter le nombre de lignes pour savoir la taille du tableau
    char ligne[256];
    *nbEmployes = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        (*nbEmployes)++;
    }

    // 2. Allouer la mémoire pour le tableau
    *listeEmployes = (Employe*)malloc(*nbEmployes * sizeof(Employe));
    if (*listeEmployes == NULL) {
        printf("Erreur : Allocation mémoire échouée\n");
        fclose(fichier);
        return;
    }

    // 3. Revenir au début du fichier et le lire pour remplir le tableau
    rewind(fichier);
    for (int i = 0; i < *nbEmployes; i++) {
        fgets(ligne, sizeof(ligne), fichier);
        sscanf(ligne, "%d;%49[^;];%49[^;];%99[^;];%49[^;];%d",
               &(*listeEmployes)[i].ID, (*listeEmployes)[i].nom, (*listeEmployes)[i].prenom,
               (*listeEmployes)[i].email, (*listeEmployes)[i].Departement, &(*listeEmployes)[i].Role);
    }

    fclose(fichier);
    printf("Fichier employes.txt chargé. %d employés trouvés.\n", *nbEmployes);
}

void chargerTousLesSoldes(SoldeConge **listeSoldes, int *nbSoldes) {
    FILE *fichier = fopen("soldes.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir soldes.txt\n");
        return;
    }

    char ligne[128];
    *nbSoldes = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        (*nbSoldes)++;
    }

    *listeSoldes = (SoldeConge*)malloc(*nbSoldes * sizeof(SoldeConge));
    if (*listeSoldes == NULL) {
        printf("Erreur : Allocation mémoire échouée pour les soldes\n");
        fclose(fichier);
        return;
    }

    rewind(fichier);
    for (int i = 0; i < *nbSoldes; i++) {
        fgets(ligne, sizeof(ligne), fichier);
        sscanf(ligne, "%d;%d;%d",
               &(*listeSoldes)[i].idEmp, &(*listeSoldes)[i].typeConge, &(*listeSoldes)[i].joursRestants);
    }

    fclose(fichier);
    printf("Fichier soldes.txt chargé. %d soldes trouvés.\n", *nbSoldes);
}

void chargerTousLesConges(Conge **listeConges, int *nbConges) {
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir conges.txt\n");
        return;
    }

    char ligne[512];
    *nbConges = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        (*nbConges)++;
    }

    *listeConges = (Conge*)malloc(*nbConges * sizeof(Conge));
    if (*listeConges == NULL) {
        printf("Erreur : Allocation mémoire échouée pour les congés\n");
        fclose(fichier);
        return;
    }

    rewind(fichier);
    for (int i = 0; i < *nbConges; i++) {
        fgets(ligne, sizeof(ligne), fichier);
        sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%199[^\n]",
               &(*listeConges)[i].id, &(*listeConges)[i].idEmploye,
               &(*listeConges)[i].dateDebut.jour, &(*listeConges)[i].dateDebut.mois, &(*listeConges)[i].dateDebut.annee,
               &(*listeConges)[i].dateFin.jour, &(*listeConges)[i].dateFin.mois, &(*listeConges)[i].dateFin.annee,
               &(*listeConges)[i].type, &(*listeConges)[i].status,
               (*listeConges)[i].motif);
    }

    fclose(fichier);
    printf("Fichier conges.txt chargé. %d congés trouvés.\n", *nbConges);
}

