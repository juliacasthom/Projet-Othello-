#include "include/raylib.h"
#include "othello.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 600
#define CELL_SIZE (SCREEN_WIDTH / 8)

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

    // Vérifier si une sauvegarde existe au lancement
    bool sauvegardeExiste = FileExists("save.bin");

    // Boutons accueil
    int btnW = 240, btnH = 55, btnX = SCREEN_WIDTH/2 - btnW/2;
    // Initialisation des rectangles, qui seront mis à jour dans la boucle
    Rectangle boutonContinuer = { btnX, 0, btnW, btnH }; 
    Rectangle boutonPVP       = { btnX, 0, btnW, btnH };
    Rectangle boutonPVI       = { btnX, 0, btnW, btnH };

    // --- Noms des joueurs ---
    char nomJ1[32] = "";
    char nomJ2[32] = "";
    int  champActif = 0; // 0 = champ J1, 1 = champ J2

    // Pop-up saisie noms (PVP)
    Rectangle popupNoms     = { SCREEN_WIDTH/2 - 175, 160, 350, 300 };
    Rectangle champNomJ1    = { popupNoms.x + 25, popupNoms.y + 90,  300, 40 };
    Rectangle champNomJ2    = { popupNoms.x + 25, popupNoms.y + 185, 300, 40 };
    Rectangle btnValiderNoms = { popupNoms.x + 75, popupNoms.y + 245, 200, 40 };

    // --- Variables pour le MENU POP-UP ---
    bool menuOuvert = false;
    Rectangle btnOuvrirMenu = { SCREEN_WIDTH - 110, SCREEN_WIDTH + 30, 90, 40 };

    // --- Bouton "?" pour afficher/masquer les coups possibles ---
    bool afficherCoups = false;
    Rectangle btnAideCoups = { SCREEN_WIDTH - 155, SCREEN_WIDTH + 30, 35, 40 };

    Rectangle popupFenetre = { SCREEN_WIDTH/2 - 125, 150, 250, 300 };
    Rectangle btnRejouer   = { popupFenetre.x + 25, popupFenetre.y + 70,  200, 50 };
    Rectangle btnRetour    = { popupFenetre.x + 25, popupFenetre.y + 140, 200, 50 };
    Rectangle btnQuitter   = { popupFenetre.x + 25, popupFenetre.y + 210, 200, 50 };
    Rectangle btnFermer    = { popupFenetre.x + 10, popupFenetre.y + 10,  30,  30 };

    // --- Pop-up confirmation quitter ---
    bool confirmQuitterOuvert = false;
    Rectangle popupConfirm  = { SCREEN_WIDTH/2 - 150, SCREEN_WIDTH/2 - 80, 300, 160 };
    Rectangle btnConfirmOui = { popupConfirm.x + 20,  popupConfirm.y + 100, 110, 40 };
    Rectangle btnConfirmNon = { popupConfirm.x + 170, popupConfirm.y + 100, 110, 40 };

    Plateau p;
    bool partieTerminee = false;

    while (!WindowShouldClose()) {
        Vector2 souris = GetMousePosition();

        bool sauvegardeExiste = FileExists("save.bin");

        if (sauvegardeExiste) {
            boutonContinuer.y = 320; 
            boutonPVP.y       = 390; 
            boutonPVI.y       = 460;
        } else {
            boutonPVP.y       = 340; 
            boutonPVI.y       = 420;
        }

        // ============================
        //  ECRAN ACCUEIL
        // ============================
        if (ecran == SCREEN_ACCUEIL) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // LOGIQUE CONTINUER
                if (sauvegardeExiste && CheckCollisionPointRec(souris, boutonContinuer)) {
                    if (charger_partie(&p, &mode, nomJ1, nomJ2)) {
                        ecran = SCREEN_JEU;
                        partieTerminee = false;
                        menuOuvert = false;
                    }
                }
                // LOGIQUE PVP
                if (CheckCollisionPointRec(souris, boutonPVP)) {
                    // On passe à la saisie des noms
                    nomJ1[0] = '\0'; nomJ2[0] = '\0';
                    champActif = 0;
                    mode = MODE_PVP;
                    ecran = SCREEN_SAISIE_NOMS;
                }
                if (CheckCollisionPointRec(souris, boutonPVI)) {
                    // Pas de saisie pour PVI
                    strcpy(nomJ1, "Joueur");
                    strcpy(nomJ2, "IA");
                    mode = MODE_PVI;
                    init_plateau(&p);
                    partieTerminee = false;
                    afficherCoups = false;
                    ecran = SCREEN_JEU;
                }
            }

            BeginDrawing();
            ClearBackground(WOOD_COLOR);

            // 1. Titre et Sous-titre
            int titreTaillle = 80;
            int titreW = MeasureText("OTHELLO", titreTaillle);
            DrawText("OTHELLO", SCREEN_WIDTH/2 - titreW/2, 70, titreTaillle, DARK_PINK);
            int sousTitreW = MeasureText("Rose Edition", 28);
            DrawText("Rose Edition", SCREEN_WIDTH/2 - sousTitreW/2, 158, 28, BORDER_COLOR);

            // 2. Pions décoratifs
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

            // 3. Bouton CONTINUER
            if (sauvegardeExiste) {
                bool h = CheckCollisionPointRec(souris, boutonContinuer);
                DrawRectangleRec(boutonContinuer, h ? DARK_PINK : LIGHT_PINK);
                DrawRectangleLinesEx(boutonContinuer, 2, BORDER_COLOR);
                DrawText("Continuer", boutonContinuer.x + 65, boutonContinuer.y + 15, 22, BORDER_COLOR);
            }

            // 4. Bouton PVP
            bool survolPVP = CheckCollisionPointRec(souris, boutonPVP);
            DrawRectangleRec(boutonPVP, survolPVP ? DARK_PINK : LIGHT_PINK);
            DrawRectangleLinesEx(boutonPVP, 2, BORDER_COLOR);
            DrawText("Joueur VS Joueur", boutonPVP.x + btnW/2 - MeasureText("Joueur VS Joueur", 22)/2, boutonPVP.y + btnH/2 - 11, 22, survolPVP ? WHITE : BORDER_COLOR);

            // 5. Bouton PVI
            bool survolPVI = CheckCollisionPointRec(souris, boutonPVI);
            DrawRectangleRec(boutonPVI, survolPVI ? DARK_PINK : LIGHT_PINK);
            DrawRectangleLinesEx(boutonPVI, 2, BORDER_COLOR);
            DrawText("Joueur VS IA", boutonPVI.x + btnW/2 - MeasureText("Joueur VS IA", 22)/2, boutonPVI.y + btnH/2 - 11, 22, survolPVI ? WHITE : BORDER_COLOR);

            // 6. Crédits
            int creditsW = MeasureText("L3 Informatique - Projet Othello", 18);
            DrawText("L3 Informatique - Projet Othello", SCREEN_WIDTH/2 - creditsW/2, 540, 18, BORDER_COLOR);
            
            EndDrawing();
        }

        // ============================
        //  ECRAN SAISIE NOMS (PVP)
        // ============================
        else if (ecran == SCREEN_SAISIE_NOMS) {

            // Sélection du champ actif au clic
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(souris, champNomJ1))  champActif = 0;
                else if (CheckCollisionPointRec(souris, champNomJ2)) champActif = 1;

                // Valider
                if (CheckCollisionPointRec(souris, btnValiderNoms)) {
                    // Noms par défaut si vides
                    if (strlen(nomJ1) == 0) strcpy(nomJ1, "Joueur 1 ");
                    if (strlen(nomJ2) == 0) strcpy(nomJ2, "Joueur 2 ");
                    init_plateau(&p);
                    partieTerminee = false;
                    afficherCoups = false;
                    ecran = SCREEN_JEU;
                }
            }

            // Saisie clavier
            int key = GetCharPressed();
            while (key > 0) {
                char *nom = (champActif == 0) ? nomJ1 : nomJ2;
                int len = strlen(nom);
                if (key >= 32 && key <= 125 && len < 20) {
                    nom[len]   = (char)key;
                    nom[len+1] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                char *nom = (champActif == 0) ? nomJ1 : nomJ2;
                int len = strlen(nom);
                if (len > 0) nom[len-1] = '\0';
            }
            if (IsKeyPressed(KEY_TAB)) champActif = 1 - champActif;
            if (IsKeyPressed(KEY_ENTER)) {
                if (champActif == 0) champActif = 1;
                else {
                    if (strlen(nomJ1) == 0) strcpy(nomJ1, "Joueur 1 ");
                    if (strlen(nomJ2) == 0) strcpy(nomJ2, "Joueur 2 ");
                    init_plateau(&p);
                    partieTerminee = false;
                    afficherCoups = false;
                    ecran = SCREEN_JEU;
                }
            }

            BeginDrawing();
            ClearBackground(WOOD_COLOR);

            // Fond sombre derrière le popup
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH + 100, Fade(BLACK, 0.35f));

            // Fenêtre popup
            DrawRectangleRec(popupNoms, WOOD_COLOR);
            DrawRectangleLinesEx(popupNoms, 3, BORDER_COLOR);

            int titrePopW = MeasureText("Noms des joueurs", 26);
            DrawText("Noms des joueurs", popupNoms.x + (popupNoms.width - titrePopW)/2, popupNoms.y + 20, 26, BORDER_COLOR);

            // Champ Joueur 1
            DrawText("Joueur Rose Foncé :", popupNoms.x + 25, champNomJ1.y - 22, 17, BORDER_COLOR);
            DrawRectangleRec(champNomJ1, WHITE);
            DrawRectangleLinesEx(champNomJ1, 2, (champActif == 0) ? DARK_PINK : BORDER_COLOR);
            DrawText(nomJ1, champNomJ1.x + 8, champNomJ1.y + 10, 20, BORDER_COLOR);
            // Curseur clignotant
            if (champActif == 0 && ((int)(GetTime() * 2) % 2 == 0)) {
                int cursorX = champNomJ1.x + 8 + MeasureText(nomJ1, 20);
                DrawRectangle(cursorX, champNomJ1.y + 8, 2, 22, BORDER_COLOR);
            }

            // Champ Joueur 2
            DrawText("Joueur Rose Pale :", popupNoms.x + 25, champNomJ2.y - 22, 17, BORDER_COLOR);
            DrawRectangleRec(champNomJ2, WHITE);
            DrawRectangleLinesEx(champNomJ2, 2, (champActif == 1) ? DARK_PINK : BORDER_COLOR);
            DrawText(nomJ2, champNomJ2.x + 8, champNomJ2.y + 10, 20, BORDER_COLOR);
            if (champActif == 1 && ((int)(GetTime() * 2) % 2 == 0)) {
                int cursorX = champNomJ2.x + 8 + MeasureText(nomJ2, 20);
                DrawRectangle(cursorX, champNomJ2.y + 8, 2, 22, BORDER_COLOR);
            }

            // Bouton Valider
            bool survolValider = CheckCollisionPointRec(souris, btnValiderNoms);
            DrawRectangleRec(btnValiderNoms, survolValider ? DARK_PINK : LIGHT_PINK);
            DrawRectangleLinesEx(btnValiderNoms, 2, BORDER_COLOR);
            DrawText("Jouer !", btnValiderNoms.x + btnValiderNoms.width/2 - MeasureText("Jouer !", 20)/2, btnValiderNoms.y + 10, 20, survolValider ? WHITE : BORDER_COLOR);

            EndDrawing();
        }

        // ============================
        //  ECRAN JEU
        // ============================
        else {
            if (!menuOuvert && !confirmQuitterOuvert) {
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
                    if (!autrePeutJouer) {
                        partieTerminee = true;
                        if (FileExists("save.bin")) remove("save.bin");
                    }
                }

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(souris, btnOuvrirMenu)) {
                        menuOuvert = true;
                    }
                    // Bouton "?" toggle coups possibles
                    else if (CheckCollisionPointRec(souris, btnAideCoups)) {
                        afficherCoups = !afficherCoups;
                    }
                    else {
                        bool tourHumain = (mode == MODE_PVP) || (p.joueurActuel == NOIR);
                        if (!partieTerminee && tourHumain) {
                            int col = souris.x / CELL_SIZE;
                            int lig = souris.y / CELL_SIZE;
                            if (lig < 8 && est_coup_valide(p.cases, lig, col, p.joueurActuel)) {
                                jouer_coup(p.cases, lig, col, p.joueurActuel);
                                mettre_a_jour_scores(&p);
                                p.joueurActuel = -p.joueurActuel;
                                sauvegarder_partie(p, mode, nomJ1, nomJ2); 
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
                    sauvegarder_partie(p, mode, nomJ1, nomJ2);
                }
            }
            else if (menuOuvert && !confirmQuitterOuvert) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(souris, btnFermer))  menuOuvert = false;
                    if (CheckCollisionPointRec(souris, btnRejouer)) { init_plateau(&p); partieTerminee = false; afficherCoups = false; menuOuvert = false; }
                    if (CheckCollisionPointRec(souris, btnRetour))  { ecran = SCREEN_ACCUEIL; menuOuvert = false; }
                    if (CheckCollisionPointRec(souris, btnQuitter)) { confirmQuitterOuvert = true; }
                }
            }
            else if (confirmQuitterOuvert) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(souris, btnConfirmOui)) {
                        // 1. On sauvegarde l'état actuel
                        sauvegarder_partie(p, mode, nomJ1, nomJ2);
                        break; 
                    }
                    
                    // Bouton NON
                    if (CheckCollisionPointRec(souris, btnConfirmNon)) {
                        // 1. On supprime le fichier pour ne pas reprendre cette partie plus tard
                        if (FileExists("save.bin")) remove("save.bin");
                        break; 
                    }
                }
            }

            BeginDrawing();
            ClearBackground(WOOD_COLOR);
            int col = souris.x / CELL_SIZE;
            int lig = souris.y / CELL_SIZE;

            // Surbrillance case survolée
            if (lig >= 0 && lig < 8 && col >= 0 && col < 8) {
                Color couleurCase = (p.joueurActuel == NOIR) ? DARK_PINK : LIGHT_PINK;
                DrawRectangle(col * CELL_SIZE, lig * CELL_SIZE, CELL_SIZE, CELL_SIZE, Fade(couleurCase, 0.18f));
                DrawRectangleLinesEx((Rectangle){col*CELL_SIZE, lig*CELL_SIZE, CELL_SIZE, CELL_SIZE}, 2, couleurCase);
            }

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    DrawRectangleLines(j*CELL_SIZE, i*CELL_SIZE, CELL_SIZE, CELL_SIZE, BORDER_COLOR);
                    if (p.cases[i][j] == NOIR) {
                        DrawCircle(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, DARK_PINK);
                        DrawCircleLines(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, BLACK);
                    } else if (p.cases[i][j] == BLANC) {
                        DrawCircle(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, LIGHT_PINK);
                        DrawCircleLines(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, WHITE);
                    } else {
                        // Coups possibles — seulement si activés par "?"
                        if (afficherCoups && est_coup_valide(p.cases, i, j, p.joueurActuel)) {
                            Color couleur = (p.joueurActuel == NOIR) ? DARK_PINK : LIGHT_PINK;
                            DrawCircle(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6, WOOD_COLOR);
                            for (int k = 0; k < 4; k++)
                                DrawCircleLines(j*CELL_SIZE + CELL_SIZE/2, i*CELL_SIZE + CELL_SIZE/2, CELL_SIZE/2 - 6 + k*0.3f, couleur);
                        }
                    }
                }
            }

            // Barre inférieure
            DrawRectangle(0, SCREEN_WIDTH, SCREEN_WIDTH, 100, BORDER_COLOR);

            // Scores — avec les noms des joueurs
            DrawText(TextFormat("%s: %d", nomJ1, p.scoreNoir), 20, SCREEN_WIDTH + 20, 22, DARK_PINK);
            DrawText("  ", 20 + MeasureText(TextFormat("%s: %d", nomJ1, p.scoreNoir), 22) + 10, SCREEN_WIDTH + 20, 22, WHITE);
            DrawText(TextFormat("%s: %d", nomJ2, p.scoreBlanc), 20 + MeasureText(TextFormat("%s: %d", nomJ1, p.scoreNoir), 22) + 30, SCREEN_WIDTH + 20, 22, LIGHT_PINK);

            // Bouton "?" (toggle coups possibles)
            bool survolAide = CheckCollisionPointRec(souris, btnAideCoups);
            DrawRectangleRec(btnAideCoups, afficherCoups ? DARK_PINK : (survolAide ? LIGHT_PINK : WOOD_COLOR));
            DrawRectangleLinesEx(btnAideCoups, 2, WHITE);
            DrawText("?", btnAideCoups.x + btnAideCoups.width/2 - MeasureText("?", 22)/2, btnAideCoups.y + 8, 22, afficherCoups ? WHITE : BORDER_COLOR);

            // Bouton MENU
            bool survolBoutonMenu = CheckCollisionPointRec(souris, btnOuvrirMenu);
            DrawRectangleRec(btnOuvrirMenu, survolBoutonMenu ? DARK_PINK : LIGHT_PINK);
            DrawRectangleLinesEx(btnOuvrirMenu, 2, WHITE);
            DrawText("MENU", btnOuvrirMenu.x + 18, btnOuvrirMenu.y + 10, 20, BORDER_COLOR);

            // Tour / Fin de partie — avec noms
            if (partieTerminee) {
                DrawText("PARTIE TERMINEE", 155, SCREEN_WIDTH + 55, 20, GOLD);
                const char *nomGagnant;
                if (p.scoreNoir > p.scoreBlanc)
                    nomGagnant = TextFormat("Vainqueur : %s !", nomJ1);
                else if (p.scoreBlanc > p.scoreNoir)
                    nomGagnant = TextFormat("Vainqueur : %s !", nomJ2);
                else
                    nomGagnant = "Egalite !";
                int gagnantW = MeasureText(nomGagnant, 24);
                DrawRectangle(0, SCREEN_WIDTH/2 - 22, SCREEN_WIDTH, 44, Fade(BLACK, 0.55f));
                DrawText(nomGagnant, SCREEN_WIDTH/2 - gagnantW/2, SCREEN_WIDTH/2 - 12, 24, GOLD);
            } else {
                const char *nomTour = (p.joueurActuel == NOIR) ? nomJ1 : nomJ2;
                DrawText(TextFormat("Tour : %s", nomTour), 20, SCREEN_WIDTH + 60, 20, WHITE);
            }

            // Pop-up MENU
            if (menuOuvert) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH + 100, Fade(BLACK, 0.4f));
                DrawRectangleRec(popupFenetre, WOOD_COLOR);
                DrawRectangleLinesEx(popupFenetre, 3, BORDER_COLOR);
                DrawText("PAUSE", popupFenetre.x + 80, popupFenetre.y + 20, 30, BORDER_COLOR);
                DrawText("<--", btnFermer.x, btnFermer.y, 25, BORDER_COLOR);

                Rectangle btns[]  = {btnRejouer, btnRetour, btnQuitter};
                char *labels[] = {"Rejouer", "Menu Principal", "Quitter"};
                for (int i = 0; i < 3; i++) {
                    bool h = CheckCollisionPointRec(souris, btns[i]);
                    DrawRectangleRec(btns[i], h ? DARK_PINK : LIGHT_PINK);
                    DrawRectangleLinesEx(btns[i], 2, BORDER_COLOR);
                    DrawText(labels[i], btns[i].x + btns[i].width/2 - MeasureText(labels[i], 20)/2, btns[i].y + 15, 20, BORDER_COLOR);
                }
            }

            // Pop-up CONFIRMATION QUITTER
            if (confirmQuitterOuvert) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH + 100, Fade(BLACK, 0.55f));
                DrawRectangleRec(popupConfirm, WOOD_COLOR);
                DrawRectangleLinesEx(popupConfirm, 3, BORDER_COLOR);
                const char *msgConfirm = "Sauvegarder avant de quitter ?";
                int msgW = MeasureText(msgConfirm, 17);
                DrawText(msgConfirm, popupConfirm.x + (popupConfirm.width - msgW)/2, popupConfirm.y + 35, 17, BORDER_COLOR);

                bool survolOui = CheckCollisionPointRec(souris, btnConfirmOui);
                DrawRectangleRec(btnConfirmOui, survolOui ? DARK_PINK : LIGHT_PINK);
                DrawRectangleLinesEx(btnConfirmOui, 2, BORDER_COLOR);
                DrawText("Oui", btnConfirmOui.x + btnConfirmOui.width/2 - MeasureText("Oui", 20)/2, btnConfirmOui.y + 10, 20, BORDER_COLOR);

                bool survolNon = CheckCollisionPointRec(souris, btnConfirmNon);
                DrawRectangleRec(btnConfirmNon, survolNon ? DARK_PINK : LIGHT_PINK);
                DrawRectangleLinesEx(btnConfirmNon, 2, BORDER_COLOR);
                DrawText("Non", btnConfirmNon.x + btnConfirmNon.width/2 - MeasureText("Non", 20)/2, btnConfirmNon.y + 10, 20, BORDER_COLOR);
            }

            EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}