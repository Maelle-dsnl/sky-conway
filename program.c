#include <stdio.h>

#define HAUTEUR 10
#define LARGEUR 12

#define VIDE 0
#define JOUEUR1 1
#define JOUEUR2 2
#define NB_JOUEURS 2

#define TAILLE_ZONE 3
#define ACTIONS_PAR_TOUR 2
#define DISTANCE_MAX 2
#define RESERVE_DEPART 6

#define POINTS_ATTERRISSAGE 5
#define POINTS_NAISSANCE 2
#define SCORE_VICTOIRE 20
#define FLOTTE_MIN 2
#define MANCHES_MAX 30

#define TAILLE_CARRE 2
#define LIGNE_CARRE_J1 1
#define COLONNE_CARRE_J1 2
#define LIGNE_CARRE_J2 7
#define COLONNE_CARRE_J2 8

#define SYMB_CIEL '.'
#define SYMB_GRELE '#'

const char SYMBOLES[NB_JOUEURS + 2] = " FL";

const char COMPAGNIES[NB_JOUEURS + 1][12] = {
    "",
    "Air France",
    "Aer Lingus"
};

const char CARTE[HAUTEUR][LARGEUR + 1] = {
    "............",
    "............",
    "............",
    "............",
    "..##....##..",
    "..##....##..",
    "............",
    "............",
    "............",
    "............"
};

void initialiserPartie(int grille[HAUTEUR][LARGEUR], int scores[], int reserve[]);
int estDansZone(int joueur, int ligne);
int ligneArrivee(int joueur);
void afficherGrille(int grille[HAUTEUR][LARGEUR]);


void initialiserPartie(int grille[HAUTEUR][LARGEUR], int scores[], int reserve[])
{
    for (int l = 0; l < HAUTEUR; l++) {
        for (int c = 0; c < LARGEUR; c++) {
            grille[l][c] = VIDE;
        }
    }

    for (int j = 0; j <= NB_JOUEURS; j++) {
        scores[j] = 0;
        reserve[j] = RESERVE_DEPART;
    }

    for (int l = 0; l < TAILLE_CARRE; l++) {
        for (int c = 0; c < TAILLE_CARRE; c++) {
            grille[LIGNE_CARRE_J1 + l][COLONNE_CARRE_J1 + c] = JOUEUR1;
            grille[LIGNE_CARRE_J2 + l][COLONNE_CARRE_J2 + c] = JOUEUR2;
        }
    }
}

int estDansZone(int joueur, int ligne)
{
    if (joueur == JOUEUR1) {
        return ligne < TAILLE_ZONE;
    }
    return ligne >= HAUTEUR - TAILLE_ZONE;
}

int ligneArrivee(int joueur)
{
    if (joueur == JOUEUR1) {
        return HAUTEUR - 1;
    }
    return 0;
}

void afficherGrille(int grille[HAUTEUR][LARGEUR])
{
    printf("\n     ");
    for (int c = 0; c < LARGEUR; c++) {
        printf("%2d ", c);
    }
    printf("\n");

    for (int l = 0; l < HAUTEUR; l++) {
        printf("%3d   ", l);

        for (int c = 0; c < LARGEUR; c++) {
            if (grille[l][c] != VIDE) {
                printf("%c  ", SYMBOLES[grille[l][c]]);
            } else {
                printf("%c  ", CARTE[l][c]);
            }
        }

        for (int j = JOUEUR1; j <= JOUEUR2; j++) {
            if (estDansZone(j, l)) {
                printf("  zone %c", SYMBOLES[j]);
            }
        }
        for (int j = JOUEUR1; j <= JOUEUR2; j++) {
            if (ligneArrivee(j) == l) {
                printf("  | arrivee %c", SYMBOLES[j]);
            }
        }
        printf("\n");
    }
}

int main()
{
    int grille[HAUTEUR][LARGEUR];
    int scores[NB_JOUEURS + 1];
    int reserve[NB_JOUEURS + 1];

    initialiserPartie(grille, scores, reserve);
    afficherGrille(grille);

    return 0;
}
