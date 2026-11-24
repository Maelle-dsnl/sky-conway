#include <stdio.h>
#include <string.h>

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
#define PREMIERE_MANCHE 1

#define TAILLE_CARRE 2
#define LIGNE_CARRE_J1 1
#define COLONNE_CARRE_J1 2
#define LIGNE_CARRE_J2 7
#define COLONNE_CARRE_J2 8

#define VOISINAGE 1
#define VOISINS_NAISSANCE 3
#define SURVIE_MIN 2
#define SURVIE_MAX 3

#define SAISIE_INVALIDE -1
#define CHIFFRES_MAX 3
#define BASE_DECIMALE 10
#define TAILLE_SAISIE 20

#define PASSER 0
#define DEPLACER 1
#define POSER 2

#define DIRECTION_MIN 1
#define DIRECTION_MAX 9
#define DIRECTION_CENTRE 5

#define ACTION_OK 0
#define ERREUR_PAS_TON_AVION 1
#define ERREUR_HORS_GRILLE 2
#define ERREUR_CASE_OCCUPEE 3

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
int compterAvionsEnVol(int grille[HAUTEUR][LARGEUR], int joueur);
void afficherScores(int grille[HAUTEUR][LARGEUR], int scores[], int reserve[], int manche);
int estDansGrille(int ligne, int colonne);
int compterVoisins(int grille[HAUTEUR][LARGEUR], int ligne, int colonne);
int proprietaireNaissance(int grille[HAUTEUR][LARGEUR], int ligne, int colonne);
void copierGrille(int source[HAUTEUR][LARGEUR], int destination[HAUTEUR][LARGEUR]);
void evoluerGrille(int grille[HAUTEUR][LARGEUR], int scores[]);
int texteEnEntier(char texte[]);
int lireEntier(char question[], int minimum, int maximum);
int demanderAction(int joueur, int numeroAction);
int demanderLigne();
int demanderColonne();
int demanderDirection();
int demanderDistance();
int directionLigne(int direction);
int directionColonne(int direction);
int deplacerAvion(int grille[HAUTEUR][LARGEUR], int joueur, int ligne, int colonne,
                  int direction, int distance);


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

int compterAvionsEnVol(int grille[HAUTEUR][LARGEUR], int joueur)
{
    int total = 0;

    for (int l = 0; l < HAUTEUR; l++) {
        for (int c = 0; c < LARGEUR; c++) {
            if (grille[l][c] == joueur) {
                total++;
            }
        }
    }
    return total;
}

void afficherScores(int grille[HAUTEUR][LARGEUR], int scores[], int reserve[], int manche)
{
    printf("\n  Manche %d / %d\n", manche, MANCHES_MAX);

    for (int j = JOUEUR1; j <= JOUEUR2; j++) {
        printf("  %-10s (%c) : %2d / %d points   reserve %d   avions en vol %d\n",
               COMPAGNIES[j], SYMBOLES[j], scores[j], SCORE_VICTOIRE,
               reserve[j], compterAvionsEnVol(grille, j));
    }
}

int estDansGrille(int ligne, int colonne)
{
    if (ligne < 0 || ligne >= HAUTEUR) {
        return 0;
    }
    if (colonne < 0 || colonne >= LARGEUR) {
        return 0;
    }
    return 1;
}

int compterVoisins(int grille[HAUTEUR][LARGEUR], int ligne, int colonne)
{
    int total = 0;

    for (int dl = -VOISINAGE; dl <= VOISINAGE; dl++) {
        for (int dc = -VOISINAGE; dc <= VOISINAGE; dc++) {
            if ((dl != 0 || dc != 0) && estDansGrille(ligne + dl, colonne + dc)
                && grille[ligne + dl][colonne + dc] != VIDE) {
                total++;
            }
        }
    }
    return total;
}

int proprietaireNaissance(int grille[HAUTEUR][LARGEUR], int ligne, int colonne)
{
    int voisinsJ1 = 0;
    int voisinsJ2 = 0;

    for (int dl = -VOISINAGE; dl <= VOISINAGE; dl++) {
        for (int dc = -VOISINAGE; dc <= VOISINAGE; dc++) {
            if ((dl != 0 || dc != 0) && estDansGrille(ligne + dl, colonne + dc)) {
                if (grille[ligne + dl][colonne + dc] == JOUEUR1) {
                    voisinsJ1++;
                } else if (grille[ligne + dl][colonne + dc] == JOUEUR2) {
                    voisinsJ2++;
                }
            }
        }
    }

    if (voisinsJ1 > voisinsJ2) {
        return JOUEUR1;
    }
    return JOUEUR2;
}

void copierGrille(int source[HAUTEUR][LARGEUR], int destination[HAUTEUR][LARGEUR])
{
    for (int l = 0; l < HAUTEUR; l++) {
        for (int c = 0; c < LARGEUR; c++) {
            destination[l][c] = source[l][c];
        }
    }
}

