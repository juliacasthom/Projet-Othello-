#include "othello.h"
#include <stdio.h>

#define INFINI 1000000


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

// Algorithme alphabeta
int alphabeta(int plateau[8][8], int profondeur, int alpha, int beta, bool estMax, int pionIA, int pionHumain) {
    Coup listeCoups[64];
    int joueurActuel = estMax ? pionIA : pionHumain;
    int nbCoups = generer_coups_possibles(plateau, joueurActuel, listeCoups);

    // Si on a atteint la profondeur max ou si la partie est finie, la fonction s'arrête
    if (profondeur == 0) {
        return evaluer_plateau(plateau, pionIA, pionHumain);
    }

    // Si le joueur actuel est bloqué, on change de tour
    if (nbCoups == 0) {
        // On vérifie si l'adversaire peut jouer
        Coup coupsAdversaire[64];
        if (generer_coups_possibles(plateau, -joueurActuel, coupsAdversaire) == 0) {
            // Si personne ne peut jouer, c'est la fin, on évalue
            return evaluer_plateau(plateau, pionIA, pionHumain);
        }
        // Sinon, on passe le tour
        return alphabeta(plateau, profondeur - 1, alpha, beta, !estMax, pionIA, pionHumain);
    }

    if (estMax) {
        // --- TOUR DE L'IA ---
        for (int i = 0; i < nbCoups; i++) {
            int copiePlateau[8][8];
            copier_plateau(plateau, copiePlateau);
            jouer_coup(copiePlateau, listeCoups[i].ligne, listeCoups[i].colonne, pionIA);

            int scoreRecu = alphabeta(copiePlateau, profondeur - 1, alpha, beta, false, pionIA, pionHumain);
            
            if (scoreRecu > alpha) {
                alpha = scoreRecu;
            }
            if (alpha >= beta) {
                break; // "Élagage Bêta" -> on arrête de chercher dans cette branche
            }
        }
        return alpha;
    } 
    else {
        // --- TOUR DE L'HUMAIN ---
        for (int i = 0; i < nbCoups; i++) {
            int copiePlateau[8][8];
            copier_plateau(plateau, copiePlateau);
            jouer_coup(copiePlateau, listeCoups[i].ligne, listeCoups[i].colonne, pionHumain);

            int scoreRecu = alphabeta(copiePlateau, profondeur - 1, alpha, beta, true, pionIA, pionHumain);
            
            if (scoreRecu < beta) {
                beta = scoreRecu;
            }
            if (alpha >= beta) {
                break; // "Élagage Alpha" -> l'IA ne choisira jamais ce chemin
            }
        }
        return beta;
    }
}

// Fonction appelée par le main pour choisir le meilleur coup (dans le cadre de l'algo alphabeta)
Coup choisir_meilleur_coup_alphabeta(int plateau[8][8], int pionIA, int pionHumain, int profondeurMax) {
    Coup listeCoups[64];
    int nbCoups = generer_coups_possibles(plateau, pionIA, listeCoups);
    Coup meilleurCoup = {-1, -1}; 

    if (nbCoups == 0) return meilleurCoup;

    int meilleurScore = -INFINI;
    int alpha = -INFINI;
    int beta = INFINI;

    for (int i = 0; i < nbCoups; i++) {
        int copiePlateau[8][8];
        copier_plateau(plateau, copiePlateau);
        jouer_coup(copiePlateau, listeCoups[i].ligne, listeCoups[i].colonne, pionIA);

        // Appel de l'algorithme Alpha-Bêta
        int scoreDuCoup = alphabeta(copiePlateau, profondeurMax - 1, alpha, beta, false, pionIA, pionHumain);

        if (scoreDuCoup > meilleurScore) {
            meilleurScore = scoreDuCoup;
            meilleurCoup = listeCoups[i];
        }
        
        if (scoreDuCoup > alpha) alpha = scoreDuCoup;
    }
    return meilleurCoup;
}

int calculer_stabilite_bords(int plateau[8][8], int joueur) {
    int nb = 0;
    // On compte simplement les coins possédés pour l'instant
    if (plateau[0][0] == joueur) nb++;
    if (plateau[0][7] == joueur) nb++;
    if (plateau[7][0] == joueur) nb++;
    if (plateau[7][7] == joueur) nb++;
    return nb;
}
