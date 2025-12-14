#ifndef interface_h
#define interface_h

// Fonctions de menu interactif (retournent le choix)
int afficherMenuPrincipal();
int afficherMenuEmploye();
int afficherMenuManager();
int afficherMenuAdmin();

// Fonction de menu generique
int afficherMenuInteractif(const char* titre, const char* options[], int nbOptions);

// Fonctions utilitaires interface
void setColor(int color);
void gotoxy(int x, int y);
void hideCursor();
void showCursor();
void drawBox(int x, int y, int width, int height);
void pause_screen();

// Fonctions Employe
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