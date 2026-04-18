#include "othello.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>

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


void sauvegarder_partie(Plateau p, GameMode m, const char* n1, const char* n2) {
    Sauvegarde s;
    s.plateau = p;
    s.mode = m;
    strcpy(s.nomJ1, n1);
    strcpy(s.nomJ2, n2);

    FILE *f = fopen("save.bin", "wb"); // !!! wb = write binary
    if (f != NULL) {
        fwrite(&s, sizeof(Sauvegarde), 1, f);
        fclose(f);
    }
}

bool charger_partie(Plateau *p, GameMode *m, char* n1, char* n2) {
    Sauvegarde s;
    FILE *f = fopen("save.bin", "rb"); // !!! rb = read binary
    if (f == NULL) return false;

    fread(&s, sizeof(Sauvegarde), 1, f);
    fclose(f);

    *p = s.plateau;
    *m = s.mode;
    strcpy(n1, s.nomJ1);
    strcpy(n2, s.nomJ2);
    return true;
}