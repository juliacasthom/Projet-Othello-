#include "ia.h"
#include <stdio.h>

// Matrice de poids pour la stratégie de positionnement
const int POIDS[8][8] = {
    { 100, -20,  10,   5,   5,  10, -20, 100},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 10,  -2,   5,   1,   1,   5,  -2,  10},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    { 10,  -2,   5,   1,   1,   5,  -2,  10},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 100, -20,  10,   5,   5,  10, -20, 100}
};

void copier_plateau(int src[8][8], int dest[8][8]) {
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            dest[i][j] = src[i][j];
}

int calculer_score_position(int plateau[8][8], int ia, int humain) {
    int score = 0;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(plateau[i][j] == ia) score += POIDS[i][j];
            else if(plateau[i][j] == humain) score -= POIDS[i][j];
        }
    }
    return score;
}

int calculer_stabilite_bords(int plateau[8][8], int joueur) {
    bool est_stable[8][8] = {false};
    int nb = 0;
    int cx[4] = {0, 0, 7, 7}, cy[4] = {0, 7, 0, 7};
    int dx[4][2] = {{1,0},{1,0},{-1,0},{-1,0}}, dy[4][2] = {{0,1},{0,-1},{0,1},{0,-1}};

    for(int c=0; c<4; c++) {
        if(plateau[cx[c]][cy[c]] == joueur) {
            if(!est_stable[cx[c]][cy[c]]) { est_stable[cx[c]][cy[c]] = true; nb++; }
            for(int d=0; d<2; d++) {
                int x = cx[c] + dx[c][d], y = cy[c] + dy[c][d];
                while(x>=0 && x<8 && y>=0 && y<8 && plateau[x][y] == joueur) {
                    if(!est_stable[x][y]) { est_stable[x][y] = true; nb++; }
                    x += dx[c][d]; y += dy[c][d];
                }
            }
        }
    }
    return nb;
}

int generer_coups_possibles(int plateau[8][8], int joueur, Coup liste_coups[64]) {
    int nb_coups = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Attention : est_coup_valide doit être définie quelque part (ex: plateau.c)
            if (est_coup_valide(plateau, i, j, joueur)) {
                liste_coups[nb_coups].x = i;
                liste_coups[nb_coups].y = j;
                nb_coups++;
            }
        }
    }
    return nb_coups;
}

int evaluer_plateau(int plateau[8][8], int ia, int humain) {
    int nb_ia = 0, nb_hum = 0, vides = 0;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(plateau[i][j] == ia) nb_ia++;
            else if(plateau[i][j] == humain) nb_hum++;
            else vides++;
        }
    }
    
    int score = calculer_score_position(plateau, ia, humain) * 10;
    score += (calculer_stabilite_bords(plateau, ia) - calculer_stabilite_bords(plateau, humain)) * 200;
    
    // Mobilité (à implémenter si vous avez generer_coups)
    // score += (mob_ia - mob_humain) * 50;

    if(vides < 10) score += (nb_ia - nb_hum) * 100; // Fin de partie
    else score += (nb_ia - nb_hum) * 1;
    
    return score;
}

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
            jouer_coup(copie, coups[i].x, coups[i].y, ia); // Fonction externe
            int score = minimax(copie, depth - 1, false, ia, humain);
            if (score > bestScore) bestScore = score;
        }
        return bestScore;
    } else {
        int bestScore = 1000000;
        for (int i = 0; i < nb_coups; i++) {
            int copie[8][8];
            copier_plateau(plateau, copie);
            jouer_coup(copie, coups[i].x, coups[i].y, humain); // Fonction externe
            int score = minimax(copie, depth - 1, true, ia, humain);
            if (score < bestScore) bestScore = score;
        }
        return bestScore;
    }
}

Coup choisir_meilleur_coup(int plateau[8][8], int ia, int humain, int profondeur) {
    Coup coups[64];
    int nb = generer_coups_possibles(plateau, ia, coups);
    Coup meilleur = coups[0];
    int maxS = -1000000;

    for(int i=0; i<nb; i++) {
        int copie[8][8];
        copier_plateau(plateau, copie);
        jouer_coup(copie, coups[i].x, coups[i].y, ia);
        int s = minimax(copie, profondeur - 1, false, ia, humain);
        if(s > maxS) { maxS = s; meilleur = coups[i]; }
    }
    return meilleur;
}