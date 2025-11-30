#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"interfaces.h"
#include"structs.h"

// Déclaration de la fonction pause_screen (prototype)
void pause_screen();

void afficherMenuPrincipal(){
    printf("\tBIENVENUE\n");
    printf("--Menu Principal--87\n");
    printf("\n--------------------------\n");
    printf("1. Se connecter\n");
    printf("2. Quitter");
    printf("\n--------------------------\n");
    printf("Entrez votre choix: ");
}

void afficherMenuEmploye(){
    printf("\t--Menu Employe--\n");
    printf("\n--------------------------\n");
    printf("1. Consulter mon solde de conges\n");
    printf("2. Faire une nouvelle demande de conge\n");
    printf("3. Voir l'historique de mes demandes\n\n");
    printf("4. Se deconnecter");
    printf("\n--------------------------\n");
    printf("Entrez votre choix:\n");
}

void afficherMenuManager(){
    printf("\t--Menu Manager--\n");
    printf("\n--------------------------\n");
    printf("1. Voir les demandes en attente\n");
    printf("2. Approuver une demande \n");
    printf("3. Rejeter une demande   \n");
    printf("4. Se déconnecter\n");
    printf("\n--------------------------\n");
    printf("Entrez votre choix:\n");
}

void afficherMenuAdmin(){
    printf("\t--Menu Admin--\n");
    printf("\n--------------------------\n");
    printf("1. Gérer les employés \n");
    printf("2. Voir toutes les demandes de conge\n");
    printf("3. Mettre à jour les soldes de congés\n");
    printf("4. Se deconnecter\n");
    printf("\n--------------------------\n");
    printf("Entrez votre choix:\n");
}

// Déclarations externes des données globales
extern Employe *employes;
extern Conge *conges;
extern SoldeConge *soldes;
extern int nbEmployes;
extern int nbConges;
extern int nbSoldes;


// Fonction pour mettre en pause et attendre que l'utilisateur appuie sur ENTRER
void pause_screen(){
    int c;
    // Vider le buffer d'entrée d'abord
    while ((c = getchar()) != '\n' && c != EOF);
    
    printf("\nAppuyez sur ENTRER pour continuer...");
    fflush(stdout);
    
    // Attendre l'ENTRER
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fonction utilitaire pour convertir le type de congé en texte lisible
const char* getTypeCongeString(int type) {
    switch (type) {
        case 0: return "Annuel";
        case 1: return "Maladie";
        default: return "Inconnu";
    }
}

// La fonction principale pour afficher le solde
void afficherSoldeUtilisateur(int idUtilisateur) {
    printf("\n--- Votre Solde de Congés (ID: %d) ---\n", idUtilisateur);
    
    int trouve = 0; // Un drapeau pour savoir si on a trouvé au moins un solde

    // On parcourt tout le tableau des soldes qui est en mémoire
    for (int i = 0; i < nbSoldes; i++) {
        
        // Si le solde appartient à l'utilisateur connecté...
        if (soldes[i].idEmp == idUtilisateur) {
            
            // ...alors on l'affiche
            printf("  - Type: %-10s | Jours restants: %d\n", 
                   getTypeCongeString(soldes[i].typeConge), soldes[i].joursRestants);
            
            trouve = 1; // On a trouvé quelque chose, on lève le drapeau
        }
    }

    // Si après avoir tout parcouru, le drapeau n'a pas été levé...
    if (!trouve) {
        printf("Aucun solde de congé trouvé pour votre profil.\n");
    }
    
    printf("------------------------------------\n");
    pause_screen();
}

// logic.c

// ... (les autres fonctions)

// logic.c

// ... (les autres fonctions)
int getMaxCongeId() {
    int maxId = 0;
    for (int i = 0; i < nbConges; i++) {
        if (conges[i].id > maxId) {
            maxId = conges[i].id;
        }
    }
    return maxId;
}

void faireDemandeConge(int idUtilisateur) {
    printf("\n--- Nouvelle Demande de Congé ---\n");

    // 1. Création de la nouvelle demande en mémoire
    Conge nouvelleDemande;
    nouvelleDemande.id = getMaxCongeId() + 1;
    nouvelleDemande.idEmploye = idUtilisateur;
    nouvelleDemande.status = 0; // Le statut est 'en attente' (0)

    printf("Type de congé (0: Annuel, 1: Maladie) : ");
    scanf("%d", &nouvelleDemande.type);

    printf("Date de début (JJ MM AAAA) : ");
    scanf("%d %d %d", &nouvelleDemande.dateDebut.jour, &nouvelleDemande.dateDebut.mois, &nouvelleDemande.dateDebut.annee);

    printf("Date de fin (JJ MM AAAA) : ");
    scanf("%d %d %d", &nouvelleDemande.dateFin.jour, &nouvelleDemande.dateFin.mois, &nouvelleDemande.dateFin.annee);

    printf("Motif : ");
    scanf(" %199[^\n]", nouvelleDemande.motif);

    // 2. Ajout de la demande au tableau en mémoire (pour la session actuelle)
    // On utilise realloc pour agrandir dynamiquement le tableau
    Conge *temp = realloc(conges, (nbConges + 1) * sizeof(Conge));
    if (temp == NULL) {
        printf("Erreur : Impossible d'ajouter la demande (mémoire pleine).\n");
        return;
    }
    conges = temp;
    conges[nbConges] = nouvelleDemande;
    nbConges++;

    // 3. Sauvegarde de la demande dans le fichier conges.txt (PARTIE ESSENTIELLE)
    FILE *fichier = fopen("conges.txt", "a"); // "a" pour "append" (ajouter à la fin)
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt pour sauvegarder.\n");
        return;
    }

    // Écriture de la nouvelle ligne au bon format
    fprintf(fichier, "%d;%d;%02d;%02d;%d;%02d;%02d;%d;%d;%d;\"%s\"\n",
            nouvelleDemande.id,
            nouvelleDemande.idEmploye,
            nouvelleDemande.dateDebut.jour, nouvelleDemande.dateDebut.mois, nouvelleDemande.dateDebut.annee,
            nouvelleDemande.dateFin.jour, nouvelleDemande.dateFin.mois, nouvelleDemande.dateFin.annee,
            nouvelleDemande.type,
            nouvelleDemande.status, // C'est bien 0 pour 'en attente'
            nouvelleDemande.motif);

    fclose(fichier); // Indispensable pour valider l'écriture

    printf("\nDemande créée avec succès ! ID de votre demande : %d\n", nouvelleDemande.id);
    printf("Elle a été sauvegardée et est maintenant en attente de validation par votre manager.\n");
    printf("----------------------------------\n");
    pause_screen();
}

