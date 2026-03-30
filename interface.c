#include "othello.h"
#include <stdio.h>

 
void afficher_Plateau(Plateau *p){
    printf("\n     A   B   C   D   E   F   G   H\n");
    printf("   +---+---+---+---+---+---+---+---+\n");

    for (int i = 0; i < SIZE; i++) {
        printf(" %d |", i + 1); // Afficher numero de ligne 

        for (int j=0; j<SIZE; j++) {
            if (p->cases[i][j]==VIDE) {
                printf("   |"); // case vide 
            } else if (p->cases[i][j]==NOIR) {
                printf(" N |"); // pion noir 
            } else if (p->cases[i][j]==BLANC) {
                printf(" B |"); // pion blanc 
            }
        }
        printf("\n   +---+---+---+---+---+---+---+---+\n");
    }
    printf("\n Score --> NOIR: %d | BLANC: %d\n", p->scoreNoir, p->scoreBlanc);
    printf(" Tour du joueur: %s\n", (p->joueurActuel == NOIR)? "NOIR":"BLANC");
}

Coup lireCoup() {
    Coup c;
    char col;
    int ligne;
    printf("Entrez le coup (synthaxe ex: D3)");
    scanf(" %c%d", &col, &ligne);
    c.colonne = col - 'A'; // Convertir lettre en ind de colonne
    c.ligne = ligne - 1; // Convertir chiffre en ind de ligne
    return c;
}

