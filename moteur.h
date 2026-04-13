#ifndef MOTEUR_H
#define MOTEUR_H

#include <stdbool.h>

int captureDirection(int p[8][8], int r, int c, int dr, int dc, int joueur);
int est_coup_valide(int p[8][8], int r, int c, int joueur);
#endif