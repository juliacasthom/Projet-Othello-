
#define VIDE 0
#define NOIR 1
#define BLANC -1
#define SIZE 8

typedef struct{
    int cases[SIZE][SIZE]; 
    int joueurActuel; 
    int scoreNoir; 
    int scoreBlanc;
}Plateau; 

typedef struct {
    int x;
    int y;
} Coup;

void afficher_Plateau(Plateau *p);
