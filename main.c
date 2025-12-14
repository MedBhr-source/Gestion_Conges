#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "structs.h"
#include "interfaces.h"
#include "file.h"

Employe *employes = NULL;      
Conge *conges = NULL;          
SoldeConge *soldes = NULL;     

// Compteurs pour savoir combien d'éléments il y a dans chaque tableau
int nbEmployes = 0;
int nbConges = 0;
int nbSoldes = 0;

// Fonction pour afficher un ecran de connexion stylise
int afficherEcranConnexion() {
    system("cls");
    hideCursor();
    
    setColor(9); // Bleu
    printf("\n\n");
    printf("    ////////////////////////////////////////////////////\n");
    printf("    /                                                  /\n");
    printf("    /          SYSTEME DE GESTION DES CONGES           /\n");
    printf("    /                                                  /\n");
    printf("    ////////////////////////////////////////////////////\n");
    printf("    /                                                  /\n");
    printf("    /               CONNEXION UTILISATEUR              /\n");
    printf("    /                                                  /\n");
    printf("    ////////////////////////////////////////////////////\n");
    
    setColor(7); // Normal
    showCursor();
    
    printf("\n\n    Entrez votre ID: ");
    
    int id;
    scanf("%d", &id);
    return id;
}

void gererSession(int idUtilisateur, int roleUtilisateur) {
    int choixMenu;
    int maxChoix = (roleUtilisateur == 2) ? 5 : 4; // Admin a 5 options
    
    do {
        switch (roleUtilisateur) {
            case 0: // Employe
                choixMenu = afficherMenuEmploye();
                break;
            case 1: // Manager
                choixMenu = afficherMenuManager();
                break;
            case 2: // Admin
                choixMenu = afficherMenuAdmin();
                break;
        }

        // On dispatche l'action vers la bonne fonction
        if (roleUtilisateur == 0) { // Employe
            switch (choixMenu) {
                case 1: afficherSoldeUtilisateur(idUtilisateur); break;
                case 2: faireDemandeConge(idUtilisateur); break;
                case 3: voirHistoriqueDemandes(idUtilisateur); break;
                case 4: break; // Deconnexion
            }
        } else if (roleUtilisateur == 1) { // Manager
            switch (choixMenu) {
                case 1: listerDemandesEnAttente(idUtilisateur); break;
                case 2: approuverDemande(); break;
                case 3: rejeterDemande(); break;
                case 4: break; // Deconnexion
            }
        } else if (roleUtilisateur == 2) { // Admin
            switch (choixMenu) {
                case 1: gererEmployes(); break;
                case 2: voirToutesLesDemandes(); break;
                case 3: mettreAJourSoldes(); break;
                case 4: break; // Statistiques (a implementer)
                case 5: break; // Deconnexion
            }
        }

    } while (choixMenu != maxChoix);
}


int main(){
    // Configuration de la console pour les caracteres speciaux
    SetConsoleOutputCP(437);
    
    chargerTousLesEmployes(&employes, &nbEmployes);
    chargerTousLesConges(&conges, &nbConges);
    chargerTousLesSoldes(&soldes, &nbSoldes);   

    int choix;
    while(1){
        choix = afficherMenuPrincipal();

        switch (choix) {
            case 1: {
                int IdConnexion = afficherEcranConnexion();
                int RoleUtilisateur = trouverRoleParId(IdConnexion);

                if (RoleUtilisateur != -1) {
                    system("cls");
                    setColor(10); // Vert
                    printf("\n    Connexion reussie ! Bienvenue.\n");
                    setColor(7);
                    Sleep(1000);
                    
                    gererSession(IdConnexion, RoleUtilisateur);
                    
                    system("cls");
                    setColor(11); // Cyan
                    printf("\n    Deconnexion reussie. A bientot !\n");
                    setColor(7);
                    Sleep(1000);
                } else {
                    system("cls");
                    setColor(12); // Rouge
                    printf("\n    ID invalide. Veuillez reessayer.\n");
                    setColor(7);
                    Sleep(1500);
                }
                break;
            }
            case 2: {
                system("cls");
                setColor(11);
                printf("\n\n    Au revoir et a bientot !\n\n");
                setColor(7);
                Sleep(1000);
                return 0;
            }
            default:
                break;
        }
    }

    return 0;
}