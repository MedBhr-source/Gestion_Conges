#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "interfaces.h"
#include "file.h"
//gcc -g main.c file.c interfaces.c -o gestion_conges
Employe *employes = NULL;      
Conge *conges = NULL;          
SoldeConge *soldes = NULL;     

// Compteurs pour savoir combien d'éléments il y a dans chaque tableau
int nbEmployes = 0;
int nbConges = 0;
int nbSoldes = 0;

void gererSession( int idUtilisateur ,int roleUtilisateur) {
    int choixMenu;
    do {
        switch (roleUtilisateur) {
            case 0: // Employé
                afficherMenuEmploye();
                break;
            case 1: // Manager
                afficherMenuManager();
                break;
            case 2: // Admin
                afficherMenuAdmin();
                break;
        }
        scanf("%d", &choixMenu);

        if (choixMenu > 0) {
           printf("Action choisie : %d \n", choixMenu);
        }

        // On dispatche l'action vers la bonne fonction dans logic.c
        if (roleUtilisateur == 0) { // Employé
            switch (choixMenu) {
                case 1: afficherSoldeUtilisateur(idUtilisateur); break;
                case 2: faireDemandeConge(idUtilisateur); break;
                case 3: voirHistoriqueDemandes(idUtilisateur); break;
            }
        } else if (roleUtilisateur == 1) { // Manager
            switch (choixMenu) {
                case 1: listerDemandesEnAttente(idUtilisateur); break;
                case 2: approuverDemande(); break;
                case 3: rejeterDemande(); break;
            }
        } else if (roleUtilisateur == 2) { // Admin
            switch (choixMenu) {
                case 1: gererEmployes(); break;
                case 2: voirToutesLesDemandes(); break;
                case 3: mettreAJourSoldes(); break;
                case 4: afficherStatistiques(); break;
            }
        }

    } while (choixMenu !=0);
}


int main(){

    chargerTousLesEmployes(&employes, &nbEmployes);
    chargerTousLesConges(&conges, &nbConges);
    chargerTousLesSoldes(&soldes, &nbSoldes);   


    int choix;
    while(1){
        afficherMenuPrincipal();
        scanf("%d",&choix);

        switch (choix) {
            case 1: {
            int IdConnexion;
            printf("donner votre ID :");
            scanf("%d",&IdConnexion);
            int RoleUtilisateur = trouverRoleParId(IdConnexion);

            if (RoleUtilisateur != -1) { // -1 signifie "non trouvé"
                    system("cls");
                    printf("\nConnexion reussie !\n");
                    
                    // On appelle la fonction de session avec l'ID et le rôle
                    gererSession(IdConnexion, RoleUtilisateur);
                    
                    system("cls");
                    printf("Deconnexion reussie.\n");
                } else {
                    printf("\nID invalide. Veuillez reessayer.\n");
                }
                break;
            }
            case 2 :
                printf("Au revoir !\n");
                return 0;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
       }
    }

    return 0;
}