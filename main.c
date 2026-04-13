#include "include/raylib.h"
#include "othello.h"
#include <stdio.h>
#include "moteur.h"

#define SCREEN_WIDTH 600
#define CELL_SIZE (SCREEN_WIDTH / 8) // 600/8 = 75 pixels par case

// --- DÉFINITION DES COULEURS PERSONNALISÉES ---
Color WOOD_COLOR = { 222, 184, 135, 255 };  // Marron clair (Burlywood)
Color DARK_PINK  = { 231, 84, 128, 255 };  // Rose foncé
Color LIGHT_PINK = { 255, 182, 193, 255 };  // Rose pâle
Color BORDER_COLOR = { 101, 67, 33, 255 }; // Marron sombre pour la grille

void mettre_a_jour_scores(Plateau *p) {
    p->scoreNoir = 0; p->scoreBlanc = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p->cases[i][j] == NOIR) p->scoreNoir++;
            if (p->cases[i][j] == BLANC) p->scoreBlanc++;
        }
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_WIDTH + 100, "Othello Rose - L3 Projet");
    SetTargetFPS(60);

    Plateau p;
    // Initialisation du plateau (8x8 = 64 cases)
    for(int i = 0; i < 8; i++) 
        for(int j = 0; j < 8; j++) 
            p.cases[i][j] = VIDE;

    // Pions de départ
    p.cases[3][3] = BLANC; p.cases[4][4] = BLANC;
    p.cases[3][4] = NOIR;  p.cases[4][3] = NOIR;

    p.scoreNoir = 2; p.scoreBlanc = 2;
    p.joueurActuel = NOIR;
    bool partieTerminee = false;

    while (!WindowShouldClose()) {
        
        // --- LOGIQUE : VÉRIFICATION SI LE JOUEUR PEUT JOUER ---
        int coupsPossibles = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (est_coup_valide(p.cases, i, j, p.joueurActuel)) {
                    coupsPossibles = 1; break;
                }
            }
            if (coupsPossibles) break;
        }

        if (!coupsPossibles && !partieTerminee) {
            p.joueurActuel = -p.joueurActuel; 
            int autrePeutJouer = 0;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (est_coup_valide(p.cases, i, j, p.joueurActuel)) {
                        autrePeutJouer = 1; break;
                    }
                }
            }
            if (!autrePeutJouer) partieTerminee = true;
        }

        // --- GESTION DU CLIC ---
        if (!partieTerminee && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int col = GetMouseX() / CELL_SIZE;
            int lig = GetMouseY() / CELL_SIZE;

            if (est_coup_valide(p.cases, lig, col, p.joueurActuel)) {
                jouer_coup(p.cases, lig, col, p.joueurActuel);
                mettre_a_jour_scores(&p);
                p.joueurActuel = -p.joueurActuel;
            }
        }

        // --- DESSIN (INTERFACE GRAPHIQUE) ---
        BeginDrawing();
            ClearBackground(WOOD_COLOR); // Fond en bois clair

            // 1. Dessiner les 64 cases (8x8)
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    // Trace les bordures des cases
                    DrawRectangleLines(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, BORDER_COLOR);
                    
                    // 2. Dessiner les pions
                    if (p.cases[i][j] == NOIR) {
                        // Rose foncé avec relief
                        DrawCircle(j * CELL_SIZE + CELL_SIZE/2, i * CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, DARK_PINK);
                        DrawCircleLines(j * CELL_SIZE + CELL_SIZE/2, i * CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, BLACK);
                    } 
                    else if (p.cases[i][j] == BLANC) {
                        // Rose pâle avec relief
                        DrawCircle(j * CELL_SIZE + CELL_SIZE/2, i * CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, LIGHT_PINK);
                        DrawCircleLines(j * CELL_SIZE + CELL_SIZE/2, i * CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, WHITE);
                    }
                }
            }

            // 3. Barre de score en bas
            DrawRectangle(0, SCREEN_WIDTH, SCREEN_WIDTH, 100, BORDER_COLOR);
            DrawText(TextFormat("ROSE FONCE: %d", p.scoreNoir), 20, SCREEN_WIDTH + 20, 25, DARK_PINK);
            DrawText(TextFormat("ROSE PALE: %d", p.scoreBlanc), SCREEN_WIDTH - 200, SCREEN_WIDTH + 20, 25, LIGHT_PINK);
            
            if (partieTerminee) {
                DrawText("PARTIE TERMINEE", 180, SCREEN_WIDTH + 60, 20, GOLD);
            } else {
                DrawText(TextFormat("Tour: %s", (p.joueurActuel == NOIR) ? "ROSE FONCE" : "ROSE PALE"), 200, SCREEN_WIDTH + 60, 20, WHITE);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}