
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

typedef struct{
    int ligne;
    int colonne;
}Coup;

void afficher_Plateau(Plateau *p);
Coup lireCoup();
int coupValide(Plateau *p, Coup c);
int captureDirection(Plateau *p, Coup c, int dx, int dy);
void retournerDirection(Plateau *p, Coup c, int dx, int dy);
void jouerCoup(Plateau *p, Coup c);
void changerJoueur(Plateau *p);