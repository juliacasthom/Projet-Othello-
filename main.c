#include "othello.h"
#include <stdio.h>

int main(){
    Plateau p;

    for(int i=0; i<SIZE; i++){ // remplir quelques cases pour le test 
        for(int j=0; j<SIZE; j++) {
            p.cases[i][j]=VIDE;
        }
    }

    // pions de départ 
    p.cases[3][3]=BLANC;
    p.cases[4][4]=BLANC;
    p.cases[3][4]=NOIR;
    p.cases[4][3]=NOIR;

    p.scoreNoir=2;
    p.scoreBlanc=2;
    p.joueurActuel=NOIR;

    afficher_Plateau(&p); 

    // test lecture coup
    Coup c= lireCoup();
    if (coupValide(&p, c)){
        printf("Coup valide\n");
    } else {
        printf("Coup invalide\n");
    }

    return 0;
}