# Sky Conway

Jeu en tour par tour pour **2 joueurs**, en C dans la console, basé sur le jeu de la vie de John Conway.

---

## 1. Le principe

Chaque joueur dirige une compagnie aérienne. Il doit faire traverser le ciel à ses avions pour les faire **atterrir de l'autre côté de la grille**.

Le ciel obéit aux règles du jeu de la vie :

- un avion trop isolé se perd ;
- trop d'avions serrés au même endroit se percutent ;
- là où il y a exactement 3 avions autour d'une case vide, un nouvel avion apparaît.

En plus, des **zones de grêle** rendent certaines parties du ciel dangereuses.

Les joueurs ne peuvent pas arrêter l'évolution du ciel. Ils peuvent seulement **déplacer** ou **poser** des avions avant chaque mise à jour.

---

## 2. Le matériel

### Les joueurs

| Joueur | Compagnie | Symbole | Zone de départ | Ligne d'arrivée |
|---|---|---|---|---|
| J1 | Air France | `F` | lignes 0 à 2 (en haut) | ligne 9 (tout en bas) |
| J2 | Aer Lingus | `L` | lignes 7 à 9 (en bas) | ligne 0 (tout en haut) |

### La grille

- 10 lignes × 12 colonnes.
- **Pas torique** : les bords ne sont pas reliés. Une case hors de la grille compte comme vide.
- Une case vaut `.` (ciel), `#` (grêle), `F` (avion de J1) ou `L` (avion de J2).

### La carte de départ

Chaque joueur commence avec **un carré de 4 avions** sur la grille et **6 avions en réserve**.

```
      0  1  2  3  4  5  6  7  8  9 10 11
  0   .  .  .  .  .  .  .  .  .  .  .  .    zone F  | arrivée L
  1   .  .  F  F  .  .  .  .  .  .  .  .    zone F
  2   .  .  F  F  .  .  .  .  .  .  .  .    zone F
  3   .  .  .  .  .  .  .  .  .  .  .  .
  4   .  .  #  #  .  .  .  .  #  #  .  .
  5   .  .  #  #  .  .  .  .  #  #  .  .
  6   .  .  .  .  .  .  .  .  .  .  .  .
  7   .  .  .  .  .  .  .  .  L  L  .  .    zone L
  8   .  .  .  .  .  .  .  .  L  L  .  .    zone L
  9   .  .  .  .  .  .  .  .  .  .  .  .    zone L  | arrivée F
```

Le carré 2×2 s'appelle un **bloc** dans le jeu de la vie. C'est une forme **stable** : il ne bouge pas tant que personne n'y touche.

La carte est **symétrique par rotation** : chaque joueur trouve une zone de grêle exactement sur son chemin le plus court, donc aucun joueur n'est avantagé.

---

## 3. Le but du jeu

| Événement | Points |
|---|---|
| Un de ses avions **atterrit** | **+5** |
| Un nouvel avion **naît** pour sa compagnie | **+2** |

Les conditions de fin sont vérifiées à la fin de chaque manche, dans cet ordre :

1. **Un joueur atteint 20 points** : celui qui a le plus de points gagne. S'ils sont à égalité, la partie est nulle.
2. **Un joueur a moins de 2 avions** (sur la grille et en réserve) : il est éliminé et l'autre gagne. Si les deux sont dans ce cas, la partie est nulle.
3. **La 30e manche est terminée** : celui qui a le plus de points gagne. En cas d'égalité, la partie est nulle.

Les points passent avant les avions : un joueur qui atteint 20 points en faisant atterrir ses derniers avions gagne.

---

## 4. Déroulement d'une manche

1. Le **premier joueur** fait ses **2 actions**.
2. Le **second joueur** fait ses **2 actions**.
3. Le ciel **évolue une fois** selon les règles de Conway.
4. Les avions arrivés sur leur ligne d'arrivée **atterrissent**.
5. La **grêle** frappe : les avions sur une case de grêle s'écrasent.
6. On vérifie si la partie est terminée.

Le premier joueur change à chaque manche : J1 commence les manches impaires, J2 les manches paires.

---

## 5. Les actions

À chaque action, le joueur choisit :

