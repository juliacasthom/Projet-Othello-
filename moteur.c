#include "othello.h"
#include "moteur.h"

int captureDirection(int p[8][8], int r, int c, int dx, int dy, int joueur) {
    int x = r + dx;
    int y = c + dy;
    int adversaire = -joueur;

    if (x<0 || x>=SIZE || y<0 || y>=SIZE || p[x][y] != adversaire) return 0;
    
    x += dx; y += dy;
    while (x>=0 && x<SIZE && y>=0 && y<SIZE) {
        if (p[x][y] == VIDE) return 0;
        if (p[x][y] == joueur) return 1;
        x += dx; y += dy;
    }
    return 0; 
}

int est_coup_valide(int p[8][8], int r, int c, int joueur) {
    if (r<0 || r>=SIZE || c<0 || c>=SIZE || p[r][c] != VIDE) return 0;
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    for (int i=0; i<8; i++)
        if (captureDirection(p, r, c, dirs[i][0], dirs[i][1], joueur)) return 1;
    return 0;
}

void jouer_coup(int p[8][8], int r, int c, int joueur) {
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    p[r][c] = joueur;
    for (int i=0; i<8; i++) {
        if (captureDirection(p, r, c, dirs[i][0], dirs[i][1], joueur)) {
            int x = r + dirs[i][0], y = c + dirs[i][1];
            while (p[x][y] == -joueur) {
                p[x][y] = joueur;
                x += dirs[i][0]; y += dirs[i][1];
            }
        }
    }
}

void calculer_scores(Plateau *p) {
    p->scoreNoir = 0; p->scoreBlanc = 0;
    for(int i=0; i<SIZE; i++)
        for(int j=0; j<SIZE; j++) {
            if(p->cases[i][j] == NOIR) p->scoreNoir++;
            else if(p->cases[i][j] == BLANC) p->scoreBlanc++;
        }
}