#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************
 *        STRUCTURES PRODUITS
 **************************************/
typedef struct Produit {
    int id;
    char nom[30];
    float prix;
    int quantite;
    struct Produit *suivant;
} Produit;

#define TAILLE_TABLE 13

typedef struct {
    Produit *table[TAILLE_TABLE];
} TableHachage;


/**************************************
 *        STRUCTURES CLIENTS (ABR)
 **************************************/
typedef struct Client {
    int id;
    char nom[30];
    float totalDepense;
    struct Client *gauche;
    struct Client *droite;
} Client;


/**************************************
 *        STRUCTURES FILE (CAISSE)
 **************************************/
typedef struct ClientFile {
    int idClient;
    struct ClientFile *suivant;
} ClientFile;

typedef struct {
    ClientFile *deb;
    ClientFile *fin;
} FileAttente;


/**************************************
 *     STRUCTURES TRANSACTIONS (PILE)
 **************************************/
typedef struct Transaction {
    int idClient;
    int idProduit;
    int quantite;
    float total;
    char date[20];
    struct Transaction *suivant;
} Transaction;


/**************************************
 *         FONCTION DE HACHAGE
 **************************************/
int hachage(int id) {
    return id % TAILLE_TABLE;
}


/**************************************
 *        PRODUITS — AJOUT
 **************************************/
void ajouterProduit(TableHachage *tab, int id, char nom[], float prix, int quantite) {
    int index = hachage(id);

    Produit *p = malloc(sizeof(Produit));
    p->id = id;
    strcpy(p->nom, nom);
    p->prix = prix;
    p->quantite = quantite;
    p->suivant = tab->table[index];
    tab->table[index] = p;

    printf("Produit ajouté.\n");
}


/**************************************
 *   PRODUITS — AFFICHAGE SIMPLES
 **************************************/
void afficherProduits(TableHachage *tab) {
    printf("\n--- Liste des produits ---\n");

    for (int i = 0; i < TAILLE_TABLE; i++) {
        Produit *p = tab->table[i];
        while (p != NULL) {
            printf("ID: %d | Nom: %s | Prix: %.2f | Stock: %d\n",
                   p->id, p->nom, p->prix, p->quantite);
            p = p->suivant;
        }
    }
}


/**************************************
 *  RECHERCHE PRODUIT PAR ID
 **************************************/
Produit* chercherProduit(TableHachage *tab, int id) {
    int index = hachage(id);
    Produit *p = tab->table[index];

    while (p != NULL) {
        if (p->id == id)
            return p;
        p = p->suivant;
    }
    return NULL;
}


/**************************************
 *        CLIENTS — INSERTION ABR
 **************************************/
Client* ajouterClient(Client *racine, int id, char nom[], float total) {
    if (racine == NULL) {
        Client *c = malloc(sizeof(Client));
        c->id = id;
        strcpy(c->nom, nom);
        c->totalDepense = total;
        c->gauche = c->droite = NULL;
        return c;
    }
    if (id < racine->id)
        racine->gauche = ajouterClient(racine->gauche, id, nom, total);
    else
        racine->droite = ajouterClient(racine->droite, id, nom, total);

    return racine;
}


/**************************************
 *    CLIENTS — AFFICHAGE INORDRE
 **************************************/
void afficherClients(Client *racine) {
    if (racine == NULL) return;
    afficherClients(racine->gauche);
    printf("ID: %d | Nom: %s | Total dépensé: %.2f\n",
           racine->id, racine->nom, racine->totalDepense);
    afficherClients(racine->droite);
}


/**************************************
 *       FILE D’ATTENTE — ENFILER
 **************************************/
void enfiler(FileAttente *f, int idClient) {
    ClientFile *nouv = malloc(sizeof(ClientFile));
    nouv->idClient = idClient;
    nouv->suivant = NULL;

    if (f->fin == NULL) {
        f->deb = f->fin = nouv;
    } else {
        f->fin->suivant = nouv;
        f->fin = nouv;
    }
    printf("Client %d ajouté à la file.\n", idClient);
}


/**************************************
 *  FILE — DEFILER (SUPPRIMER DEBUT)
 **************************************/
int defiler(FileAttente *f) {
    if (f->deb == NULL) return -1;

    int id = f->deb->idClient;
    ClientFile *tmp = f->deb;
    f->deb = f->deb->suivant;
    if (f->deb == NULL)
        f->fin = NULL;
    free(tmp);
    return id;
}


/**************************************
 *          MENU PRINCIPAL
 **************************************/
void menu() {
    printf("\n========== MINI SUPERMARCHE ==========\n");
    printf("1. Ajouter produit\n");
    printf("2. Afficher produits\n");
    printf("3. Ajouter client\n");
    printf("4. Afficher clients\n");
    printf("5. Enfiler un client (caisse)\n");
    printf("6. Passer un client à la caisse\n");
    printf("0. Quitter\n");
}


/**************************************
 *              MAIN
 **************************************/
int main() {
    TableHachage tab;
    for (int i = 0; i < TAILLE_TABLE; i++) tab.table[i] = NULL;

    Client *clients = NULL;
    FileAttente file = {NULL, NULL};

    int choix;

    do {
        menu();
        printf("Choix: ");
        scanf("%d", &choix);

        if (choix == 1) {
            int id, q;
            char nom[30];
            float prix;

            printf("ID: ");
            scanf("%d", &id);
            printf("Nom: ");
            scanf("%s", nom);
            printf("Prix: ");
            scanf("%f", &prix);
            printf("Quantité: ");
            scanf("%d", &q);

            ajouterProduit(&tab, id, nom, prix, q);
        }

        else if (choix == 2) {
            afficherProduits(&tab);
        }

        else if (choix == 3) {
            int id;
            char nom[30];

            printf("ID client: ");
            scanf("%d", &id);
            printf("Nom: ");
            scanf("%s", nom);

            clients = ajouterClient(clients, id, nom, 0);
            printf("Client ajouté.\n");
        }

        else if (choix == 4) {
            afficherClients(clients);
        }

        else if (choix == 5) {
            int id;
            printf("ID client: ");
            scanf("%d", &id);
            enfiler(&file, id);
        }

        else if (choix == 6) {
            int id = defiler(&file);
            if (id == -1)
                printf("La file est vide.\n");
            else
                printf("Client %d est passé à la caisse.\n", id);
        }

    } while (choix != 0);

    return 0;
}
