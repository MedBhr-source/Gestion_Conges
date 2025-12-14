#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include"interfaces.h"
#include"structs.h"

// Codes des touches
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESCAPE 27

// Couleurs console
#define CLR_NORMAL 7
#define CLR_HIGHLIGHT 112  // Fond blanc, texte noir
#define CLR_TITLE 11       // Cyan clair
#define CLR_BORDER 9       // Bleu

// Déclaration de la fonction pause_screen (prototype)
void pause_screen();

// Fonction pour changer la couleur du texte
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Fonction pour positionner le curseur
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fonction pour cacher le curseur
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Fonction pour afficher le curseur
void showCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Fonction pour dessiner une boite avec caracteres ASCII
void drawBox(int x, int y, int width, int height) {
    setColor(CLR_BORDER);
    
    // Ligne du haut
    gotoxy(x, y);
    printf("+");
    for(int i = 0; i < width - 2; i++) printf("-");
    printf("+");
    
    // Lignes verticales
    for(int i = 1; i < height - 1; i++) {
        gotoxy(x, y + i);
        printf("|");
        gotoxy(x + width - 1, y + i);
        printf("|");
    }
    
    // Ligne du bas
    gotoxy(x, y + height - 1);
    printf("+");
    for(int i = 0; i < width - 2; i++) printf("-");
    printf("+");
    
    setColor(CLR_NORMAL);
}

// Fonction generique pour afficher un menu avec navigation par fleches
int afficherMenuInteractif(const char* titre, const char* options[], int nbOptions) {
    system("cls");
    hideCursor();
    
    int selection = 0;
    int key;
    int boxWidth = 50;
    int boxHeight = nbOptions + 6;
    int startX = 15;
    int startY = 3;
    
    while(1) {
        // Dessiner la boite
        drawBox(startX, startY, boxWidth, boxHeight);
        
        // Afficher le titre
        setColor(CLR_TITLE);
        gotoxy(startX + (boxWidth - strlen(titre)) / 2, startY + 1);
        printf("%s", titre);
        
        // Ligne separatrice
        setColor(CLR_BORDER);
        gotoxy(startX, startY + 2);
        printf("+");
        for(int i = 0; i < boxWidth - 2; i++) printf("-");
        printf("+");
        
        // Afficher les options
        for(int i = 0; i < nbOptions; i++) {
            gotoxy(startX + 3, startY + 4 + i);
            
            if(i == selection) {
                setColor(CLR_HIGHLIGHT);
                printf(" >> %-40s ", options[i]);
            } else {
                setColor(CLR_NORMAL);
                printf("    %-40s ", options[i]);
            }
        }
        
        // Instructions
        setColor(CLR_BORDER);
        gotoxy(startX + 2, startY + boxHeight);
        printf("[Fleches: Naviguer] [Entrer: Selectionner]");
        
        setColor(CLR_NORMAL);
        
        // Lire la touche
        key = _getch();
        
        if(key == 224 || key == 0) { // Touche speciale (fleche)
            key = _getch();
            
            if(key == KEY_UP) {
                selection--;
                if(selection < 0) selection = nbOptions - 1;
            }
            else if(key == KEY_DOWN) {
                selection++;
                if(selection >= nbOptions) selection = 0;
            }
        }
        else if(key == KEY_ENTER) {
            showCursor();
            return selection + 1; // Retourne 1-indexed pour compatibilite
        }
        else if(key == KEY_ESCAPE) {
            showCursor();
            return nbOptions; // Derniere option (souvent deconnexion/quitter)
        }
    }
}

// Fonctions de menu avec navigation par fleches
int afficherMenuPrincipal(){
    const char* options[] = {
        "Se connecter",
        "Quitter"
    };
    return afficherMenuInteractif("BIENVENUE - Menu Principal", options, 2);
}

int afficherMenuEmploye(){
    const char* options[] = {
        "Consulter mon solde de conges",
        "Faire une nouvelle demande de conge",
        "Voir l'historique de mes demandes",
        "Se deconnecter"
    };
    return afficherMenuInteractif("Menu Employe", options, 4);
}

