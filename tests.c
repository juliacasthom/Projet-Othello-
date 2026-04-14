#include <stdio.h>
#include <assert.h>
#include "othello.h"

// --- UTILITAIRES ---

void vider(int p[8][8]) {
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) p[i][j] = VIDE;
}

// --- FONCTIONS ---

void test_evaluation_basique() {
    int p1[8][8], p2[8][8];
    vider(p1); vider(p2);
    p1[3][3] = NOIR; 
    p2[0][0] = NOIR; 
    int s1 = evaluer_plateau(p1, NOIR, BLANC);
    int s2 = evaluer_plateau(p2, NOIR, BLANC);
    assert(s2 > s1);
    printf("Test Evaluation: OK (Coin > Centre)\n");
}

void test_stabilite_propagation() {
    int p[8][8];
    vider(p);
    p[0][0] = NOIR; p[0][1] = NOIR; p[1][0] = NOIR;
    int stab = calculer_stabilite_bords(p, NOIR);
    assert(stab == 3);
    printf("Test Stabilite: OK (3 pions stables trouves)\n");
}

// --- MINIMAX ---

void test_minimax_simple() {
    int p[8][8];
    vider(p);
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) p[i][j] = BLANC;
    p[0][0] = VIDE; p[0][1] = VIDE; p[0][2] = VIDE;
    printf("Test Minimax en cours...\n");
    int score = minimax(p, 2, true, NOIR, BLANC); 
    printf("Test Minimax: OK (Score calcule: %d)\n", score);
}

// --- ALPHA-BÊTA ---

void test_alphabeta_simple() {
    int p[8][8];
    vider(p);

    // 1. On remplit presque tout de BLANC
    for(int i=0; i<8; i++) 
        for(int j=0; j<8; j++) 
            p[i][j] = BLANC;

    // 2. On vide les cases qu'on veut tester
    p[0][0] = VIDE; // Coin (Très fort)
    p[0][1] = VIDE; // Case à côté du coin (Faible)
    p[7][7] = VIDE; // Autre coin (Très fort)
    p[1][1] = VIDE; // Case X (Très dangereuse)

    // 3. TRÈS IMPORTANT : Placer des pions NOIRS pour rendre les coups valides
    // Pour que Noir puisse jouer en (0,0), il faut un Blanc en (0,1) et un Noir en (0,2)
    p[0][1] = BLANC;
    p[0][2] = NOIR;  // Ancre pour le coin haut-gauche
    
    // Pour que Noir puisse jouer en (7,7), il faut un Blanc en (6,7) et un Noir en (5,7)
    p[6][7] = BLANC;
    p[5][7] = NOIR;  // Ancre pour le coin bas-droite

    // Pour que Noir puisse jouer en (1,1), il faut un Blanc en (2,2) et un Noir en (3,3)
    p[2][2] = BLANC;
    p[3][3] = NOIR;  // Ancre pour la case dangereuse

    printf("Test Alpha-Beta en cours...\n");
    
    // L'IA doit maintenant trouver des coups valides
    Coup meilleur = choisir_meilleur_coup_alphabeta(p, NOIR, BLANC, 4);
    
    printf("IA a choisi : Ligne %d, Col %d\n", meilleur.ligne, meilleur.colonne);

    // L'IA doit impérativement choisir un des deux coins car ils valent +100
    // et éviter (1,1) qui a un gros malus dans ta matrice.
    assert((meilleur.ligne == 0 && meilleur.colonne == 0) || (meilleur.ligne == 7 && meilleur.colonne == 7));

    printf("Test Alpha-Beta: OK\n");
}

// --- MAIN ---

int main() {
    printf("--- DEMARRAGE DES TESTS UNITAIRES ---\n");
    
    test_evaluation_basique();
    test_stabilite_propagation();
    test_minimax_simple();
    test_alphabeta_simple(); // Appel du nouveau test
    
    printf("--- TOUS LES TESTS ONT REUSSI ---\n");
    return 0;
}