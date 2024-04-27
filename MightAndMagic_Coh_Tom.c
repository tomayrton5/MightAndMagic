#include <stdio.h>

// Interface pour copier : ----------------- JOUEUR 1 -----------------

// Types structurés : 

struct Unite {
    char nom[12];
    int taille;
    int attaque;
    int defense;
    int compteur;
    int nb_renfort;
};

typedef struct Unite S_unite ;

struct Joueur {
    char prenom[26];
    int age;
    int couleur;
    int point_de_mouvement;
    int pt_de_vie;
    // 3 Couleurs attribuées
};
typedef struct Joueur S_joueur ;

// -----------------------------------------------------------------

// Création fichier .h (Initialisation types structurés)
/* 
1) Plateau.h : Déclarations fonctions (en lien avec plateau), manipulation des cases, détection colisions ...
2) Unites.h : Déclarations fonctions, structures (statistiques unités)
3) Activations.h : Murs et Unités activés
4) Interface.h : Affichage plateaux, règles de jeu, menu principal, menu avancé ...
5) Utils.h : Fonctions de gestion de mémoire, fonctions de génération des apparitions aléatoires ... 
*/

// -----------------------------------------------------------------

// Sous-programmes : 

void Donnees_Joueurs(S_joueur *joueur1, S_joueur *joueur2)
{
    printf("----------------- JOUEUR 1 -----------------\n");
    printf("Quel est le prénom du joueur 1 ?\n");
    scanf(" %s", joueur1->prenom);
    printf("Quel est l'âge du joueur 1 ?\n");   
    scanf("%d", &joueur1->age);
    joueur1->point_de_mouvement = 3;
    joueur1->pt_de_vie = 100;
    printf("\n");
    printf("----------------- JOUEUR 2 -----------------\n");
    printf("Quel est le prénom du joueur 2 ?\n");
    scanf(" %s", joueur2->prenom);
    printf("Quel est l'âge du joueur 2 ?\n");
    scanf("%d", &joueur2->age);
    joueur2->point_de_mouvement = 3;
    joueur2->pt_de_vie = 100;
    printf("\n");

    if (joueur1->age < joueur2->age)
    {
        printf("----------------- JOUEUR 1 -----------------\n");
        printf("Choisissez la couleur rouge ou bleu\n");
        scanf(" %c", &joueur1->couleur);
    }
    else
    {
        printf("----------------- JOUEUR 2 -----------------\n");
        printf("Choisissez la couleur rouge ou bleu :\n");
        scanf(" %c", &joueur2->couleur);
    }
}


void Activer_Murs(int ligne, int colonne, int plateau[][8]) {
   int points_de_vie_murs = 3; // Initialisation des points de vie des murs
    for (int i = 0; i < 6; i++) {
        int count = 0;
        for (int j = 0; j < 8; j++) {
            if (plateau[i][j] == 1) {
                count++;
                if (count == 3 && plateau[i][j-2]==1 && plateau[i][j-1]==1 && plateau[i][j]==1 ) {
                    plateau[i][j - 2] = 4;   // (0=vide, 1=basique, 2=élite, 3=légendaire, 4=murs, 5=activation_B)
                    plateau[i][j - 1] = 4;
                    plateau[i][j] = 4;
                }
            } else {
                count = 0;
            }
        }
    }
}

/* Dans le main : 

S_unite tab_Unite[96];      --> 96 car il peut y avoir au maximum 96 unités sur les 2 plateaux (2*6*8)
for (int i = 0; i < 96; i++) {
    tab_Unite[i].compteur = 0; // Initialisation du compteur à 0 pour toutes les unités
}
*/

void Activer_Basiques(int ligne, int colonne, int plateau[][8], S_unite tab_Unite[]) {
  for (int j = 0; j < 8; j++) {
      int count = 0;
      for (int i = 0; i < 6; i++) {
          if (plateau[i][j] == 1) {
              count++;
              if (count == 3) {
                  plateau[i - 2] [j] = 5; 
                  plateau[i - 1][j] = 5;
                  plateau[i][j] = 5;
                for (int k = 0; k < 3; k++) {
                  if (tab_Unite[i -2 + k* LON + j].compteur > 0){
                    tab_Unite[i -2 + k* LON + j].compteur--;
                  }
                }
              }
          } else {
              count = 0;
          }
      }
  }
}

