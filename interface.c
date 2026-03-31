#include "othello.h"
#include <stdio.h>
#include <ctype.h>

 
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

int coupValide(Plateau *p, Coup c){
    if (c.ligne<0 || c.ligne>=SIZE || c.colonne<0 || c.colonne>=SIZE)
        return 0; // coup hors plateau 
    
    if (p->cases[c.ligne][c.colonne]!=VIDE) 
        return 0;

    int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (int i=0; i<8; i++)
        if (captureDirection(p, c, directions[i][0], directions[i][1]))
        return 1;

    return 0;
}

void retournerDirection(Plateau *p, Coup c, int dx, int dy) {
    if (!captureDirection(p, c, dx, dy))
        return; // peut etre a modifier, à voir si ok ou non de rien renvoyer
    int x= c.ligne+dx;
    int y= c.colonne+dy;
    int adversaire=-p-> joueurActuel;

    while (p->cases[x][y]==adversaire) {
        p->cases[x][y]=p->joueurActuel;
        x+=dx;
        y+=dy;
    }
}

void jouerCoup(Plateau *p, Coup c) {
    p->cases[c.ligne][c.colonne]= p-> joueurActuel;
    int directions[8][2]= {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for (int i=0; i<8; i++)
        retournerDirection(p, c, directions[i][0], directions[i][1]);

    // maj score
    int noir=0;
    int blanc=0;
    for (int i=0; i<SIZE; i++) 
        for (int j=0; j<SIZE; j++) {
            if (p->cases[i][j]==NOIR)
                noir++;
            else if (p->cases[i][j]==BLANC)
                blanc++;
        }
    p-> scoreNoir=noir;
    p-> scoreBlanc=blanc;
}
void changerJoueur(Plateau *p){
    p-> joueurActuel = -p->joueurActuel;
}