#include "othello.h"
#include <stdio.h>

// --- LOGIQUE PURE (Tableau 8x8) ---

/*int captureDirection(int p[8][8], int r, int c, int dx, int dy, int joueur) {
    int x = r + dx;
    int y = c + dy;
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || p[x][y] != -joueur) return 0;
    x += dx; y += dy;
    while (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        if (p[x][y] == VIDE) return 0;
        if (p[x][y] == joueur) return 1;
        x += dx; y += dy;
    }
    return 0;
}*/

/*int est_coup_valide(int p[8][8], int r, int c, int joueur) {
    if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || p[r][c] != VIDE) return 0;
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    for (int i = 0; i < 8; i++) 
        if (captureDirection(p, r, c, dirs[i][0], dirs[i][1], joueur)) return 1;
    return 0;
}*/

void executer_coup(int p[8][8], int r, int c, int joueur) {
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    p[r][c] = joueur;
    for (int i = 0; i < 8; i++) {
        if (captureDirection(p, r, c, dirs[i][0], dirs[i][1], joueur)) {
            int x = r + dirs[i][0], y = c + dirs[i][1];
            while (p[x][y] == -joueur) {
                p[x][y] = joueur;
                x += dirs[i][0]; y += dirs[i][1];
            }
        }
    }
}

// --- WRAPPERS POUR LE MAIN (Structure Plateau) ---

int coupValide(Plateau *p, Coup c) {
    return est_coup_valide(p->cases, c.ligne, c.colonne, p->joueurActuel);
}

void jouerCoup(Plateau *p, Coup c) {
    executer_coup(p->cases, c.ligne, c.colonne, p->joueurActuel);
    // Mise à jour des scores
    p->scoreNoir = 0; p->scoreBlanc = 0;
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            if(p->cases[i][j] == NOIR) p->scoreNoir++;
            if(p->cases[i][j] == BLANC) p->scoreBlanc++;
        }
    }
}

void changerJoueur(Plateau *p) { p->joueurActuel = -p->joueurActuel; }

// --- AFFICHAGE ---

void afficher_Plateau(Plateau *p) {
    printf("\n    A B C D E F G H\n  +-----------------+\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d | ", i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (p->cases[i][j] == VIDE) printf(". ");
            else printf("%c ", p->cases[i][j] == NOIR ? 'N' : 'B');
        }
        printf("|\n");
    }
    printf("  +-----------------+\nScore N:%d B:%d | Tour: %s\n", p->scoreNoir, p->scoreBlanc, p->joueurActuel==NOIR?"NOIR":"BLANC");
}

Coup lireCoup() {
    Coup c; char col; int lig;
    printf("Coup (ex D3): ");
    scanf(" %c%d", &col, &lig);
    c.colonne = (col >= 'a') ? col - 'a' : col - 'A';
    c.ligne = lig - 1;
    return c;
}