void Activer_Elites(int ligne, int colonne, int plateau[][8], S_unite tab_Unite[]) {
  for (int j = 0; j < 8; j++) { 
          for (int i = 0; i < 6-1 ; i++) {
              if (plateau[i][j] == 2 && plateau[i+1][j] == 1 && plateau[i+2][j] == 1) {
                      plateau[i + 2] [j] = 6;         // 6=activation_E (donc compteur + statistiques en fonction de case=6)
                      plateau[i + 1][j] = 6;
                      plateau[i][j] = 6;
                for (int k=0; k < 3; k++){
                  if (tab_Unite[i + k* LON + j].compteur > 0) {
                    tab_Unite[i + k* LON + j].compteur--;
                  }
                }
              }
          }
      }
}

void Activer_Legendaires(int ligne, int colonne, int plateau[][8], S_unite tab_Unite[], int *nbUnites) {
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 6 - 1; i++) {
            if (plateau[i][j] == 3) {
                // Vérifier si les conditions sont remplies pour activer l'unité légendaire
                if (plateau[i + 1][j] == 1 && plateau[i][j + 1] == 1 && plateau[i + 1][j + 1] == 1) {
                    // Activer l'unité légendaire en changeant sa valeur dans la matrice plateau
                    plateau[i][j] = 7; 

                    // Stocker les détails de l'unité légendaire dans tab_Unite
                    tab_Unite[*nbUnites].ligne = i;
                    tab_Unite[*nbUnites].colonne = j;
                    tab_Unite[*nbUnites].type_unite = 7; // 7=activation_L (donc compteur + statistiques en fonction de case=7)
                    tab_Unite[*nbUnites].compteur = 5; // Initialiser le compteur à 5
                    (*nbUnites)++;
                }
            }
        }
    }
}


void Avance_Sur_Front(int ligne, int colonne, int plateau[][8]) {
    for (int j = 0; j < 8; j++) {
        for (int i = 1; i < 6; i++) {
            if ((plateau[i][j] == 4 || plateau[i][j] == 5 || plateau[i][j] == 6) && (plateau[i - 1][j] == 0 || plateau[i - 1][j] == 1 || plateau[i - 1][j] == 2 || plateau[i - 1][j] == 3)) {
                // Vérifie si la case devant est vide ou contient une unité non activée
                int k = i - 1;
                while (k > 0 && plateau[k][j] == 0) {
                    k--;
                }
                // Si la case devant est un mur, laisser l'unité activée actuelle sur sa position
                // Sinon, effectuer l'échange
                if (plateau[k][j] != 4) {
                    int temp = plateau[k + 1][j];
                    plateau[k + 1][j] = plateau[i][j];
                    plateau[i][j] = temp;
                }
            } 
        }
    }
}


int afficherMenu(int ligne, int colonne) ;
void menu_principal() ;
void couleur_aleatoire_joueur2() ;
void couleur_aleatoire_joueur1() ;
void dessiner_plateau1(int ligne, int colonne, int plateau[][8]) ;
void dessiner_plateau2(int ligne, int colonne, int plateau[][8] ) ;
void dessiner_murs(int ligne, int colonne,int c) ;
void initialiser_plateau( int plateau [][8]) ;
void dessiner_basiques(int ligne, int colonne,int c);
void dessiner_elites(int ligne, int colonne,int c) ;
void dessiner_legendaires(int ligne, int colonne,int c) ;
void dessiner_plateau_vide(int ligne, int colonne ) ;
void dessiner_rectangle(int ligne,int colonne,int c,int lg, int la) ;
void positionner_curseur(int ligne, int colonne);
void color (int couleurDuTexte, int couleurDuFond);
void plein_ecran();
void affichage_caractere_speciaux();
void normal_ecran();
int main()
{
    srand(time(NULL)); // permet d'initialiser le générateur pseudo aléatoire doit se faire qu'un seule fois au début d'un programme si vous utiliser la fonction rand()
    plein_ecran() ;

    system("pause");
    int plateau [8][8], z ;
    initialiser_plateau(plateau) ;
    dessiner_plateau1( 8, 55,plateau ) ;
    dessiner_murs( 9, 56, c) ;
    // normal_ecran(); // Vous pouvez commenter ou supprimer cette ligne

    color(0,0) ;
    positionner_curseur(140,140);



    return 0 ;
}