| Touche | Action | Règle |
|---|---|---|
| `1` | **Déplacer** | Un de ses avions avance de **1 ou 2 cases en ligne droite**, dans l'une des 8 directions. La case d'arrivée doit être **vide** et dans la grille. Avec 2 cases, il peut passer par-dessus un autre avion. |
| `2` | **Poser** | Il prend un avion de sa réserve et le pose sur une case **vide** de **sa zone de départ**. |
| `0` | **Passer** | Il ne fait rien. |

Les directions se choisissent avec le pavé numérique :

```
7 8 9      ↖ ↑ ↗
4   6      ←   →
1 2 3      ↙ ↓ ↘
```

- Le jeu demande la **ligne**, puis la **colonne** de l'avion, puis la **direction**, puis le **nombre de cases** (1 ou 2).
- Si une action est impossible (pas son avion, case occupée, hors de la grille, réserve vide, hors de sa zone…), le jeu affiche pourquoi et redemande. L'action n'est pas perdue.
- Un même avion peut être déplacé deux fois dans le même tour.

---

## 6. L'évolution du ciel (règles de Conway)

Pour chaque case, on compte ses **voisins** : les avions présents dans les 8 cases autour. **La compagnie ne compte pas** : un `F` et un `L` sont tous les deux un voisin. La grêle ne compte pas non plus : une case de grêle vide est une case vide.

| Situation | Résultat |
|---|---|
| Case vide avec **exactement 3** voisins | Un avion **naît** |
| Avion avec **2 ou 3** voisins | Il **survit** |
| Avion avec **0, 1, 4 voisins ou plus** | Il **disparaît** |

Toutes les cases sont mises à jour **en même temps** : on calcule la nouvelle grille à partir de l'ancienne, sans toucher à l'ancienne pendant le calcul.

**À qui appartient un avion qui naît ?** À la compagnie qui a **le plus d'avions parmi ses 3 voisins**. Comme il y a 3 voisins et 2 compagnies, il y a toujours une majorité : 2 `F` et 1 `L` donnent un `F`. Cette compagnie marque **+2 points**.

Un avion qui survit garde sa compagnie.

---

## 7. L'atterrissage

Après chaque action et après l'évolution, tout avion qui se trouve sur **la ligne d'arrivée de sa compagnie** atterrit : il est retiré de la grille et rapporte **5 points**.

- Un `F` sur la ligne 9 atterrit. Un `L` sur la ligne 9 reste en place.
- Un avion qui naît directement sur sa ligne d'arrivée atterrit aussi.
- Un avion qui a atterri ne revient pas en jeu.

---

## 8. La grêle

À la fin de chaque manche, après l'évolution et les atterrissages, **tout avion qui se trouve sur une case de grêle s'écrase** : il est retiré de la grille, sans points.

- Un avion peut **traverser** la grêle pendant les actions sans risque. Le danger, c'est d'y être **au moment de la fin de manche**.
- Un avion qui naît sur une case de grêle s'écrase donc tout de suite.

---

## 9. Exemples

### Faire avancer un bloc (2 actions)

Un bloc ne peut pas avancer case par case sans se casser. Mais on peut faire **sauter les deux avions du haut** de 2 cases vers le bas (direction `2`, 2 cases) :

```
Avant                 Action 1 : (1,2) → (3,2)    Action 2 : (1,3) → (3,3)
1   .  F  F  .        1   .  .  F  .              1   .  .  .  .
2   .  F  F  .        2   .  F  F  .              2   .  F  F  .
3   .  .  .  .        3   .  F  .  .              3   .  F  F  .
```

Le bloc a avancé d'une ligne et il est toujours stable : il survit à l'évolution. C'est la technique de base pour traverser le ciel.

### La cascade d'atterrissage (1 action)

Quand un bloc `F` occupe les lignes 7 et 8, on fait sauter **un seul** avion du haut sur la ligne 9 : il atterrit (+5). Il reste 3 avions en équerre. À l'évolution, la case vide a exactement 3 voisins : un avion `F` y naît (+2) et **le bloc se reforme tout seul**. On peut recommencer à la manche suivante.

