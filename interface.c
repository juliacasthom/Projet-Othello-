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

int coupValide(Plateau *p, Coup c){
    if (c.ligne<0 || c.ligne>=SIZE || c.colonne<0 || c.colonne>=SIZE) {
        return 0; // coup hors plateau 
    }
    if (p->cases[c.ligne][c.colonne]!=VIDE) {
        return 0;
    }
    return 1;
}

int captureDirection(Plateau *p, Coup c, int dx, int dy) {
    int x = c.ligne + dx;
    int y = c.colonne + dy;
    int adversaire = -p->joueurActuel;

    // verif si au moins 1 pion adv a coté
    if (x<0 || x>=SIZE || y<0 || y>=SIZE || p->cases[x][y]!=adversaire)
        return 0; // pas de capture possible dans cette direction 
    
    if (p->cases[x][y]!=adversaire) 
        return 0;
    
    x += dx;
    y += dy;

    while (x>=0 && x<SIZE && y>=0 && y<SIZE) {
        if (p->cases[x][y]==VIDE)
            return 0; // pas de capture possible dans cette direction 
        
        if (p->cases[x][y]==p->joueurActuel) 
            return 1; // capture possible dans cette direction 
        
        x += dx;
        y += dy;
    }
    return 0; 
}