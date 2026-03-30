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

typedef struct {
    int x;
    int y;
} Coup;


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

int generer_coups_possibles(int plateau[8][8], int joueur, Coup liste_coups[60]) {
    int nb_coups = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // On utilise votre fonction de vérification de validité (le "sandwich")
            if (est_coup_valide(plateau, i, j, joueur)) {
                liste_coups[nb_coups].x = i;
                liste_coups[nb_coups].y = j;
                nb_coups++;
            }
        }
    }
    return nb_coups; // Retourne le nombre de coups trouvés
}

Coup choisir_meilleur_coup(int plateau[8][8], int ia, int humain) {
    Coup coups[60];
    int nb_coups = generer_coups_possibles(plateau, ia, coups);
    
    Coup bestMove = coups[0];
    int bestScore = -1000000;
    int profondeur = 4; // Tu peux régler la difficulté ici

    for (int i = 0; i < nb_coups; i++) {
        int plateauCopie[8][8];
        for(int r=0; r<8; r++) for(int c=0; c<8; c++) plateauCopie[r][c] = plateau[r][c];
        
        jouer_coup(plateauCopie, coups[i].x, coups[i].y, ia);
        
        // On appelle le minimax pour voir ce que ce coup rapporte
        int score = minimax(plateauCopie, profondeur - 1, false, ia, humain);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = coups[i];
        }
    }
    return bestMove;
}


int evaluer_plateau(int plateau[8][8], int ia, int humain) {
    int score = 0;
    int nb_pions_ia = 0;
    int nb_pions_humain = 0;
    int nb_cases_vides = 0;

    // --- 1. POSITION (POIDS DES CASES) ---
    // Cette fonction parcourt le plateau et fait la somme selon la matrice de poids
    score += calculer_score_position(plateau, ia, humain); 

    // --- 2. MOBILITÉ (LIBERTÉ DE MOUVEMENT) ---
    // On compte combien de coups l'IA peut jouer vs l'Humain
    int mob_ia = calculer_nb_coups_possibles(plateau, ia);
    int mob_humain = calculer_nb_coups_possibles(plateau, humain);
    
    // On donne un poids important à la mobilité (ex: 50 points par coup d'avance)
    score += (mob_ia - mob_humain) * 50;

    // --- 3. STABILITÉ (DÉFENSE DES BORDS) ---
    int stabilite_ia = calculer_stabilite_bords(plateau, ia);
    int stabilite_humain = calculer_stabilite_bords(plateau, humain);
    
    // La stabilité est très précieuse (200 points par pion stable)
    score += (stabilite_ia - stabilite_humain) * 200; 

    // --- 4. MATÉRIEL (COMPTAGE DES PIONS) ---
    // On compte les pions présents et les cases vides pour savoir si on est en fin de partie
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (plateau[i][j] == ia) nb_pions_ia++;
            else if (plateau[i][j] == humain) nb_pions_humain++;
            else nb_cases_vides++;
        }
    }

    // Stratégie différente selon la phase de jeu :
    if (nb_cases_vides > 10) {
        // En début/milieu de partie, le nombre de pions compte peu (poids de 1)
        score += (nb_pions_ia - nb_pions_humain) * 1;
    } else {
        // En fin de partie (moins de 10 cases), gagner des pions devient la priorité
        score += (nb_pions_ia - nb_pions_humain) * 100;
    }

    return score;
}


int minimax(int plateau[8][8], int depth, bool isMax, int ia, int humain) {
    // 1. Condition d'arrêt : profondeur atteinte ou fin de partie
    // (Ici on simplifie : si plus de coups possibles, on évalue)
    Coup coups[60];
    int nb_coups = generer_coups_possibles(plateau, isMax ? ia : humain, coups);

    if (depth == 0 || nb_coups == 0) {
        return evaluer_plateau(plateau, ia, humain);
    }

    if (isMax) { // Tour de l'IA (Programme)
        int bestScore = -1000000; // Équivalent de -INFINITY

        for (int i = 0; i < nb_coups; i++) {
            // Simulation du coup sur une copie pour ne pas abîmer le vrai plateau
            int plateauCopie[8][8];
            for(int r=0; r<8; r++) for(int c=0; c<8; c++) plateauCopie[r][c] = plateau[r][c];
            
            jouer_coup(plateauCopie, coups[i].x, coups[i].y, ia); // Ta fonction qui retourne les pions
            
            int score = minimax(plateauCopie, depth - 1, false, ia, humain);
            
            if (score > bestScore) {
                bestScore = score;
            }
        }
        return bestScore;
    } 
    else { // Tour de l'Adversaire (Humain)
        int bestScore = 1000000; // Équivalent de +INFINITY

        for (int i = 0; i < nb_coups; i++) {
            int plateauCopie[8][8];
            for(int r=0; r<8; r++) for(int c=0; c<8; c++) plateauCopie[r][c] = plateau[r][c];
            
            jouer_coup(plateauCopie, coups[i].x, coups[i].y, humain);
            
            int score = minimax(plateauCopie, depth - 1, true, ia, humain);
            
            if (score < bestScore) {
                bestScore = score;
            }
        }
        return bestScore;
    }
}