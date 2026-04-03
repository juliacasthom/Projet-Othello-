#include "othello.h"
#include <stdio.h>

int main() {
    Plateau p;

    // Initialisation du plateau (ton raisonnement)
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            p.cases[i][j] = VIDE;
        }
    }

    // Pions de départ 
    p.cases[3][3] = BLANC;
    p.cases[4][4] = BLANC;
    p.cases[3][4] = NOIR;
    p.cases[4][3] = NOIR;

    p.scoreNoir = 2;
    p.scoreBlanc = 2;
    p.joueurActuel = NOIR;

    while(1) {
        afficher_Plateau(&p);
        
        // --- Vérification si le joueur actuel peut jouer ---
        int coupsPossibles = 0;
        for (int i=0; i<SIZE; i++) {
            for (int j=0; j<SIZE; j++) {
                Coup test; 
                test.ligne = i; 
                test.colonne = j;
                if (coupValide(&p, test)) { // On utilise le nom de ton interface.c
                    coupsPossibles = 1;
                    break;
                }
            }
            if(coupsPossibles) break;
        }

        if (!coupsPossibles) {
            // On vérifie si l'AUTRE peut jouer avant d'arrêter
            p.joueurActuel = -p.joueurActuel; 
            
            int autrePeutJouer = 0;
            for (int i=0; i<SIZE; i++) {
                for (int j=0; j<SIZE; j++) {
                    Coup test; test.ligne = i; test.colonne = j;
                    if (coupValide(&p, test)) {
                        autrePeutJouer = 1;
                        break;
                    }
                }
            }

            if (!autrePeutJouer) {
                printf("\nPlus aucun coup possible. Partie terminee.\n");
                if (p.scoreNoir > p.scoreBlanc) printf("Les pions NOIR gagnent !\n");
                else if (p.scoreBlanc > p.scoreNoir) printf("Les pions BLANC gagnent !\n");
                else printf("Match nul !\n");
                break; // SORTIE DU WHILE(1)
            } else {
                printf("Le joueur n'a pas de coup, il passe son tour.\n");
                continue; 
            }
        }

        // --- Lecture et exécution du coup ---
        Coup c = lireCoup();

        if (coupValide(&p, c)) {
            jouerCoup(&p, c);
            changerJoueur(&p);
        } else {
            printf("Coup invalide, essayez a nouveau.\n");
            continue;
        }
    }

    return 0;
}