int afficherMenuManager(){
    const char* options[] = {
        "Voir les demandes en attente",
        "Approuver une demande",
        "Rejeter une demande",
        "Se deconnecter"
    };
    return afficherMenuInteractif("Menu Manager", options, 4);
}

int afficherMenuAdmin(){
    const char* options[] = {
        "Gerer les employes",
        "Voir toutes les demandes de conge",
        "Mettre a jour les soldes de conges",
        "Statistiques des conges",
        "Se deconnecter"
    };
    return afficherMenuInteractif("Menu Administrateur", options, 5);
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
    
    // Effacer l'écran après avoir appuyé sur ENTRER
    system("cls");
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
    system("cls");
    printf("\n--- Votre Solde de Conges (ID: %d) ---\n", idUtilisateur);
    
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
        printf("Aucun solde de conge trouve pour votre profil.\\n");
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
    system("cls");
    printf("\n--- Nouvelle Demande de Conge ---\n");

    // 1. Création de la nouvelle demande en mémoire
    Conge nouvelleDemande;
    nouvelleDemande.id = getMaxCongeId() + 1;
    nouvelleDemande.idEmploye = idUtilisateur;
    nouvelleDemande.status = 0; // Le statut est 'en attente' (0)

    printf("Type de conge (0: Annuel, 1: Maladie) : ");
    scanf("%d", &nouvelleDemande.type);

    printf("Date de debut (JJ MM AAAA) : ");
    scanf("%d %d %d", &nouvelleDemande.dateDebut.jour, &nouvelleDemande.dateDebut.mois, &nouvelleDemande.dateDebut.annee);

    printf("Date de fin (JJ MM AAAA) : ");
    scanf("%d %d %d", &nouvelleDemande.dateFin.jour, &nouvelleDemande.dateFin.mois, &nouvelleDemande.dateFin.annee);

    printf("Motif : ");
    scanf(" %199[^\n]", nouvelleDemande.motif);

    // 2. Ajout de la demande au tableau en mémoire (pour la session actuelle)
    // On utilise realloc pour agrandir dynamiquement le tableau
    Conge *temp = realloc(conges, (nbConges + 1) * sizeof(Conge));
    if (temp == NULL) {
        printf("Erreur : Impossible d'ajouter la demande (memoire pleine).\n");
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

    printf("\nDemande creee avec succes ! ID de votre demande : %d\n", nouvelleDemande.id);
    printf("Elle a ete sauvegardee et est maintenant en attente de validation par votre manager.\n");
    printf("----------------------------------\n");
    pause_screen();
}

void voirHistoriqueDemandes(int idUtilisateur){
    system("cls");
    printf("\n\t=== HISTORIQUE DES DEMANDES ===\n");
    printf("--------------------------\n");
    
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt\n");
        return;
    }
    
    char ligne[512];
    int trouveDemande = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        // Supprimer le \n
        ligne[strcspn(ligne, "\n")] = 0;
        
        int id, idEmploye, type, status;
        int jourDebut, moisDebut, anneeDebut, jourFin, moisFin, anneeFin;
        
        // Parser jusqu'au motif
        int n = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
                   &id, &idEmploye, &jourDebut, &moisDebut, &anneeDebut,
                   &jourFin, &moisFin, &anneeFin, &type, &status);
        
        if (n == 10 && idEmploye == idUtilisateur) {
            // Extraire motif entre guillemets
            char *start = strchr(ligne, '"');
            char *end = start ? strchr(start + 1, '"') : NULL;
            char motif[256] = {0};
            
            if (start && end) {
                strncpy(motif, start + 1, end - start - 1);
            }
            
            printf("\nDemande #%d:\n", id);
            printf("  Du: %02d/%02d/%04d au %02d/%02d/%04d\n", 
                   jourDebut, moisDebut, anneeDebut, jourFin, moisFin, anneeFin);
            printf("  Type: %s\\n", type == 0 ? "Annuel" : "Maladie");
            printf("  Statut: %s\\n", 
                   status == 0 ? "En attente" : (status == 1 ? "Approuve" : "Rejete"));
            printf("  Motif: %s\\n", motif);
            trouveDemande = 1;
        }
    }
    
    fclose(fichier);
    
    if(!trouveDemande){
        printf("Aucune demande de conge trouvee pour cet utilisateur.\n");
    }
    printf("--------------------------\n\n");
    pause_screen();
}