void voirHistoriqueDemandes(int idUtilisateur){
    printf("\n\t=== HISTORIQUE DES DEMANDES ===\n");
    printf("--------------------------\n");
    
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt\n");
        return;
    }
    
    char ligne[512];
    int trouveDemande = 0;
    
    // Lire le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        int id, idEmploye, type, status;
        int jourDebut, moisDebut, anneeDebut;
        int jourFin, moisFin, anneeFin;
        char motif[256];
        
        // Parser la ligne avec sscanf selon le format du fichier
        int resultat = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\"%255[^\"]\"",
                   &id, &idEmploye,
                   &jourDebut, &moisDebut, &anneeDebut,
                   &jourFin, &moisFin, &anneeFin,
                   &type, &status,
                   motif);
        
        if (resultat == 11 && idEmploye == idUtilisateur) {
            printf("\nDemande #%d:\n", id);
            printf("  Du: %02d/%02d/%04d au %02d/%02d/%04d\n", 
                   jourDebut, moisDebut, anneeDebut,
                   jourFin, moisFin, anneeFin);
            printf("  Type: %s\n", type == 0 ? "Annuel" : "Maladie");
            printf("  Statut: %s\n", 
                   status == 0 ? "En attente" : (status == 1 ? "Approuvé" : "Rejeté"));
            printf("  Motif: %s\n", motif);
            trouveDemande = 1;
        }
    }
    
    fclose(fichier);
    
    if(!trouveDemande){
        printf("Aucune demande de congé trouvée pour cet utilisateur.\n");
    }
    printf("--------------------------\n\n");
    pause_screen();
}

// Fonctions Manager
void listerDemandesEnAttente(int idUtilisateur){
    printf("\n\t=== DEMANDES EN ATTENTE ===\n");
    printf("--------------------------\n");
    
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt\n");
        return;
    }
    
    char ligne[512];
    int trouveDemande = 0;
    
    // Lire le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        int id, idEmploye, type, status;
        int jourDebut, moisDebut, anneeDebut;
        int jourFin, moisFin, anneeFin;
        char motif[256];
        
        // Parser la ligne avec sscanf selon le format du fichier
        int resultat = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\"%255[^\"]\"",
                   &id, &idEmploye,
                   &jourDebut, &moisDebut, &anneeDebut,
                   &jourFin, &moisFin, &anneeFin,
                   &type, &status,
                   motif);
        
        // Afficher uniquement les demandes EN ATTENTE (status == 0)
        if (resultat == 11 && status == 0) {
            printf("\nDemande #%d:\n", id);
            printf("  Employe ID: %d\n", idEmploye);
            printf("  Du: %02d/%02d/%04d au %02d/%02d/%04d\n", 
                   jourDebut, moisDebut, anneeDebut,
                   jourFin, moisFin, anneeFin);
            printf("  Type: %s\n", type == 0 ? "Annuel" : "Maladie");
            printf("  Motif: %s\n", motif);
            trouveDemande = 1;
        }
    }
    
    fclose(fichier);
    
    if(!trouveDemande){
        printf("Aucune demande en attente.\n");
    }
    printf("--------------------------\n");
    pause_screen();
}