```
Avant                 Saut (7,2) → (9,2)       Après l'évolution
7   .  F  F  .        7   .  .  F  .           7   .  F  F  .
8   .  F  F  .        8   .  F  F  .           8   .  F  F  .
9   .  .  .  .        9   .  .  .  .  (+5)     9   .  .  .  .  (+2)
```

Si on fait atterrir 2 avions d'un coup, les 2 qui restent sont isolés et meurent.

### Attaquer l'adversaire

Un seul avion `L` posé juste sous un bloc `F` suffit à le casser :

```
Avant l'évolution     Après 1 évolution
4   .  F  F  .        4   .  F  F  .
5   .  F  F  .        5   F  .  .  .
6   .  L  .  .        6   .  L  F  .
```

Les avions du bas du bloc ont maintenant 4 voisins : ils disparaissent. Le bloc n'est plus stable.

### La grêle

Un bloc qui fonce tout droit vers le bas arrive sur la grêle des colonnes 2 et 3. S'il termine la manche dessus, ses avions s'écrasent. Il faut donc **contourner** la zone, ce qui coûte des manches, ou sauter par-dessus au bon moment.

---

## 10. Pourquoi c'est un vrai jeu

- **Avancer coûte cher** : un avion seul meurt à la fin de la manche. Il faut faire voyager des groupes stables.
- **La grêle oblige à choisir** : passer au plus court en prenant le risque d'y rester, ou faire un détour.
- **On peut attaquer** : un avion bien placé à côté d'un groupe adverse provoque une surpopulation.
- **On peut voler des avions** : une naissance revient à la compagnie majoritaire autour.
- **La réserve est limitée** : chaque avion perdu rapproche de l'élimination.

---

## 11. Respect des règles de Conway

- Sans action des joueurs et sans grêle, la grille évolue **exactement** selon les règles de Conway. La compagnie d'un avion ne change rien au nombre de voisins.
- Nos règles ajoutées ne changent jamais le calcul de l'évolution :
  - la **compagnie** d'un avion qui naît ne change que son propriétaire, pas le fait qu'il naisse ;
  - l'**atterrissage** et la **grêle** retirent des avions **après** l'évolution, comme le ferait une action de joueur.
- La grêle est la seule altération des règles : elle peut retirer un avion pourtant vivant au sens de Conway. Elle est limitée à des **zones fixes, affichées sur la carte**, comme le sujet l'autorise.
- Grille finie, **non torique**, 10 × 12. Les cases hors de la grille comptent comme vides.
- **Une seule évolution par manche**, après que les deux joueurs ont joué, pour que l'évolution soit la même pour les deux.

---

## 12. Choix techniques

- Un seul fichier `program.c`, écrit uniquement avec les notions vues en cours : variables, `if` / `switch`, boucles `for` / `while` / `do...while`, fonctions, tableaux à 1 et 2 dimensions, chaînes de caractères.
- Pas de `struct`, pas de pointeurs, pas de `malloc`, pas de bibliothèque externe.
- Les saisies se font avec `fgets`, comme dans le cours. Le texte tapé est ensuite converti en nombre caractère par caractère.
- La carte (ciel et grêle) est un tableau de chaînes de caractères. Les avions sont dans un tableau d'entiers séparé : `0` = vide, `1` = Air France, `2` = Aer Lingus.
- Les fonctions de logique du jeu ne font aucun affichage : elles renvoient un code, et ce sont les fonctions d'affichage qui écrivent les messages.

### Constantes du programme

Toutes ces valeurs sont des `#define` en haut de `program.c`. On peut les changer après des parties de test.

| Constante | Valeur |
|---|---|
| `HAUTEUR` / `LARGEUR` | 10 / 12 |
| `VIDE` / `JOUEUR1` / `JOUEUR2` | 0 / 1 / 2 |
| `TAILLE_ZONE` | 3 (lignes de la zone de départ) |
| `ACTIONS_PAR_TOUR` | 2 |
| `DISTANCE_MAX` | 2 |
| `RESERVE_DEPART` | 6 |
| `POINTS_ATTERRISSAGE` | 5 |
| `POINTS_NAISSANCE` | 2 |
| `SCORE_VICTOIRE` | 20 |
| `FLOTTE_MIN` | 2 |
| `MANCHES_MAX` | 30 |
