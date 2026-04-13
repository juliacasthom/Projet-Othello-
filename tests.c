#include <stdio.h>
#include <assert.h>
#include "othello.h"

// --- UTILITAIRES ---

void vider(int p[8][8]) {
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) p[i][j] = VIDE;
}

// --- TESTS EXISTANTS ---

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

void test_minimax_simple() {
    int p[8][8];
    vider(p);
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) p[i][j] = BLANC;
    p[0][0] = VIDE; p[0][1] = VIDE; p[0][2] = VIDE;
    printf("Test Minimax en cours...\n");
    int score = minimax(p, 2, true, NOIR, BLANC); 
    printf("Test Minimax: OK (Score calcule: %d)\n", score);
}

// --- NOUVEAU TEST : ALPHA-BÊTA ---

void test_alphabeta_simple() {
    int p[8][8];
    vider(p);

    // On prépare un plateau presque plein
    for(int i=0; i<8; i++) 
        for(int j=0; j<8; j++) 
            p[i][j] = BLANC;

    // On laisse 4 cases vides
    p[0][0] = VIDE; // Case très forte (Coin)
    p[0][1] = VIDE; // Case faible
    p[7][7] = VIDE; // Autre coin
    p[1][1] = VIDE; // Case dangereuse (X-square)

    printf("Test Alpha-Beta en cours...\n");
    
    // 1. Tester si la fonction alphabeta renvoie un score cohérent
    int score = alphabeta(p, 3, -1000000, 1000000, true, NOIR, BLANC);
    assert(score != 0);

    // 2. Tester si choisir_meilleur_coup_alphabeta choisit bien un coin
    Coup meilleur = choisir_meilleur_coup_alphabeta(p, NOIR, BLANC, 4);
    
    // L'IA devrait choisir soit (0,0) soit (7,7) car ce sont des coins (poids 100)
    // Contrairement à (1,1) qui a un poids négatif (-50)
    assert((meilleur.ligne == 0 && meilleur.colonne == 0) || (meilleur.ligne == 7 && meilleur.colonne == 7));

    printf("Test Alpha-Beta: OK (Meilleur coup trouve en [%d,%d] avec score %d)\n", 
            meilleur.ligne, meilleur.colonne, score);
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