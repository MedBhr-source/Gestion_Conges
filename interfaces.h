#ifndef interface_h
#define interface_h

void afficherMenuPrincipal();
void afficherMenuEmploye();
void afficherMenuManager();
void afficherMenuAdmin();

// Fonctions Employé
void afficherSoldeUtilisateur(int idUtilisateur);
void faireDemandeConge(int idUtilisateur);
void voirHistoriqueDemandes(int idUtilisateur);

// Fonctions Manager
void listerDemandesEnAttente(int idUtilisateur);
void approuverDemande();
void rejeterDemande();

// Fonctions Admin
void gererEmployes();
void voirToutesLesDemandes();
void mettreAJourSoldes();
void afficherStatistiques();

#endif