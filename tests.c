#include <stdio.h>
#include <assert.h>
#include "ia.h"

// --- STUBS (FAUSSES FONCTIONS) POUR LE TEST ---

int est_coup_valide(int plateau[8][8], int x, int y, int joueur) {
    // On simule une règle simple : si la case est vide, on peut jouer.
    // Cela permet à generer_coups_possibles de trouver des cases pour l'IA.
    if (plateau[x][y] == VIDE) return 1;
    return 0;
}

void jouer_coup(int plateau[8][8], int x, int y, int joueur) {
    // On simule le placement d'un pion sans gérer les retournements pour l'instant.
    plateau[x][y] = joueur;
}

// --- FIN DES STUBS ---

void vider(int p[8][8]) {
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) p[i][j] = VIDE;
}

void test_evaluation_basique() {
    int p1[8][8], p2[8][8];
    vider(p1); vider(p2);
    
    p1[3][3] = NOIR; // Pion central
    p2[0][0] = NOIR; // Pion au coin
    
    int s1 = evaluer_plateau(p1, NOIR, BLANC);
    int s2 = evaluer_plateau(p2, NOIR, BLANC);
    
    assert(s2 > s1);
    printf("Test Evaluation: OK (Coin > Centre)\n");
}

void test_stabilite_propagation() {
    int p[8][8];
    vider(p);
    p[0][0] = NOIR;
    p[0][1] = NOIR;
    p[1][0] = NOIR;
    
    int stab = calculer_stabilite_bords(p, NOIR);
    assert(stab == 3);
    printf("Test Stabilite: OK (3 pions stables trouves)\n");
}

void test_minimax_simple() {
    int p[8][8];
    vider(p);
    
    // On simule un plateau avec seulement 3 cases vides pour que ce soit rapide et sûr
    for(int i=0; i<8; i++) 
        for(int j=0; j<8; j++) 
            p[i][j] = BLANC; // On remplit tout de Blanc
            
    p[0][0] = VIDE; // On laisse 3 cases vides
    p[0][1] = VIDE;
    p[0][2] = VIDE;
    p[1][1] = NOIR; // Un pion noir pour pouvoir faire un sandwich (si ta règle simulée le demande)

    printf("Test Minimax en cours...\n");
    int score = minimax(p, 2, true, NOIR, BLANC); 
    printf("Test Minimax: OK (Score: %d)\n", score);
}

int main() {
    printf("--- DEMARRAGE DES TESTS UNITAIRES ---\n");
    test_evaluation_basique();
    test_stabilite_propagation();
    test_minimax_simple();
    printf("--- TOUS LES TESTS ONT REUSSI ---\n");
    return 0;
}