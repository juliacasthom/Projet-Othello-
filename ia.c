#include "othello.h"
#include <stdio.h>

// Matrice de poids pour la stratégie de positionnement
const int POIDS_IA[8][8] = {
    { 100, -20,  10,   5,   5,  10, -20, 100},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 10,  -2,   5,   1,   1,   5,  -2,  10},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    { 10,  -2,   5,   1,   1,   5,  -2,  10},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 100, -20,  10,   5,   5,  10, -20, 100}
};

// Fonction utilitaire pour copier le tableau
void copier_plateau(int src[8][8], int dest[8][8]) {
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            dest[i][j] = src[i][j];
}

// Générateur de coups pour l'IA
int generer_coups_possibles(int plateau[8][8], int joueur, Coup liste_coups[64]) {
    int nb = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (est_coup_valide(plateau, i, j, joueur)) {
                liste_coups[nb].ligne = i;
                liste_coups[nb].colonne = j;
                nb++;
            }
        }
    }
    return nb;
}

// Évaluation du plateau
int evaluer_plateau(int plateau[8][8], int ia, int humain) {
    int score = 0;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(plateau[i][j] == ia) score += POIDS_IA[i][j];
            else if(plateau[i][j] == humain) score -= POIDS_IA[i][j];
        }
    }
    return score;
}

// Algorithme Minimax
int minimax(int plateau[8][8], int depth, bool isMax, int ia, int humain) {
    Coup coups[64];
    int joueur_actuel = isMax ? ia : humain;
    int nb_coups = generer_coups_possibles(plateau, joueur_actuel, coups);

    if (depth == 0 || nb_coups == 0) {
        return evaluer_plateau(plateau, ia, humain);
    }

    if (isMax) {
        int bestScore = -1000000;
        for (int i = 0; i < nb_coups; i++) {
            int copie[8][8];
            copier_plateau(plateau, copie);
            jouer_coup(copie, coups[i].ligne, coups[i].colonne, ia);
            int score = minimax(copie, depth - 1, false, ia, humain);
            if (score > bestScore) bestScore = score;
        }
        return bestScore;
    } else {
        int bestScore = 1000000;
        for (int i = 0; i < nb_coups; i++) {
            int copie[8][8];
            copier_plateau(plateau, copie);
            jouer_coup(copie, coups[i].ligne, coups[i].colonne, humain);
            int score = minimax(copie, depth - 1, true, ia, humain);
            if (score < bestScore) bestScore = score;
        }
        return bestScore;
    }
}

// Fonction appelée par le main pour choisir le meilleur coup
Coup choisir_meilleur_coup(int plateau[8][8], int ia, int humain, int profondeur) {
    Coup coups[64];
    int nb = generer_coups_possibles(plateau, ia, coups);
    Coup meilleur = coups[0];
    int maxS = -1000000;

    for(int i=0; i<nb; i++) {
        int copie[8][8];
        copier_plateau(plateau, copie);
        jouer_coup(copie, coups[i].ligne, coups[i].colonne, ia);
        int s = minimax(copie, profondeur - 1, false, ia, humain);
        if(s > maxS) {
            maxS = s;
            meilleur = coups[i];
        }
    }
    return meilleur;
}