void positionner_curseur(int ligne, int colonne)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);//permet de déclarer la variable "hstdout" qui fait référénce à la console
    COORD pos; // COORD est un type structuré défini dans la bibliothèque windows.h
    pos.X=colonne;// numéro de la colonne
    pos.Y=ligne;// numéro de la ligne
    SetConsoleCursorPosition(hStdout, pos);
}




void plein_ecran()
{
    // Press ALT key
    keybd_event(VK_MENU, 0x38, 0, 0);

    // Press ENTER key
    keybd_event(VK_RETURN, 0x1c, 0, 0);

    // Release ENTER key
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);

    // Release ALT key
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);

    // Zoom out by 5 clicks (simulating CTRL and '-')
    for (int i = 0; i <9 ; i++)
    {
        keybd_event(VK_CONTROL, 0x1d, 0, 0); // Press CTRL key
        keybd_event(0xBD, 0, 0, 0); // Press '-' key (VK_SUBTRACT)
        keybd_event(0xBD, 0, KEYEVENTF_KEYUP, 0); // Release '-' key
        keybd_event(VK_CONTROL, 0x1d, KEYEVENTF_KEYUP, 0); // Release CTRL key
    }
}

void normal_ecran()
{
    // Zoom in by 5 clicks (simulating CTRL and '+')
    for (int i = 0; i <9 ; i++)
    {
        keybd_event(VK_CONTROL, 0x1d, 0, 0); // Press CTRL key
        keybd_event(0xBB, 0, 0, 0); // Press '+' key (VK_ADD)
        keybd_event(0xBB, 0, KEYEVENTF_KEYUP, 0); // Release '+' key
        keybd_event(VK_CONTROL, 0x1d, KEYEVENTF_KEYUP, 0); // Release CTRL key
    }

    // Press ALT key
    keybd_event(VK_MENU, 0x38, 0, 0);

    // Press ENTER key
    keybd_event(VK_RETURN, 0x1c, 0, 0);

    // Release ENTER key
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);

    // Release ALT key
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

void affichage_caractere_speciaux()
{

    printf("%c\n\n",218);
    printf("%c\n\n",196);
    printf("%c\n\n",191);
    printf("%c\n\n",179);
    printf("%c\n\n",195);
    printf("%c\n\n",180);
    printf("%c\n\n",192);
    printf("%c\n\n",217);

}

void color (int couleurDuTexte, int couleurDuFond)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDuFond*16+couleurDuTexte);
}
// 0.Noir
// 1.Bleu fonc�
// 2.Vert fonc�
// 3.Turquoise
// 4.Rouge fonc�
// 5.Violet
// 6.Vert caca d'oie
// 7.Gris clair
// 8.Gris fonc�
// 9.Bleu fluo
// 10.Vert fluo
// 11.Turquoise
// 12.Rouge fluo
// 13.Violet 2
// 14.Jaune
// 15.Blanc
void dessiner_rectangle(int ligne, int colonne, int c, int lg, int la)
{
    color(c, c);
    positionner_curseur(ligne,colonne);
    for (int i = 0; i < lg; i++)
    {
        for (int j = 0; j < la; j++)
        {
            positionner_curseur(ligne + i, colonne + j);
            printf(" ");
        }
    }
    positionner_curseur(ligne,colonne);
}

void dessiner_plateau_vide(int ligne, int colonne )
{

    for ( int j=0 ; j<=8; j++)
        for (int i =0 ; i<6*LON ; i++ )
        {
            color (15,15) ;
            positionner_curseur(ligne+i,colonne+LAR*j);
            printf("%c\n\n",179);
        }

    for ( int j=0 ; j<=8*LAR; j++)
        for (int i =0 ; i<=6; i++ )
        {

            positionner_curseur(ligne+LON*i,colonne+j);
            printf("%c\n\n",196);
        }





}






