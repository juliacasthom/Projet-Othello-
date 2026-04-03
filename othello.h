#ifndef OTHELLO_H
#define OTHELLO_H

#include <stdbool.h>

#define SIZE 8
#define VIDE 0
#define NOIR 1
#define BLANC -1

// Structure pour le plateau
typedef struct {
    int cases[SIZE][SIZE]; 
    int joueurActuel; 
    int scoreNoir; 
    int scoreBlanc;
} Plateau; 

// Structure pour un coup
typedef struct {
    int ligne;
    int colonne;
} Coup;

// --- FONCTIONS DU MOTEUR DE JEU (moteur.c) ---
int est_coup_valide(int p[8][8], int r, int c, int joueur);
void jouer_coup(int p[8][8], int r, int c, int joueur);

// --- FONCTIONS DE L'IA (ia.c) ---
int generer_coups_possibles(int plateau[8][8], int joueur, Coup liste_coups[64]);
Coup choisir_meilleur_coup(int plateau[8][8], int ia, int humain, int profondeur);
int minimax(int plateau[8][8], int depth, bool isMax, int ia, int humain);
int evaluer_plateau(int plateau[8][8], int ia, int humain);

#endif