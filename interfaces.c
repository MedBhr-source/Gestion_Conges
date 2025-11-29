#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"interfaces.h"
#include"structs.h"


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
}

void voirHistoriqueDemandes(int idUtilisateur){
    printf("\n\t=== HISTORIQUE DES DEMANDES ===\n");
    printf("--------------------------\n");
    
    int trouveDemande = 0;
    for(int i = 0; i < nbConges; i++){
        if(conges[i].idEmploye == idUtilisateur){
            printf("\nDemande #%d:\n", conges[i].id);
            printf("  Du: %02d/%02d/%04d au %02d/%02d/%04d\n", 
                   conges[i].dateDebut.jour, conges[i].dateDebut.mois, conges[i].dateDebut.annee,
                   conges[i].dateFin.jour, conges[i].dateFin.mois, conges[i].dateFin.annee);
            printf("  Type: %s\n", conges[i].type == 0 ? "Annuel" : "Maladie");
            printf("  Statut: %s\n", 
                   conges[i].status == 0 ? "En attente" : (conges[i].status == 1 ? "Approuvé" : "Rejeté"));
            printf("  Motif: %s\n", conges[i].motif);
            trouveDemande = 1;
        }
    }
    
    if(!trouveDemande){
        printf("Aucune demande de congé trouvée pour cet utilisateur.\n");
    }
    printf("--------------------------\n\n");
}

// Fonctions Manager
void listerDemandesEnAttente(int idUtilisateur){
    printf("Demandes en attente pour le manager %d\n", idUtilisateur);
}

void approuverDemande(){
    printf("Approbation de demande\n");
}

void rejeterDemande(){
    printf("Rejet de demande\n");
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