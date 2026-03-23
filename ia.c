#include <stdbool.h>

// Vos constantes (à adapter selon votre projet)
#define VIDE 0
#define NOIR 1
#define BLANC 2

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


/*
 * Calcule le nombre de pions stables sur les bords pour un joueur donné.
 */
int calculer_stabilite_bords(int plateau[8][8], int joueur) {
    // Tableau pour mémoriser les pions déjà comptés comme stables 
    // (pour éviter de compter 2 fois un pion si un bord est entièrement rempli)
    bool est_stable[8][8] = {false};
    int nb_stables = 0;

    // Coordonnées des 4 coins
    int coins_x[4] = {0, 0, 7, 7};
    int coins_y[4] = {0, 7, 0, 7};

    // Directions de propagation pour chaque coin (dx = ligne, dy = colonne)
    // Coin (0,0) propage vers le Bas (1,0) et vers la Droite (0,1)
    // Coin (0,7) propage vers le Bas (1,0) et vers la Gauche (0,-1)
    // Coin (7,0) propage vers le Haut (-1,0) et vers la Droite (0,1)
    // Coin (7,7) propage vers le Haut (-1,0) et vers la Gauche (0,-1)
    int dir_x[4][2] = { {1, 0}, {1, 0}, {-1, 0}, {-1, 0} }; 
    int dir_y[4][2] = { {0, 1}, {0, -1}, {0, 1}, {0, -1} };

    // Étape 1 : On boucle sur les 4 coins
    for (int c = 0; c < 4; c++) {
        int cx = coins_x[c];
        int cy = coins_y[c];

        // Si le coin appartient au joueur, c'est notre "ancre"
        if (plateau[cx][cy] == joueur) {
            
            // On marque le coin comme stable
            if (!est_stable[cx][cy]) {
                est_stable[cx][cy] = true;
                nb_stables++;
            }

            // Étape 2 : On propage sur les 2 bords connectés à ce coin
            for (int d = 0; d < 2; d++) {
                int x = cx + dir_x[c][d];
                int y = cy + dir_y[c][d];

                // Tant qu'on reste sur le plateau ET que c'est un pion du joueur
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && plateau[x][y] == joueur) {
                    
                    // Si ce pion n'a pas encore été compté
                    if (!est_stable[x][y]) {
                        est_stable[x][y] = true;
                        nb_stables++;
                    }
                    
                    // On avance à la case suivante sur le bord
                    x += dir_x[c][d];
                    y += dir_y[c][d];
                }
            }
        }
    }

    return nb_stables;
}


int evaluer_plateau(int plateau[8][8], int ia, int humain) {
    int score = 0;
    
    // 1. Matrice des poids (Position)
    score += calculer_score_position(plateau, ia, humain); 
    
    // 2. Mobilité (Différence des coups possibles)
    // score += ...
    
    // 3. Stabilité (Nouveau !)
    int stabilite_ia = calculer_stabilite_bords(plateau, ia);
    int stabilite_humain = calculer_stabilite_bords(plateau, humain);
    
    // On donne une énorme valeur à la stabilité (ex: 200 points par pion stable)
    score += (stabilite_ia - stabilite_humain) * 200; 
    
    return score;
}