// Fonctions Manager
void listerDemandesEnAttente(int idUtilisateur){
    system("cls");
    printf("\n\t=== DEMANDES EN ATTENTE ===\n");
    printf("--------------------------\n");
    
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt\n");
        return;
    }
    
    char ligne[512];
    int trouveDemande = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        // Supprimer le \n
        ligne[strcspn(ligne, "\n")] = 0;
        
        int id, idEmploye, type, status;
        int jourDebut, moisDebut, anneeDebut, jourFin, moisFin, anneeFin;
        
        // Parser jusqu'au motif
        int n = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
                   &id, &idEmploye, &jourDebut, &moisDebut, &anneeDebut,
                   &jourFin, &moisFin, &anneeFin, &type, &status);
        
        if (n == 10 && status == 0) {
            // Extraire motif entre guillemets
            char *start = strchr(ligne, '"');
            char *end = start ? strchr(start + 1, '"') : NULL;
            char motif[256] = {0};
            
            if (start && end) {
                strncpy(motif, start + 1, end - start - 1);
            }
            
            printf("\nDemande #%d:\n", id);
            printf("  Employe ID: %d\n", idEmploye);
            printf("  Du: %02d/%02d/%04d au %02d/%02d/%04d\n", 
                   jourDebut, moisDebut, anneeDebut, jourFin, moisFin, anneeFin);
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
    system("cls");
    printf("\n--- Approuver une Demande ---\n");
    
    int idDemande;
    printf("Entrez l'ID de la demande a approuver: ");
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
                printf("Demande #%d approuvee avec succes!\n", idDemande);
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
        printf("Demande #%d introuvable ou deja traitee.\\n", idDemande);
    }
    
    printf("----------------------------------\n");
    pause_screen();
}

void rejeterDemande(){
    system("cls");
    printf("\n--- Rejeter une Demande ---\n");
    
    int idDemande;
    printf("Entrez l'ID de la demande a rejeter: ");
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
                printf("Demande #%d rejetee avec succes!\n", idDemande);
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
        printf("Demande #%d introuvable ou deja traitee.\n", idDemande);
    }
    
    printf("----------------------------------\n");
    pause_screen();
}