void evoluerGrille(int grille[HAUTEUR][LARGEUR], int scores[])
{
    int ancienne[HAUTEUR][LARGEUR];

    copierGrille(grille, ancienne);

    for (int l = 0; l < HAUTEUR; l++) {
        for (int c = 0; c < LARGEUR; c++) {
            int voisins = compterVoisins(ancienne, l, c);

            if (ancienne[l][c] == VIDE) {
                if (voisins == VOISINS_NAISSANCE) {
                    int proprietaire = proprietaireNaissance(ancienne, l, c);
                    grille[l][c] = proprietaire;
                    scores[proprietaire] = scores[proprietaire] + POINTS_NAISSANCE;
                }
            } else if (voisins < SURVIE_MIN || voisins > SURVIE_MAX) {
                grille[l][c] = VIDE;
            }
        }
    }
}

int texteEnEntier(char texte[])
{
    int nombre = 0;
    int i = 0;

    while (texte[i] != '\n' && texte[i] != '\0') {
        if (texte[i] < '0' || texte[i] > '9' || i >= CHIFFRES_MAX) {
            return SAISIE_INVALIDE;
        }
        nombre = nombre * BASE_DECIMALE + (texte[i] - '0');
        i++;
    }

    if (i == 0) {
        return SAISIE_INVALIDE;
    }
    return nombre;
}

int lireEntier(char question[], int minimum, int maximum)
{
    char texte[TAILLE_SAISIE];
    int nombre;

    do {
        printf("  %s (%d a %d) : ", question, minimum, maximum);
        fgets(texte, TAILLE_SAISIE, stdin);
        nombre = texteEnEntier(texte);

        while (texte[strlen(texte) - 1] != '\n') {
            fgets(texte, TAILLE_SAISIE, stdin);
            nombre = SAISIE_INVALIDE;
        }

        if (nombre < minimum || nombre > maximum) {
            printf("  Saisie invalide : tape un nombre entre %d et %d.\n", minimum, maximum);
        }
    } while (nombre < minimum || nombre > maximum);

    return nombre;
}

int demanderAction(int joueur, int numeroAction)
{
    printf("\n  %s (%c) - action %d / %d\n", COMPAGNIES[joueur], SYMBOLES[joueur],
           numeroAction, ACTIONS_PAR_TOUR);
    printf("    %d = Deplacer   %d = Poser   %d = Passer\n", DEPLACER, POSER, PASSER);
    return lireEntier("Ton choix", PASSER, POSER);
}

int demanderLigne()
{
    return lireEntier("Ligne", 0, HAUTEUR - 1);
}

int demanderColonne()
{
    return lireEntier("Colonne", 0, LARGEUR - 1);
}

int demanderDirection()
{
    int direction;

    printf("    7 8 9\n");
    printf("    4   6\n");
    printf("    1 2 3\n");

    do {
        direction = lireEntier("Direction", DIRECTION_MIN, DIRECTION_MAX);
        if (direction == DIRECTION_CENTRE) {
            printf("  %d n'est pas une direction.\n", DIRECTION_CENTRE);
        }
    } while (direction == DIRECTION_CENTRE);

    return direction;
}

int demanderDistance()
{
    return lireEntier("Nombre de cases", 1, DISTANCE_MAX);
}

int directionLigne(int direction)
{
    switch (direction) {
        case 7:
        case 8:
        case 9:
            return -1;
        case 1:
        case 2:
        case 3:
            return 1;
        default:
            return 0;
    }
}

int directionColonne(int direction)
{
    switch (direction) {
        case 7:
        case 4:
        case 1:
            return -1;
        case 9:
        case 6:
        case 3:
            return 1;
        default:
            return 0;
    }
}

int deplacerAvion(int grille[HAUTEUR][LARGEUR], int joueur, int ligne, int colonne,
                  int direction, int distance)
{
    int nouvelleLigne = ligne + directionLigne(direction) * distance;
    int nouvelleColonne = colonne + directionColonne(direction) * distance;

    if (grille[ligne][colonne] != joueur) {
        return ERREUR_PAS_TON_AVION;
    }
    if (estDansGrille(nouvelleLigne, nouvelleColonne) == 0) {
        return ERREUR_HORS_GRILLE;
    }
    if (grille[nouvelleLigne][nouvelleColonne] != VIDE) {
        return ERREUR_CASE_OCCUPEE;
    }

    grille[nouvelleLigne][nouvelleColonne] = joueur;
    grille[ligne][colonne] = VIDE;
    return ACTION_OK;
}

int main()
{
    int grille[HAUTEUR][LARGEUR];
    int scores[NB_JOUEURS + 1];
    int reserve[NB_JOUEURS + 1];
    int manche = PREMIERE_MANCHE;

    initialiserPartie(grille, scores, reserve);
    afficherGrille(grille);
    afficherScores(grille, scores, reserve, manche);

    return 0;
}