void approuverDemande(){
    printf("\n--- Approuver une Demande ---\n");
    
    int idDemande;
    printf("Entrez l'ID de la demande à approuver: ");
    scanf("%d", &idDemande);
    
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt\n");
        return;
    }
    
    FILE *fichierTemp = fopen("conges_temp.txt", "w");
    if (fichierTemp == NULL) {
        printf("Erreur : Impossible de créer le fichier temporaire\n");
        fclose(fichier);
        return;
    }
    
    char ligne[512];
    int trouve = 0;
    
    // Lire et réécrire le fichier
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        int id;
        sscanf(ligne, "%d;", &id);
        
        if (id == idDemande) {
            // Remplacer le status (position 9) par 1 (approuvé)
            // Réinsérer la ligne avec le nouveau statut
            int idEmploye, type, status;
            int jourDebut, moisDebut, anneeDebut;
            int jourFin, moisFin, anneeFin;
            char motif[256];
            
            int resultat = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\"%255[^\"]\"",
                       &id, &idEmploye,
                       &jourDebut, &moisDebut, &anneeDebut,
                       &jourFin, &moisFin, &anneeFin,
                       &type, &status,
                       motif);
            
            if (resultat == 11) {
                fprintf(fichierTemp, "%d;%d;%02d;%02d;%d;%02d;%02d;%d;%d;%d;\"%s\"\n",
                        id, idEmploye,
                        jourDebut, moisDebut, anneeDebut,
                        jourFin, moisFin, anneeFin,
                        type, 1, motif); // 1 = Approuvé
                printf("Demande #%d approuvée avec succès!\n", idDemande);
                trouve = 1;
            }
        } else {
            fputs(ligne, fichierTemp);
        }
    }
    
    fclose(fichier);
    fclose(fichierTemp);
    
    if (trouve) {
        remove("conges.txt");
        rename("conges_temp.txt", "conges.txt");
    } else {
        remove("conges_temp.txt");
        printf("Demande #%d introuvable ou déjà traitée.\n", idDemande);
    }
    
    printf("----------------------------------\n");
    pause_screen();
}

void rejeterDemande(){
    printf("\n--- Rejeter une Demande ---\n");
    
    int idDemande;
    printf("Entrez l'ID de la demande à rejeter: ");
    scanf("%d", &idDemande);
    
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt\n");
        return;
    }
    
    FILE *fichierTemp = fopen("conges_temp.txt", "w");
    if (fichierTemp == NULL) {
        printf("Erreur : Impossible de créer le fichier temporaire\n");
        fclose(fichier);
        return;
    }
    
    char ligne[512];
    int trouve = 0;
    
    // Lire et réécrire le fichier
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        int id;
        sscanf(ligne, "%d;", &id);
        
        if (id == idDemande) {
            // Remplacer le status (position 9) par 2 (rejeté)
            int idEmploye, type, status;
            int jourDebut, moisDebut, anneeDebut;
            int jourFin, moisFin, anneeFin;
            char motif[256];
            
            int resultat = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\"%255[^\"]\"",
                       &id, &idEmploye,
                       &jourDebut, &moisDebut, &anneeDebut,
                       &jourFin, &moisFin, &anneeFin,
                       &type, &status,
                       motif);
            
            if (resultat == 11) {
                fprintf(fichierTemp, "%d;%d;%02d;%02d;%d;%02d;%02d;%d;%d;%d;\"%s\"\n",
                        id, idEmploye,
                        jourDebut, moisDebut, anneeDebut,
                        jourFin, moisFin, anneeFin,
                        type, 2, motif); // 2 = Rejeté
                printf("Demande #%d rejetée avec succès!\n", idDemande);
                trouve = 1;
            }
        } else {
            fputs(ligne, fichierTemp);
        }
    }
    
    fclose(fichier);
    fclose(fichierTemp);
    
    if (trouve) {
        remove("conges.txt");
        rename("conges_temp.txt", "conges.txt");
    } else {
        remove("conges_temp.txt");
        printf("Demande #%d introuvable ou déjà traitée.\n", idDemande);
    }
    
    printf("----------------------------------\n");
    pause_screen();
}

// Fonctions Admin
void gererEmployes(){
    printf("Gestion des employés\n");
}

void voirToutesLesDemandes(){
    printf("Affichage de toutes les demandes\n");
}

void mettreAJourSoldes(){
    printf("Mise à jour des soldes de congés\n");
}