// Fonctions Admin
void gererEmployes(){
    const char* options[] = {
        "Ajouter un employe",
        "Supprimer un employe",
        "Lister tous les employes",
        "Retour au menu"
    };
    
    int choix = afficherMenuInteractif("Gestion des Employes", options, 4);
    showCursor();
    
    switch(choix) {
        case 1: {
            // AJOUTER UN EMPLOYE
            system("cls");
            setColor(CLR_TITLE);
            printf("\n    === AJOUTER UN EMPLOYE ===\n\n");
            setColor(CLR_NORMAL);
            
            int maxId = 0;
            for(int i = 0; i < nbEmployes; i++) {
                if(employes[i].ID > maxId) maxId = employes[i].ID;
            }
            
            Employe nouvelEmploye;
            nouvelEmploye.ID = maxId + 1;
            
            printf("    Nom: ");
            scanf("%14s", nouvelEmploye.nom);
            
            printf("    Prenom: ");
            scanf("%14s", nouvelEmploye.prenom);
            
            printf("    Email: ");
            scanf("%29s", nouvelEmploye.email);
            
            printf("    Departement: ");
            scanf("%14s", nouvelEmploye.Departement);
            
            printf("    Role (0: Employe, 1: Manager, 2: Admin): ");
            scanf("%d", &nouvelEmploye.Role);
            
            // Ajouter à la mémoire
            Employe *temp = realloc(employes, (nbEmployes + 1) * sizeof(Employe));
            if (temp == NULL) {
                setColor(12);
                printf("\n    Erreur : Allocation memoire echouee\n");
                setColor(CLR_NORMAL);
                break;
            }
            employes = temp;
            employes[nbEmployes] = nouvelEmploye;
            nbEmployes++;
            
            // Sauvegarder dans le fichier
            FILE *fichier = fopen("employes.txt", "a");
            if (fichier == NULL) {
                setColor(12);
                printf("\n    Erreur : Impossible d'ouvrir employes.txt\n");
                setColor(CLR_NORMAL);
                break;
            }
            
            fprintf(fichier, "\n%d;%s;%s;%s;%s;%d\n",
                    nouvelEmploye.ID, nouvelEmploye.nom, nouvelEmploye.prenom,
                    nouvelEmploye.email, nouvelEmploye.Departement, nouvelEmploye.Role);
            fclose(fichier);
            
            setColor(10);
            printf("\n    Employe #%d ajoute avec succes!\n", nouvelEmploye.ID);
            setColor(CLR_NORMAL);
            break;
        }
        case 2: {
            // SUPPRIMER UN EMPLOYE
            system("cls");
            setColor(CLR_TITLE);
            printf("\n    === SUPPRIMER UN EMPLOYE ===\n\n");
            setColor(CLR_NORMAL);
            
            int idSupprimer;
            printf("    Entrez l'ID de l'employe a supprimer: ");
            scanf("%d", &idSupprimer);
            
            FILE *fichier = fopen("employes.txt", "r");
            if (fichier == NULL) {
                setColor(12);
                printf("\n    Erreur : Impossible d'ouvrir employes.txt\n");
                setColor(CLR_NORMAL);
                break;
            }
            
            FILE *fichierTemp = fopen("employes_temp.txt", "w");
            if (fichierTemp == NULL) {
                printf("Erreur : Impossible de créer le fichier temporaire\n");
                fclose(fichier);
                break;
            }
            
            char ligne[256];
            int trouve = 0;
            
            while (fgets(ligne, sizeof(ligne), fichier)) {
                if (ligne[0] == '\n' || ligne[0] == '\0') continue;
                
                int id;
                sscanf(ligne, "%d;", &id);
                
                if (id == idSupprimer) {
                    trouve = 1;
                    setColor(10);
                    printf("\n    Employe #%d supprime avec succes!\n", idSupprimer);
                    setColor(CLR_NORMAL);
                } else {
                    fputs(ligne, fichierTemp);
                }
            }
            
            fclose(fichier);
            fclose(fichierTemp);
            
            if (trouve) {
                remove("employes.txt");
                rename("employes_temp.txt", "employes.txt");
                
                // Retirer de la mémoire
                for(int i = 0; i < nbEmployes; i++) {
                    if(employes[i].ID == idSupprimer) {
                        for(int j = i; j < nbEmployes - 1; j++) {
                            employes[j] = employes[j + 1];
                        }
                        nbEmployes--;
                        break;
                    }
                }
            } else {
                remove("employes_temp.txt");
                setColor(12);
                printf("\n    Employe #%d introuvable.\n", idSupprimer);
                setColor(CLR_NORMAL);
            }
            break;
        }
        case 3: {
            system("cls");
            setColor(CLR_TITLE);
            printf("\n    +==================================================+\n");
            printf("    |            LISTE DES EMPLOYES                    |\n");
            printf("    +==================================================+\n\n");
            setColor(CLR_NORMAL);
            
            for(int i = 0; i < nbEmployes; i++) {
                setColor(CLR_BORDER);
                printf("    +------------------------------------------+\n");
                setColor(CLR_NORMAL);
                printf("    | ");
                setColor(14); // Jaune
                printf("ID: %d", employes[i].ID);
                setColor(CLR_NORMAL);
                printf("\n    |   Nom: %s %s\n", employes[i].nom, employes[i].prenom);
                printf("    |   Email: %s\n", employes[i].email);
                printf("    |   Departement: %s\n", employes[i].Departement);
                printf("    |   Role: ");
                if(employes[i].Role == 0) {
                    setColor(10); printf("Employe");
                } else if(employes[i].Role == 1) {
                    setColor(11); printf("Manager");
                } else {
                    setColor(12); printf("Admin");
                }
                setColor(CLR_NORMAL);
                printf("\n");
                setColor(CLR_BORDER);
                printf("    +------------------------------------------+\n");
                setColor(CLR_NORMAL);
            }
            break;
        }
        case 4:
            return;
        default:
            break;
    }
    pause_screen();
}