void dessiner_basiques(int ligne, int colonne,int c)
{

    couleur_aleatoire_joueur1() ;
    for (int i=2 ; i<8 ; i++ )
    {
        for ( int j=3 ; j <13 ; j++)
        {

            positionner_curseur (ligne + i, colonne+ j ) ;
            printf(" ");

        }
    }

    color(0,0) ;

}

void dessiner_elites(int ligne, int colonne,int c)
{
    couleur_aleatoire_joueur1() ;
    for (int i=4 ; i<15; i++ )
    {

        for ( int j=2 ; j <14; j++)
        {

            positionner_curseur (ligne + i, colonne+ j ) ;
            printf(" ");

        }
    }



    for ( int j=5 ; j < 11 ; j++)
    {

        positionner_curseur (ligne + 2, colonne+ j ) ;
        printf(" ");

    }
    for ( int j=3 ; j < 13 ; j++)
    {

        positionner_curseur (ligne + 3, colonne+ j ) ;
        printf(" ");


    }
    for ( int j=5 ; j < 11 ; j++)
    {

        positionner_curseur (ligne + 16, colonne+ j ) ;
        printf(" ");

    }
    for ( int j=3 ; j < 13 ; j++)
    {

        positionner_curseur (ligne + 15, colonne+ j ) ;
        printf(" ");

    }

    color(0,0) ;

}


