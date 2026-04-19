#include "othello.h"
#include <stdio.h>

#define INFINI 1000000
#define COEFF_POSITION 10
#define COEFF_MOBILITE 50
#define COEFF_STABILITE 150 


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

/* Calcul stabilité:
 Un pion est stable s'il ne peut plus jamais être retourné.
 La stabilité se propage à partir des coins, le long des bords.*/
int calculer_stabilite_bords(int plateau[8][8], int joueur) {
    bool est_stable[8][8] = {false};
    int nb_stables = 0;

    int coins_x[4] = {0, 0, 7, 7};
    int coins_y[4] = {0, 7, 0, 7};
    int dir_x[4][2] = { {1, 0}, {1, 0}, {-1, 0}, {-1, 0} }; 
    int dir_y[4][2] = { {0, 1}, {0, -1}, {0, 1}, {0, -1} };

    for (int c = 0; c < 4; c++) {
        int cx = coins_x[c], cy = coins_y[c];
        if (plateau[cx][cy] == joueur) {
            if (!est_stable[cx][cy]) { est_stable[cx][cy] = true; nb_stables++; }
            for (int d = 0; d < 2; d++) {
                int x = cx + dir_x[c][d], y = cy + dir_y[c][d];
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && plateau[x][y] == joueur) {
                    if (!est_stable[x][y]) { est_stable[x][y] = true; nb_stables++; }
                    x += dir_x[c][d]; y += dir_y[c][d];
                }
            }
        }
    }
    return nb_stables;
}

// Évaluation du plateau
int evaluer_plateau(int plateau[8][8], int ia, int humain) {
    int score_position = 0;
    int nb_ia = 0, nb_humain = 0, vides = 0;

    // 1. Position
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(plateau[i][j] == ia) {
                score_position += POIDS_IA[i][j];
                nb_ia++;
            } else if(plateau[i][j] == humain) {
                score_position -= POIDS_IA[i][j];
                nb_humain++;
            } else {
                vides++;
            }
        }
    }

    // 2. Mobilité (Nombre de coups disponibles)
    Coup temp[64];
    int mob_ia = generer_coups_possibles(plateau, ia, temp);
    int mob_humain = generer_coups_possibles(plateau, humain, temp);

    // 3. Stabilité
    int stab_ia = calculer_stabilite_bords(plateau, ia);
    int stab_humain = calculer_stabilite_bords(plateau, humain);

    // Calcul final
    int score_final = 0;
    score_final += score_position * COEFF_POSITION;           // Coef Position : 10
    score_final += (mob_ia - mob_humain) * COEFF_MOBILITE;    // Coef Mobilité : 50
    score_final += (stab_ia - stab_humain) * COEFF_STABILITE; // Coef Stabilité : 150

    // En fin de partie, le nombre de pions de l'abversaire rentre en compte
    if (vides < 10) score_final += (nb_ia - nb_humain) * 100;
    else score_final += (nb_ia - nb_humain) * 1;

    return score_final;
}


// Algorithme Minimax
//int minimax(int plateau[8][8], int depth, bool isMax, int ia, int humain) {
    //Coup coups[64];
    //int joueur_actuel = isMax ? ia : humain;
    //int nb_coups = generer_coups_possibles(plateau, joueur_actuel, coups);

    //if (depth == 0) return evaluer_plateau(plateau, ia, humain);

    //if (nb_coups == 0) {
        //if (generer_coups_possibles(plateau, -joueur_actuel, coups) == 0)
            //return evaluer_plateau(plateau, ia, humain);
        //return minimax(plateau, depth - 1, !isMax, ia, humain);
    //}

    //int bestScore = isMax ? -INFINI : INFINI;
    //for (int i = 0; i < nb_coups; i++) {
        //int copie[8][8];
        //copier_plateau(plateau, copie);
        //jouer_coup(copie, coups[i].ligne, coups[i].colonne, joueur_actuel);
        //int score = minimax(copie, depth - 1, !isMax, ia, humain);
        //if (isMax) { if (score > bestScore) bestScore = score; }
        //else { if (score < bestScore) bestScore = score; }
    //}
    //return bestScore;
//}

// Fonction appelée par le main pour choisir le meilleur coup
//Coup choisir_meilleur_coup(int plateau[8][8], int ia, int humain, int profondeur) {
    //Coup coups[64];
    //int nb = generer_coups_possibles(plateau, ia, coups);
    //Coup meilleur = coups[0];
    //int maxS = -1000000;

    //for(int i=0; i<nb; i++) {
        //int copie[8][8];
        //copier_plateau(plateau, copie);
        //jouer_coup(copie, coups[i].ligne, coups[i].colonne, ia);
        //int s = minimax(copie, profondeur - 1, false, ia, humain);
        //if(s > maxS) {
            //maxS = s;
            //meilleur = coups[i];
        //}
    //}
    //return meilleur;
//}

// fonction pour trier les coups basés sur la matrice de poids
// Plus le poids est élevé, plus le coup est prioritaire
void trier_coups(Coup liste_coups[], int nb_coups) {
    for (int i= 0; i<nb_coups-1; i++) {
        for (int j= i+1; j < nb_coups; j++) {
            
            // On récupère les poids des deux coups comparés
            int poids1= POIDS_IA[liste_coups[i].ligne][liste_coups[i].colonne];
            int poids2= POIDS_IA[liste_coups[j].ligne][liste_coups[j].colonne];

            // Si le coup actuel a un poids inférieur au coup suivant, on échange
            if (poids1 < poids2) {
                Coup temp = liste_coups[i];
                liste_coups[i] = liste_coups[j];
                liste_coups[j] = temp;
            }
        }
    }
}

// Algorithme alphabeta
int alphabeta(int plateau[8][8], int profondeur, int alpha, int beta, bool estMax, int pionIA, int pionHumain) {
    Coup listeCoups[64];
    int joueurActuel = estMax ? pionIA : pionHumain;
    int nbCoups = generer_coups_possibles(plateau, joueurActuel, listeCoups);
    trier_coups(listeCoups, nbCoups);

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
    trier_coups(listeCoups, nbCoups);
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

