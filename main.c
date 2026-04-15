#include "include/raylib.h"
#include "othello.h"
#include <stdio.h>

#define SCREEN_WIDTH 600
#define CELL_SIZE (SCREEN_WIDTH / 8)

typedef enum { SCREEN_ACCUEIL, SCREEN_JEU } GameScreen;
typedef enum { MODE_PVP, MODE_PVI } GameMode;

Color WOOD_COLOR   = { 222, 184, 135, 255 };
Color DARK_PINK    = { 231, 84,  128, 255 };
Color LIGHT_PINK   = { 255, 182, 193, 255 };
Color BORDER_COLOR = { 101, 67,  33,  255 };

void mettre_a_jour_scores(Plateau *p) {
    p->scoreNoir = 0; p->scoreBlanc = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (p->cases[i][j] == NOIR)  p->scoreNoir++;
            if (p->cases[i][j] == BLANC) p->scoreBlanc++;
        }
}

void init_plateau(Plateau *p) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            p->cases[i][j] = VIDE;
    p->cases[3][3] = BLANC; p->cases[4][4] = BLANC;
    p->cases[3][4] = NOIR;  p->cases[4][3] = NOIR;
    p->scoreNoir = 2; p->scoreBlanc = 2;
    p->joueurActuel = NOIR;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_WIDTH + 100, "Othello Rose - L3 Projet");
    SetTargetFPS(60);

    GameScreen ecran = SCREEN_ACCUEIL;
    GameMode   mode  = MODE_PVP;

    // Boutons accueil
    int btnW = 240, btnH = 55, btnX = SCREEN_WIDTH/2 - btnW/2;
    Rectangle boutonPVP = { btnX, 340, btnW, btnH };
    Rectangle boutonPVI = { btnX, 420, btnW, btnH };

    // --- Variables pour le MENU POP-UP ---
    bool menuOuvert = false;
    Rectangle btnOuvrirMenu = { SCREEN_WIDTH - 110, SCREEN_WIDTH + 30, 90, 40 };
    
    Rectangle popupFenetre = { SCREEN_WIDTH/2 - 125, 150, 250, 300 };
    Rectangle btnRejouer   = { popupFenetre.x + 25, popupFenetre.y + 70, 200, 50 };
    Rectangle btnRetour    = { popupFenetre.x + 25, popupFenetre.y + 140, 200, 50 };
    Rectangle btnQuitter   = { popupFenetre.x + 25, popupFenetre.y + 210, 200, 50 };
    Rectangle btnFermer    = { popupFenetre.x + 10, popupFenetre.y + 10, 30, 30 }; // Petite flèche

    Plateau p;
    bool partieTerminee = false;

    while (!WindowShouldClose()) {
        Vector2 souris = GetMousePosition();

        if (ecran == SCREEN_ACCUEIL) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(souris, boutonPVP)) {
                    mode = MODE_PVP; init_plateau(&p); partieTerminee = false; ecran = SCREEN_JEU;
                }
                if (CheckCollisionPointRec(souris, boutonPVI)) {
                    mode = MODE_PVI; init_plateau(&p); partieTerminee = false; ecran = SCREEN_JEU;
                }
            }

            BeginDrawing();
            ClearBackground(WOOD_COLOR);
            int titreTaillle = 80;
            int titreW = MeasureText("OTHELLO", titreTaillle);
            DrawText("OTHELLO", SCREEN_WIDTH/2 - titreW/2, 70, titreTaillle, DARK_PINK);
            int sousTitreW = MeasureText("Rose Edition", 28);
            DrawText("Rose Edition", SCREEN_WIDTH/2 - sousTitreW/2, 158, 28, BORDER_COLOR);

            int nbPions = 4, rayon = 28, espacement = 75;
            int totalPions = nbPions * espacement - (espacement - rayon*2);
            int startX = SCREEN_WIDTH/2 - totalPions/2 + rayon;
            for (int k = 0; k < nbPions; k++) {
                int cx = startX + k * espacement;
                Color c = (k % 2 == 0) ? DARK_PINK : LIGHT_PINK;
                Color outline = (k % 2 == 0) ? BLACK : WHITE;
                DrawCircle(cx, 265, rayon, c);
                DrawCircleLines(cx, 265, rayon, outline);
            }

            bool survolPVP = CheckCollisionPointRec(souris, boutonPVP);
            DrawRectangleRec(boutonPVP, survolPVP ? DARK_PINK : LIGHT_PINK);
            DrawRectangleLinesEx(boutonPVP, 2, BORDER_COLOR);
            DrawText("Joueur VS Joueur", boutonPVP.x + btnW/2 - MeasureText("Joueur VS Joueur", 22)/2, boutonPVP.y + btnH/2 - 11, 22, survolPVP ? WHITE : BORDER_COLOR);

            bool survolPVI = CheckCollisionPointRec(souris, boutonPVI);
            DrawRectangleRec(boutonPVI, survolPVI ? DARK_PINK : LIGHT_PINK);
            DrawRectangleLinesEx(boutonPVI, 2, BORDER_COLOR);
            DrawText("Joueur VS IA", boutonPVI.x + btnW/2 - MeasureText("Joueur VS IA", 22)/2, boutonPVI.y + btnH/2 - 11, 22, survolPVI ? WHITE : BORDER_COLOR);

            int creditsW = MeasureText("L3 Informatique - Projet Othello", 18);
            DrawText("L3 Informatique - Projet Othello", SCREEN_WIDTH/2 - creditsW/2, 510, 18, BORDER_COLOR);
            EndDrawing();
        }
        else {
            // --- LOGIQUE ÉCRAN DE JEU ---
            if (!menuOuvert) {
                int coupsPossibles = 0;
                for (int i = 0; i < 8 && !coupsPossibles; i++)
                    for (int j = 0; j < 8; j++)
                        if (est_coup_valide(p.cases, i, j, p.joueurActuel))
                            { coupsPossibles = 1; break; }

                if (!coupsPossibles && !partieTerminee) {
                    p.joueurActuel = -p.joueurActuel;
                    int autrePeutJouer = 0;
                    for (int i = 0; i < 8 && !autrePeutJouer; i++)
                        for (int j = 0; j < 8; j++)
                            if (est_coup_valide(p.cases, i, j, p.joueurActuel))
                                { autrePeutJouer = 1; break; }
                    if (!autrePeutJouer) partieTerminee = true;
                }

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Clic sur bouton MENU
                    if (CheckCollisionPointRec(souris, btnOuvrirMenu)) {
                        menuOuvert = true;
                    } 
                    // Clic sur le plateau
                    else {
                        bool tourHumain = (mode == MODE_PVP) || (p.joueurActuel == NOIR);
                        if (!partieTerminee && tourHumain) {
                            int col = souris.x / CELL_SIZE;
                            int lig = souris.y / CELL_SIZE;
                            if (lig < 8 && est_coup_valide(p.cases, lig, col, p.joueurActuel)) {
                                jouer_coup(p.cases, lig, col, p.joueurActuel);
                                mettre_a_jour_scores(&p);
                                p.joueurActuel = -p.joueurActuel;
                            }
                        }
                    }
                }
                
                // Logique IA
                if (!partieTerminee && mode == MODE_PVI && p.joueurActuel == BLANC) {
                    Coup cIA = choisir_meilleur_coup_alphabeta(p.cases, BLANC, NOIR, 6);
                    if (cIA.ligne != -1) {
                        jouer_coup(p.cases, cIA.ligne, cIA.colonne, BLANC);
                        mettre_a_jour_scores(&p);
                    }
                    p.joueurActuel = NOIR;
                }
            } 
            else { 
                // --- LOGIQUE SI MENU OUVERT ---
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(souris, btnFermer))  menuOuvert = false;
                    if (CheckCollisionPointRec(souris, btnRejouer)) { init_plateau(&p); partieTerminee = false; menuOuvert = false; }
                    if (CheckCollisionPointRec(souris, btnRetour))  { ecran = SCREEN_ACCUEIL; menuOuvert = false; }
                    if (CheckCollisionPointRec(souris, btnQuitter)) break;
                }
            }

            BeginDrawing();
            ClearBackground(WOOD_COLOR);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    DrawRectangleLines(j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE, BORDER_COLOR);
                    if (p.cases[i][j] == NOIR) {
                        DrawCircle(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, DARK_PINK);
                        DrawCircleLines(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, BLACK);
                    } else if (p.cases[i][j] == BLANC) {
                        DrawCircle(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, LIGHT_PINK);
                        DrawCircleLines(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, WHITE);
                    }
                    else {
                        // affichage coups possibles
                        if (est_coup_valide(p.cases, i, j, p.joueurActuel)) {
                            Color couleur = (p.joueurActuel == NOIR) ? DARK_PINK : LIGHT_PINK;
                            Vector2 center = {
                                j*CELL_SIZE + CELL_SIZE/2,
                                i*CELL_SIZE + CELL_SIZE/2
                            };
                            float r = CELL_SIZE/2 - 6;

                            //fond (plateau)
                            DrawCircle(center.x, center.y, r, WOOD_COLOR);

                            //contour + épais 
                            for (int k = 0; k < 4; k++) {
                                DrawCircleLines(
                                    j*CELL_SIZE + CELL_SIZE/2,
                                    i*CELL_SIZE + CELL_SIZE/2,
                                    CELL_SIZE/2 - 6 + k*0.3f,
                                    couleur
                                );
                            }
                        }
                    }
                }
            }

            DrawRectangle(0, SCREEN_WIDTH, SCREEN_WIDTH, 100, BORDER_COLOR);
            DrawText(TextFormat("ROSE FONCE: %d", p.scoreNoir), 20, SCREEN_WIDTH + 20, 25, DARK_PINK);
            DrawText(TextFormat("ROSE PALE: %d", p.scoreBlanc), SCREEN_WIDTH-350, SCREEN_WIDTH + 20, 25, LIGHT_PINK);

            // Bouton MENU à l'écran
            bool survolBoutonMenu = CheckCollisionPointRec(souris, btnOuvrirMenu);
            DrawRectangleRec(btnOuvrirMenu, survolBoutonMenu ? DARK_PINK : LIGHT_PINK);
            DrawRectangleLinesEx(btnOuvrirMenu, 2, WHITE);
            DrawText("MENU", btnOuvrirMenu.x + 18, btnOuvrirMenu.y + 10, 20, BORDER_COLOR);

            if (partieTerminee) DrawText("PARTIE TERMINEE", 180, SCREEN_WIDTH + 60, 20, GOLD);
            else DrawText(TextFormat("Tour: %s", p.joueurActuel == NOIR ? "ROSE FONCE" : (mode == MODE_PVI ? "IA" : "ROSE PALE")), 20, SCREEN_WIDTH + 60, 20, WHITE);

            // --- DESSIN DU POP-UP ---
            if (menuOuvert) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH + 100, Fade(BLACK, 0.4f)); // Fond sombre
                DrawRectangleRec(popupFenetre, WOOD_COLOR);
                DrawRectangleLinesEx(popupFenetre, 3, BORDER_COLOR);
                DrawText("PAUSE", popupFenetre.x + 80, popupFenetre.y + 20, 30, BORDER_COLOR);
                
                // Bouton flèche retour
                DrawText("<--", btnFermer.x, btnFermer.y, 25, BORDER_COLOR);

                // Boutons du menu
                Rectangle btns[] = {btnRejouer, btnRetour, btnQuitter};
                char *labels[] = {"Rejouer", "Menu Principal", "Quitter"};
                for(int i=0; i<3; i++) {
                    bool h = CheckCollisionPointRec(souris, btns[i]);
                    DrawRectangleRec(btns[i], h ? DARK_PINK : LIGHT_PINK);
                    DrawRectangleLinesEx(btns[i], 2, BORDER_COLOR);
                    DrawText(labels[i], btns[i].x + btns[i].width/2 - MeasureText(labels[i], 20)/2, btns[i].y + 15, 20, BORDER_COLOR);
                }
            }
            EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}