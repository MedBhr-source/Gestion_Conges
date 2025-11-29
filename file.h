#ifndef FILES_H
#define FILES_H

#include "structs.h"

int trouverRoleParId(int idRecherche);

void chargerTousLesEmployes(Employe **listeEmployes, int *nbEmployes);

void chargerTousLesConges(Conge **listeConges, int *nbConges);
void chargerTousLesSoldes(SoldeConge **listeSoldes, int *nbSoldes);


#endif
