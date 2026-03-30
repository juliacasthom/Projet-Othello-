#ifndef IA_H
#define IA_H

#include <stdbool.h>

#define VIDE 0
#define NOIR 1
#define BLANC -1

typedef struct {
    int x;
    int y;
} Coup;

// Prototypes temporaires pour permettre le test des algorithmes
int est_coup_valide(int plateau[8][8], int x, int y, int joueur);
void jouer_coup(int plateau[8][8], int x, int y, int joueur);

// Prototypes des fonctions d'évaluation
int evaluer_plateau(int plateau[8][8], int ia, int humain);
int calculer_score_position(int plateau[8][8], int ia, int humain);
int calculer_stabilite_bords(int plateau[8][8], int joueur);

// Prototypes du moteur IA
int minimax(int plateau[8][8], int depth, bool isMax, int ia, int humain);
Coup choisir_meilleur_coup(int plateau[8][8], int ia, int humain, int profondeur);

// Utilitaires
void copier_plateau(int source[8][8], int destination[8][8]);
int generer_coups_possibles(int plateau[8][8], int joueur, Coup liste_coups[64]);

#endif