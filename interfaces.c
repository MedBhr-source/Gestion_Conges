#include<stdio.h>
#include"interfaces.h"


void afficherMenuPrincipal(){
    printf("\tBIENVENUE\n");
    printf("--Menu Principal--\n");
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