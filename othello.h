#ifndef OTHELLO_H
#define OTHELLO_H

#include <stdbool.h>

// --- CONSTANTES ---
#define SIZE 8
#define VIDE 0
#define NOIR 1
#define BLANC -1
#define INFINI 1000000


// --- Les ENUMS ---
typedef enum { SCREEN_ACCUEIL, SCREEN_SAISIE_NOMS, SCREEN_JEU } GameScreen;
typedef enum { MODE_PVP, MODE_PVI } GameMode;

// --- STRUCTURES ---

// Structure pour représenter un coup sur la grille
typedef struct {
    int ligne;
    int colonne;
} Coup;

// Structure regroupant l'état complet de la partie
typedef struct {
    int cases[SIZE][SIZE]; 
    int joueurActuel; 
    int scoreNoir; 
    int scoreBlanc;
} Plateau; 

// Structures pour le système de sauvegarde
typedef struct {
    Plateau plateau;
    GameMode mode;
    char nomJ1[32];
    char nomJ2[32];
} Sauvegarde;

// --- MOTEUR DE JEU (moteur.c) ---

// Vérifie si un sandwich est possible dans une direction donnée
int captureDirection(int p[8][8], int r, int c, int dr, int dc, int joueur);

// Vérifie si un coup est valide sur l'ensemble du plateau
int est_coup_valide(int p[8][8], int r, int c, int joueur);

// Applique un coup et retourne les pions capturés
void jouer_coup(int p[8][8], int r, int c, int joueur);

// Sauvegarde de partie et génération de partie enregistrée
void sauvegarder_partie(Plateau p, GameMode m, const char* n1, const char* n2);
bool charger_partie(Plateau *p, GameMode *m, char* n1, char* n2);


// --- INTELLIGENCE ARTIFICIELLE (ia.c) ---

// Fonctions de calcul de score (Heuristiques)
int evaluer_plateau(int plateau[8][8], int pionIA, int pionHumain);
int calculer_score_position(int plateau[8][8], int ia, int humain);
int calculer_stabilite_bords(int plateau[8][8], int joueur);

// Algorithmes de recherche
int minimax(int plateau[8][8], int profondeur, bool estMax, int pionIA, int pionHumain);
int alphabeta(int plateau[8][8], int profondeur, int alpha, int beta, bool estMax, int pionIA, int pionHumain);

// Fonctions de sélection du meilleur coup
Coup choisir_meilleur_coup(int plateau[8][8], int ia, int humain, int profondeur);
Coup choisir_meilleur_coup_alphabeta(int plateau[8][8], int pionIA, int pionHumain, int profondeurMax);

// Utilitaires de simulation
void copier_plateau(int source[8][8], int destination[8][8]);
int generer_coups_possibles(int plateau[8][8], int joueur, Coup liste_coups[64]);

#endif