#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    int ID ;
    char nom [15];
    char prenom[15];
    char email[30];
    char Departement[15];
    int Role; // 0: employe 1: Manager 2: admin
}Employe;

typedef struct {
    int jour;
    int mois;
    int annee;
}Date;

typedef struct {
    int id;
    int idEmploye;
    Date dateDebut;
    Date dateFin;
    int type; // 0: Annuel 1: Maladie
    int status; // 0: En attente  1: Approuve  2: Rejete
    char motif[50];
} Conge;

typedef struct {
    int idEmp ;
    int typeConge;      // Le type de congé (ex: 0: Annuel, 1: Maladie, 2: Maternité)
    int joursRestants;
}SoldeConge;

#endif