void voirToutesLesDemandes(){
    system("cls");
    printf("\n\t=== TOUTES LES DEMANDES DE CONGE ===\n");
    printf("--------------------------\n");
    
    FILE *fichier = fopen("conges.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier conges.txt\n");
        return;
    }
    
    char ligne[512];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        // Supprimer le \n
        ligne[strcspn(ligne, "\n")] = 0;
        
        int id, idEmploye, type, status;
        int jourDebut, moisDebut, anneeDebut, jourFin, moisFin, anneeFin;
        
        // Parser jusqu'au motif
        int n = sscanf(ligne, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
                   &id, &idEmploye, &jourDebut, &moisDebut, &anneeDebut,
                   &jourFin, &moisFin, &anneeFin, &type, &status);
        
        if (n == 10) {
            // Extraire motif entre guillemets
            char *start = strchr(ligne, '"');
            char *end = start ? strchr(start + 1, '"') : NULL;
            char motif[256] = {0};
            
            if (start && end) {
                strncpy(motif, start + 1, end - start - 1);
            }
            
            printf("\nDemande #%d:\n", id);
            printf("  Employe ID: %d\n", idEmploye);
            printf("  Du: %02d/%02d/%04d au %02d/%02d/%04d\n", 
                   jourDebut, moisDebut, anneeDebut, jourFin, moisFin, anneeFin);
            printf("  Type: %s\n", type == 0 ? "Annuel" : "Maladie");
            printf("  Statut: %s\n", 
                   status == 0 ? "En attente" : (status == 1 ? "Approuvé" : "Rejeté"));
            printf("  Motif: %s\n", motif);
            trouve = 1;
        }
    }
    
    fclose(fichier);
    
    if(!trouve){
        printf("Aucune demande de conge trouvee.\n");
    }
    printf("--------------------------\n");
    pause_screen();
}

void mettreAJourSoldes(){
    system("cls");
    printf("\n--- Mettre a Jour les Soldes ---\n");
    
    int idEmploye;
    printf("Entrez l'ID de l'employe: ");
    scanf("%d", &idEmploye);
    
    FILE *fichier = fopen("soldes.txt", "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier soldes.txt\n");
        return;
    }
    
    FILE *fichierTemp = fopen("soldes_temp.txt", "w");
    if (fichierTemp == NULL) {
        printf("Erreur : Impossible de créer le fichier temporaire\n");
        fclose(fichier);
        return;
    }
    
    char ligne[128];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (ligne[0] == '\n' || ligne[0] == '\0') continue;
        
        int id, type, jours;
        sscanf(ligne, "%d;%d;%d", &id, &type, &jours);
        
        if (id == idEmploye) {
            printf("Solde actuel pour type %d: %d jours\n", type, jours);
            printf("Nouveaux jours restants (pour type %d): ", type);
            
            int nouveauJours;
            scanf("%d", &nouveauJours);
            
            fprintf(fichierTemp, "%d;%d;%d\n", id, type, nouveauJours);
            trouve = 1;
        } else {
            fputs(ligne, fichierTemp);
        }
    }
    
    fclose(fichier);
    fclose(fichierTemp);
    
    if (trouve) {
        remove("soldes.txt");
        rename("soldes_temp.txt", "soldes.txt");
        printf("Soldes mis a jour avec succes!\n");
    } else {
        remove("soldes_temp.txt");
        printf("Employe #%d introuvable.\n", idEmploye);
    }
    
    printf("----------------------------------\n");
    pause_screen();
}