void dessiner_legendaires(int ligne, int colonne,int c)
{
    couleur_aleatoire_joueur1() ;

    for ( int j=6 ; j <= 24; j++)
    {

        positionner_curseur (ligne + 2, colonne+ j ) ;
        printf(" ");

    }
    for ( int j=4 ; j <= 26; j++)
    {

        positionner_curseur (ligne + 3, colonne+ j ) ;
        printf(" ");

    }


    for (int i= 4; i< 15; i++ )
    {

        for ( int j=3 ; j <= 27; j++)
        {

            positionner_curseur (ligne + i, colonne+ j ) ;
            printf(" ");

        }
    }
    for ( int j=6 ; j <= 24; j++)
    {

        positionner_curseur (ligne + 16, colonne+ j ) ;
        printf(" ");

    }
    for ( int j=4 ; j <= 26; j++)
    {

        positionner_curseur (ligne + 15, colonne+ j ) ;
        printf(" ");







    }
}

    void couleur_aleatoire_joueur1()
    {
        int x ;
        x = rand() % 3+1 ;
        switch (x)
        {
        case 1:
            color(1,1);
            break;
        case 2:
            color(5,5);

            break;
        case 3:
            color(2,2);

            break;

        }


    }

    void couleur_aleatoire_joueur2()
    {
        int x ;
        x = rand() % 3+1 ;
        switch (x)
        {


        }


    }





    void dessiner_murs(int ligne, int colonne,int c)
    {
        color(c,c) ;
         for ( int j=1 ;  j < 3; j++)
        {

            positionner_curseur (ligne + 5, colonne+ j ) ;
            printf(" ");

        }

        for ( int j=4 ;  j <6 ; j++)
        {

            positionner_curseur (ligne + 5, colonne+ j ) ;
            printf(" ");

        }
         for ( int j=7 ;  j < 9 ; j++)
        {

            positionner_curseur (ligne + 5, colonne+ j ) ;
            printf(" ");

        }
        for ( int j=10 ;  j < 12 ; j++)
        {

            positionner_curseur (ligne + 5, colonne+ j ) ;
            printf(" ");

        }
        for ( int j=13 ;  j < 15; j++)
        {

            positionner_curseur (ligne + 5, colonne+ j ) ;
            printf(" ");

        }

    }




    void dessiner_plateau1(int ligne, int colonne, int plateau[][8] )
    {
        {

            dessiner_plateau_vide(ligne, colonne);


            int x, y;
            for ( int i=0 ; i<10; i++ )
            {

                x = rand() % 6;
                y = rand() % 8;


                plateau[x][y] = rand() % 4 + 1;

                switch (plateau[x][y]) // appeler les couleurs dans le swtich case !!!!!!!!!
                {
                case 1:
                    color(0,0);
                    dessiner_basiques(ligne + x * LON, colonne + y * LAR, 0);
                    break;
                case 2:
                    color(0,0);
                    dessiner_elites(ligne + x * LON, colonne + y * LAR, 0);
                    break;
                case 3:
                    color(0,0);
                    dessiner_legendaires(ligne + x * LON, colonne + y * LAR, 0);
                    break;
                case 4:
                    color(0,0);
                    dessiner_murs(ligne + x * LON, colonne + y * LAR, 0);
                    break ;


                }
            }



            color(0,0) ;
        }
    }

    void dessiner_plateau2(int ligne, int colonne, int plateau[][8] )
    {
        {

            dessiner_plateau_vide(ligne, colonne);
            /*
            int x, y ;

            x = rand() % 7;
            y = rand() % 7;


            plateau[x][y] = rand() % 4 + 1;

            switch (plateau[x][y])
            {
            case 1:
                color(0,0);
                dessiner_basiques(ligne + x * LON, colonne + y * LAR, 0);
                break;
            case 2:
                color(0,0);
                dessiner_elites(ligne + x * LON, colonne + y * LAR, 0);
                break;
            case 3:
                color(0,0);
                dessiner_legendaires(ligne + x * LON, colonne + y * LAR, 0);
                break;
            case 4:
                color(0,0);
                dessiner_murs(ligne + x * LON, colonne + y * LAR, 0);
                break;
            }
            */
            color(0,0) ;
        }

    }



    void initialiser_plateau(int plateau [][8])
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                plateau[i][j] = 0;
            }
        }

    }

    void menu_principal()
    {
        char menu[3][58] =
        {
            "   Jouer                                ",
            "   Regles de jeu                       ",
            "   Quitter                             "
        };

        int choix = 0; // numéro de l'option choisie dans le menu
        int frappe = 0; // permet de stocker le code de la touche clavier

        while (frappe != 13) // tant qu'on n'a pas appuyé sur la touche Entrée
        {
            system("cls");
            printf(" ___________  Menu principal __________\n");
            printf("|                                       |\n");
            printf("|                                       |\n");
            for (int i = 0; i < 3; i++)
            {
                if (i == choix)
                {
                    color(0, 15); // Inverse les couleurs pour la sélection en surbrillance
                    printf("|%s|\n", menu[i]);
                    color(15, 0);
                }
                else
                {
                    printf("|%s|\n", menu[i]);
                }
            }
            printf("|                                       |\n");
            printf("|_______________________________________|\n");

            frappe = getch(); // Attend une frappe de touche

            if (frappe == 72) // 72 est le code de la flèche directionnelle vers le haut
            {
                // Gestion du déplacement vers le haut
                if (choix > 0)
                {
                    choix--;
                }
            }
            else if (frappe == 80) // 80 est le code de la flèche directionnelle vers le bas
            {
                // Gestion du déplacement vers le bas
                if (choix < 2)
                {
                    choix++;
                }
            }
        }

        system("cls");
        printf(" ___________  Menu principal __________\n");
        for (int i = 0; i < 3; i++)
        {
            printf("|%s|\n", menu[i]);
            if (i == choix)
            {
                printf("|_______________________________________|\n\n");
            }
            else
            {
                printf("|                                       |\n");
            }
        }

        switch (choix)
        {
        case 0:
            // Ajoutez le code pour ouvrir un fichier
            printf("Vous avez choisi d'ouvrir un fichier.\n");
            break;
        case 1:
            // afficher_regles_jeu(); // Appeler la fonction pour afficher les règles du jeu
            printf("Vous avez choisi de consulter les règles du jeu.\n");
            break;
        case 2:
            printf("Vous avez choisi de quitter. Au revoir !\n");
            break;
        default:
            printf("Choix invalide ! Merci de taper une valeur entre 1 et 3 !\n");
            break;
        }
        system("pause");
    }
