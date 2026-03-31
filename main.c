#include "othello.h"
#include <stdio.h>

int main(){
    Plateau p;

    for(int i=0; i<SIZE; i++) // remplir quelques cases pour le test 
        for(int j=0; j<SIZE; j++) 
            p.cases[i][j]=VIDE;
        
    

    // pions de départ 
    p.cases[3][3]=BLANC;
    p.cases[4][4]=BLANC;
    p.cases[3][4]=NOIR;
    p.cases[4][3]=NOIR;

    p.scoreNoir=2;
    p.scoreBlanc=2;
    p.joueurActuel=NOIR;

    while(1) {
        afficher_Plateau(&p);
        Coup c = lireCoup();

        if (coupValide(&p, c)) {
            jouerCoup(&p, c);
            changerJoueur(&p);
        } else {
            printf("Coup invalide, essayez à nouveau.\n");
            continue;
        }
        // vérif s'il reste coups possibles
        int coupsPossibles=0;
        for (int i=0; i<SIZE; i++)
            for (int j=0; j<SIZE; j++) {
                Coup test={i,j};
                if (coupValide(&p, test)) 
                    coupsPossibles=1;   
            }
    
        if (!coupsPossibles) {
            printf("\nPartie terminée\n");
            afficher_Plateau(&p);
            if (p.scoreNoir > p.scoreBlanc) 
                printf("Les pions NOIR gagnent\n");
            else if (p.scoreBlanc > p.scoreNoir)
                printf("Les pions BLANC gagnent\n");
            else
                printf("Match nul\n");
        }    
    }

    return 0;
}