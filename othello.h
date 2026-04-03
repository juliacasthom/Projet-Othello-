#ifndef OTHELLO_H
#define OTHELLO_H

#include <stdbool.h>

#define SIZE 8
#define VIDE 0
#define NOIR 1
#define BLANC -1

typedef struct {
    int cases[SIZE][SIZE]; 
    int joueurActuel; 
    int scoreNoir; 
    int scoreBlanc;
} Plateau; 

typedef struct {
    int ligne;
    int colonne;
} Coup;

// Fonctions d'affichage et de lecture (Interface)
void afficher_Plateau(Plateau *p);
Coup lireCoup();

// --- LOGIQUE DE JEU (Utilisée par le Main et l'IA) ---
// Version pour le tableau int[8][8] (utilisée par l'IA)
int est_coup_valide(int p[8][8], int r, int c, int joueur);
void executer_coup(int p[8][8], int r, int c, int joueur);

// Version pour la structure Plateau (utilisée par le Main)
int coupValide(Plateau *p, Coup c);
void jouerCoup(Plateau *p, Coup c);
void changerJoueur(Plateau *p);

#endif