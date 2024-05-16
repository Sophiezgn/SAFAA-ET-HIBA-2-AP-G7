#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define MAX_USERS 10
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define FILENAME "users.txt"

typedef struct {
    int jour;
    int mois;
    int annee;
} date;

typedef struct {
    int ID;
    char NomPr[30];
    int Dosage;
    float prixUni;
    int QtSt;
    int seuilAS;
    date dateEntree;
    date dateSortie;
    date datePeremption;
    int StockMin;
    int StockS;
} produit;
typedef struct {
    int NumOrdonnance;
    char nomPatient[50];
    int nbMedicaments;
    struct {
        int ID;
        int quantite;
    } medicaments[10]; 
} Ordonnance;
typedef struct {
    int NumCommande;
    int codeCl;
    char nomCL[50];
    char VilleCL[50];
    int QtC;
    date dateC;
} Commande;
// Structure pour stocker les informations de l'utilisateur
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};
// Fonction pour vérifier les informations de connexion
int authenticate(struct User users[], int num_users, char *username, char *password) {
    for (int i = 0; i < num_users; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Authentification réussie
        }
    }
    return 0; // Authentification échouée
}

// Fonction pour enregistrer un nouvel utilisateur
int registerUser(struct User users[], int *num_users, char *username, char *password) {
    if (*num_users >= MAX_USERS) {
        return 0; // Impossible d'ajouter plus d'utilisateurs
    }
    strcpy(users[*num_users].username, username);
    strcpy(users[*num_users].password, password);
    (*num_users)++;
    return 1; // Enregistrement réussi
}

// Fonction pour enregistrer les utilisateurs dans un fichier texte
void saveUsersToFile(struct User users[], int num_users) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    for (int i = 0; i < num_users; ++i) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }

    fclose(file);
    printf("Les utilisateurs ont été enregistrés dans %s.\n", FILENAME);
}

// Fonction pour charger les utilisateurs à partir d'un fichier texte
int loadUsersFromFile(struct User users[]) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    int num_users = 0;
    while (fscanf(file, "%s %s", users[num_users].username, users[num_users].password) == 2) {
        num_users++;
        if (num_users >= MAX_USERS) {
            break;
        }
    }

    fclose(file);
    return num_users;
}

date saisirDate() {
    date d;
    printf("Entrez la date: ");
    scanf("%d %d %d", &d.jour, &d.mois, &d.annee);
    return d;
}

void clearScreen() {
    system("cls");
}

void afficherListeProduits() {
    FILE *fichier;
    fichier = fopen("GestionDeStockMedicament.csv", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    produit p;
    char ligne[1024];

    printf("ID,Nom du produit,Dosage,Prix unitaire,Quantite en stock,Seuil d'alerte de stock,Date de derniere entree en stock,Date de derniere sortie en stock,Date de peremption,Stock minimum,Stock de securite\n");

    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%d,%[^,],%d,%f,%d,%d,%d/%d/%d,%d/%d/%d,%d/%d/%d,%d,%d\n",
               &p.ID, p.NomPr, &p.Dosage, &p.prixUni, &p.QtSt, &p.seuilAS,
               &p.dateEntree.jour, &p.dateEntree.mois, &p.dateEntree.annee,
               &p.dateSortie.jour, &p.dateSortie.mois, &p.dateSortie.annee,
               &p.datePeremption.jour, &p.datePeremption.mois, &p.datePeremption.annee,
               &p.StockMin, &p.StockS);

        printf("%d,%s,%d,%.2f,%d,%d,%d/%d/%d,%d/%d/%d,%d/%d/%d,%d,%d\n",
               p.ID, p.NomPr, p.Dosage, p.prixUni, p.QtSt, p.seuilAS,
               p.dateEntree.jour, p.dateEntree.mois, p.dateEntree.annee,
               p.dateSortie.jour, p.dateSortie.mois, p.dateSortie.annee,
               p.datePeremption.jour, p.datePeremption.mois, p.datePeremption.annee,
               p.StockMin, p.StockS);
    }

    fclose(fichier);
}

void saisirProduit(FILE *fichier) {
    produit p;
    printf("Entrez l'ID du produit : ");
    scanf("%d", &p.ID);
    fprintf(fichier, "%d,", p.ID);

    printf("Entrez le nom du produit : ");
    scanf("%s", p.NomPr);
    fprintf(fichier, "%s,", p.NomPr);
    
    printf("Entrez dosage produit : ");
    scanf("%d", &p.Dosage );
    fprintf(fichier, "%d,", p.Dosage );
    
    printf("Entrez le prix unitaire du produit : ");
    scanf("%f", &p.prixUni);
    fprintf(fichier, "%.2f,", p.prixUni);

    printf("Entrez la quantite en stock du produit : ");
    scanf("%d", &p.QtSt);
    fprintf(fichier, "%d,", p.QtSt);

    printf("Entrez le seuil d'alerte de stock du produit : ");
    scanf("%d", &p.seuilAS);
    fprintf(fichier, "%d,", p.seuilAS);

    printf("Entrez la date de derniere entree en stock:\n");
    p.dateEntree = saisirDate();
    fprintf(fichier, "%d/%d/%d,", p.dateEntree.jour, p.dateEntree.mois, p.dateEntree.annee);

    printf("Entrez la date de derniere sortie en stock:\n");
    p.dateSortie = saisirDate();
    fprintf(fichier, "%d/%d/%d,", p.dateSortie.jour, p.dateSortie.mois, p.dateSortie.annee);

    printf("Entrez la date de peremption du produit:\n");
    p.datePeremption = saisirDate();
    fprintf(fichier, "%d/%d/%d,", p.datePeremption.jour, p.datePeremption.mois, p.datePeremption.annee);

    printf("Entrez le stock minimum du produit : ");
    scanf("%d", &p.StockMin);
    fprintf(fichier, "%d,", p.StockMin);

    printf("Entrez le stock de securite du produit : ");
    scanf("%d", &p.StockS);
    fprintf(fichier, "%d\n", p.StockS); 
}


void supprimerProduitByID(int id) {
    FILE *fichier, *temp;
    char ligne[1024];

    fichier = fopen("GGestionDeStockMedicament.csv", "r");
    temp = fopen("temp.csv", "w");

    if (fichier == NULL || temp == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    bool trouve = false;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        int produitID;
        if (sscanf(ligne, "%d,", &produitID) == 1) {
            if (produitID != id) {
                fprintf(temp, "%s", ligne);
            } else {
                trouve = true;
            }
        }
    }

    fclose(fichier);
    fclose(temp);

    remove("GestionDeStockDesMedicament.csv");
    rename("temp.csv", "GestionDeStockDesMedicament.csv");

    if (trouve) {
        printf("Produit avec l'ID %d supprimé avec succès.\n", id);
    } else {
        printf("Aucun produit avec l'ID %d trouvé.\n", id);
    }
}
void afficherProduitsPerimes() {
    FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    produit p;
    char ligne[1024];
    time_t now;
    struct tm *local;

    // Obtention de la date actuelle
    now = time(NULL);
    local = localtime(&now);
    int currentYear = local->tm_year + 1900;
    int currentMonth = local->tm_mon + 1;
    int currentDay = local->tm_mday;

    printf("Liste des produits périmés :\n");
    printf("ID\tNom du produit\tDate de péremption\n");

    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%d,%[^,],%*d,%*f,%*d,%*d,%*d/%*d/%*d,%*d/%*d/%*d,%d/%d/%d,%*d,%*d\n", 
               &p.ID, p.NomPr, &p.datePeremption.jour, &p.datePeremption.mois, &p.datePeremption.annee);
        if (p.datePeremption.annee < currentYear ||
            (p.datePeremption.annee == currentYear && p.datePeremption.mois < currentMonth) ||
            (p.datePeremption.annee == currentYear && p.datePeremption.mois == currentMonth && p.datePeremption.jour < currentDay)) {
            printf("%d\t%s\t%d/%d/%d\n", p.ID, p.NomPr, p.datePeremption.jour, p.datePeremption.mois, p.datePeremption.annee);
        }
    }

    fclose(fichier);
}
void ModifierMedicament() {
    FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "r+");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    int idProduit;
    printf("Entrez l'ID du produit à modifier : ");
    scanf("%d", &idProduit);

    produit p;
    bool produitTrouve = false;
    char ligne[1024];
    int posProduit = 0;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%d,%[^,],%d,%f,%d,%d,%d/%d/%d,%d/%d/%d,%d/%d/%d,%d,%d\n",
               &p.ID, p.NomPr, &p.Dosage, &p.prixUni, &p.QtSt, &p.seuilAS,
               &p.dateEntree.jour, &p.dateEntree.mois, &p.dateEntree.annee,
               &p.dateSortie.jour, &p.dateSortie.mois, &p.dateSortie.annee,
               &p.datePeremption.jour, &p.datePeremption.mois, &p.datePeremption.annee,
               &p.StockMin, &p.StockS);

        if (p.ID == idProduit) {
            produitTrouve = true;
            break;
        }
        posProduit = ftell(fichier);
    }

    if (!produitTrouve) {
        printf("Produit avec l'ID %d non trouvé.\n", idProduit);
        fclose(fichier);
        return;
    }

    printf("Informations actuelles du medicament:\n");
    printf("ID: %d\n", p.ID);
    printf("Nom: %s\n", p.NomPr);
    printf("Dosage: %d\n", p.Dosage);
    printf("Prix unitaire: %.2f\n", p.prixUni);
    printf("Quantité en stock: %d\n", p.QtSt);
    printf("Seuil d'alerte de stock: %d\n", p.seuilAS);
    printf("Date de dernière entrée en stock: %d/%d/%d\n", p.dateEntree.jour, p.dateEntree.mois, p.dateEntree.annee);
    printf("Date de dernière sortie en stock: %d/%d/%d\n", p.dateSortie.jour, p.dateSortie.mois, p.dateSortie.annee);
    printf("Date de péremption: %d/%d/%d\n", p.datePeremption.jour, p.datePeremption.mois, p.datePeremption.annee);
    printf("Stock minimum: %d\n", p.StockMin);
    printf("Stock de sécurité: %d\n", p.StockS);

    printf("\nEntrez les nouvelles informations du medicament :\n");
    printf("Entrez le nom du produit : ");
    scanf("%s", p.NomPr);
    printf("Entrez dosage produit : ");
    scanf("%d", &p.Dosage);
    printf("Entrez le prix unitaire du produit : ");
    scanf("%f", &p.prixUni);
    printf("Entrez la quantité en stock du produit : ");
    scanf("%d", &p.QtSt);
    printf("Entrez le seuil d'alerte de stock du produit : ");
    scanf("%d", &p.seuilAS);
    printf("Entrez la date de dernière entrée en stock  :\n");
    p.dateEntree = saisirDate();
    printf("Entrez la date de dernière sortie en stock :\n");
    p.dateSortie = saisirDate();
    printf("Entrez la date de péremption du produit :\n");
    p.datePeremption = saisirDate();
    printf("Entrez le stock minimum du produit : ");
    scanf("%d", &p.StockMin);
    printf("Entrez le stock de sécurité du produit : ");
    scanf("%d", &p.StockS);

    fseek(fichier, posProduit, SEEK_SET);
    fprintf(fichier, "%d,%s,%d,%.2f,%d,%d,%d/%d/%d,%d/%d/%d,%d/%d/%d,%d,%d\n",
            p.ID, p.NomPr, p.Dosage, p.prixUni, p.QtSt, p.seuilAS,
            p.dateEntree.jour, p.dateEntree.mois, p.dateEntree.annee,
            p.dateSortie.jour, p.dateSortie.mois, p.dateSortie.annee,
            p.datePeremption.jour, p.datePeremption.mois, p.datePeremption.annee,
            p.StockMin, p.StockS);

    printf("Produit avec l'ID %d modifié avec succès.\n", idProduit);

    fclose(fichier);
}

void rechercherProduitParNom(const char *nomRecherche) {
    FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    produit p;
    char ligne[1024];
    bool trouve = false;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%d,%[^,],%*d,%*f,%*d,%*d,%*d/%*d/%*d,%*d/%*d/%*d,%*d/%*d/%*d,%*d,%*d\n", 
               &p.ID, p.NomPr);
        if (strcmp(p.NomPr, nomRecherche) == 0) {
            printf("Produit trouvé :\n");
            printf("Nom: %s\n", p.NomPr);
            trouve = true;
            break; 
        }
    }

    fclose(fichier);

    if (!trouve) {
        printf("Aucun produit trouvé avec le nom \"%s\".\n", nomRecherche);
    }
}

void trierProduitsParPrixDecroissant() {
    FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }
    produit produits[10]; 
    int nbProduits = 0;
    char ligne[1024];

    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%d,%[^,],%*d,%f,%*d,%*d,%*d/%*d/%*d,%*d/%*d/%*d,%*d/%*d/%*d,%*d,%*d,%*d\n", 
               &produits[nbProduits].ID, produits[nbProduits].NomPr, &produits[nbProduits].prixUni);
        nbProduits++;
    }

    fclose(fichier);

    
    for (int i = 0; i < nbProduits - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < nbProduits; j++) {
            if (produits[j].prixUni > produits[maxIndex].prixUni) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            produit temp = produits[i];
            produits[i] = produits[maxIndex];
            produits[maxIndex] = temp;
        }
    }
}

void trierProduitsParNom() {
    FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }
    produit produits[10]; 
    int nbProduits = 0;
    char ligne[1024];

    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%d,%[^,],%*d,%f,%*d,%*d,%*d/%*d/%*d,%*d/%*d/%*d,%*d/%*d/%*d,%*d,%*d,%*d\n", 
               &produits[nbProduits].ID, produits[nbProduits].NomPr, &produits[nbProduits].prixUni);
        nbProduits++;
    }

    fclose(fichier);

    
    for (int i = 0; i < nbProduits - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < nbProduits; j++) {
            if (strcmp(produits[j].NomPr, produits[minIndex].NomPr) < 0) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            produit temp = produits[i];
            produits[i] = produits[minIndex];
            produits[minIndex] = temp;
        }
    }

    printf("Liste des produits triés par nom :\n");
    printf("ID\tNom du produit\tPrix unitaire\n");
    for (int i = 0; i < nbProduits; i++) {
        printf("%d\t%s\t%.2f\n", produits[i].ID, produits[i].NomPr, produits[i].prixUni);
    }
}




void saisirOrdonnance(Ordonnance *o) {
    printf("Entrez le numéro de l'ordonnance : ");
    scanf("%d", &o->NumOrdonnance);
    
    printf("Entrez le nom du patient : ");
    scanf("%s", o->nomPatient);
    
    printf("Entrez le nombre de médicaments prescrits : ");
    scanf("%d", &o->nbMedicaments);
    
    for (int i = 0; i < o->nbMedicaments; ++i) {
        printf("Médicament %d :\n", i + 1);
        printf("ID du médicament : ");
        scanf("%d", &o->medicaments[i].ID);
        printf("Quantité prescrite : ");
        scanf("%d", &o->medicaments[i].quantite);
    }
}

void enregistrerOrdonnance(const Ordonnance *o) {
    FILE *fichier;
    fichier = fopen("ordonnances.csv", "a");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier d'ordonnances.\n");
        return;
    }

    
    fprintf(fichier, "%d,%s,%d", o->NumOrdonnance, o->nomPatient, o->nbMedicaments);
    for (int i = 0; i < o->nbMedicaments; ++i) {
        fprintf(fichier, ",%d,%d", o->medicaments[i].ID, o->medicaments[i].quantite);
    }
    fprintf(fichier, "\n");

    fclose(fichier);

    printf("Ordonnance enregistrée avec succès dans le fichier ordonnances.csv.\n");
}

void servirOrdonnance() {
    Ordonnance o;
    saisirOrdonnance(&o);
    enregistrerOrdonnance(&o);
}

void enregistrerCommande(Commande c) {
    FILE *fichier;
    fichier = fopen("commande.csv", "a");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier de commande.\n");
        return;
    }

    // Enregistrement de la commande dans le fichier
    fprintf(fichier, "%d,%d,%s,%s,%d,%d/%d/%d\n", c.NumCommande, c.codeCl, c.nomCL, c.VilleCL, c.QtC,
            c.dateC.jour, c.dateC.mois, c.dateC.annee);

    fclose(fichier);

    printf("Commande enregistrée avec succès dans le fichier commande.csv.\n");
}


void saisirCommande(){
    Commande c;
    printf("Entrez le numéro de commande : ");
    scanf("%d", &c.NumCommande);
    
    printf("Entrez le code client : ");
    scanf("%d", &c.codeCl);
    
    printf("Entrez le nom du client : ");
    scanf("%s", c.nomCL);
    
    printf("Entrez la ville du client : ");
    scanf("%s", c.VilleCL);
    
    printf("Entrez la quantité de vente : ");
    scanf("%d", &c.QtC);
    
    printf("Entrez la date de vente (jour mois annee) : ");
    scanf("%d %d %d", &c.dateC.jour, &c.dateC.mois, &c.dateC.annee);

    enregistrerCommande(c);
}
int compareDates(date date1, date date2) {
    if (date1.annee < date2.annee) {
        return -1;
    } else if (date1.annee > date2.annee) {
        return 1;
    } else {
        if (date1.mois < date2.mois) {
            return -1;
        } else if (date1.mois > date2.mois) {
            return 1;
        } else {
            if (date1.jour < date2.jour) {
                return -1;
            } else if (date1.jour > date2.jour) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}
    
float calculerVentesPeriode(date dateDebut, date dateFin) {
    FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier de stock.\n");
        return -1.0; // Indique une erreur
    }

    produit p;
    float totalVentes = 0.0;
    char ligne[1024];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%d,%[^,],%*d,%f,%d,%*d,%d/%d/%d,%d/%d/%d,%*d/%*d/%*d,%*d,%*d,%*d\n",
               &p.ID, p.NomPr, &p.prixUni, &p.QtSt,
               &p.dateEntree.jour, &p.dateEntree.mois, &p.dateEntree.annee,
               &p.dateSortie.jour, &p.dateSortie.mois, &p.dateSortie.annee);
        
        if (compareDates(p.dateEntree, dateDebut) >= 0 && compareDates(p.dateSortie, dateFin) <= 0) {
            totalVentes += (p.QtSt * p.prixUni);
        }
    }

    fclose(fichier);
    return totalVentes;
}


void afficherVentesPeriode(float totalVentes) {
    if (totalVentes >= 0.0) {
        printf("Total des ventes pour la période spécifiée : %.2f\n", totalVentes);
    } else {
        printf("Erreur lors de l'ouverture du fichier de stock.\n");
    }
}
void statistiqueVente() {
    FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier de stock.\n");
        return;
    }

    produit p;
    char ligne[1024];
    float totalVentes = 0.0;
    int totalProduits = 0;
    float prixMoyen = 0.0;
    int produitsPerimes = 0;

    
    time_t now;
    struct tm *local;
    now = time(NULL);
    local = localtime(&now);
    int currentYear = local->tm_year + 1900;
    int currentMonth = local->tm_mon + 1;
    int currentDay = local->tm_mday;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        sscanf(ligne, "%*d,%*[^,],%*d,%f,%*d,%*d,%*d/%*d/%*d,%*d/%*d/%*d,%d/%d/%d,%*d,%*d\n",
               &p.prixUni, &p.datePeremption.jour, &p.datePeremption.mois, &p.datePeremption.annee);

        
        totalVentes += p.prixUni;

        
        totalProduits++;

        
        if (p.datePeremption.annee < currentYear ||
            (p.datePeremption.annee == currentYear && p.datePeremption.mois < currentMonth) ||
            (p.datePeremption.annee == currentYear && p.datePeremption.mois == currentMonth && p.datePeremption.jour < currentDay)) {
            produitsPerimes++;
        }
    }

    
    prixMoyen = totalVentes / totalProduits;

    printf("Statistiques de vente :\n");
    printf("Total des ventes : %.2f\n", totalVentes);
    printf("Nombre total de produits : %d\n", totalProduits);
    printf("Prix moyen des produits : %.2f\n", prixMoyen);
    printf("Nombre de produits périmés : %d\n", produitsPerimes);

    fclose(fichier);
}


int main(){	
struct User users[MAX_USERS];
    int num_users = 0;
    int choice;

    // Charger les utilisateurs à partir du fichier au démarrage
    num_users = loadUsersFromFile(users);
     
    printf("              Bienvenue dans notre programme de gestion de stocks d'une pharmacie !\n");
    sleep(2);
    printf("\n");
    printf("              Ce programme vous permettra de suivre et de gérer les stocks des produits pharmaceutiques.\n");
    sleep(2);
    printf("\n");
    printf("                        FAIT PAR: AIT RAMDAN Hiba ET ZEGAN Safaa \n");
    sleep(2);
    printf("\n");
    printf("                        ENCADRE PAR: Mme REMMACH \n");
    sleep(2);
    printf("\n");
    printf("                                                             2 AP Groupe 7 <3 \n");
    sleep(2);
    
    clearScreen();
    printf("\n");   
	 printf("\n");   
	  printf("\n");   
	   printf("\n");    
    printf("             ___________                                      \n");
    printf("            |           |                         \n");
    printf("            |           |                           \n");
    printf("            |           |                           \n");
    printf("            |           |                           \n");
    printf("  __________|           |__________                               ________________________________________________\n");
    printf(" |                                 |                             |                                                |\n");
    printf(" |                                 |                             |                     CHOIX                      |\n"); 
    printf(" |            PHARMACIE            |                             |                                                |\n");
    printf(" |                                 |                             |________________________________________________|\n");
    printf(" |__________             __________|                             |  1: Connexion.                                 |\n");
	printf("            |           |                                        |________________________________________________|\n");                                                                       
	printf("            |           |                                        |  2: Inscription.                               |\n");
	printf("            |           |                                        |________________________________________________|\n"); 
	printf("            |           |                                        |  3: Quitter.                                   | \n"); 
    printf("            |___________|                                        |________________________________________________|\n");
    
    while(1){
    	printf("\n");
    	printf("\n");
    	printf("\n");
    	printf("\n");
        printf("--------------> Entrer Votre Choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    char username[MAX_USERNAME_LENGTH];
                    char password[MAX_PASSWORD_LENGTH];

                    printf("Nom d'utilisateur : ");
                    scanf("%s", username);
                    printf("Mot de passe : ");
                    scanf("%s", password);

                    if (authenticate(users, num_users, username, password)) {
                        printf("Connexion réussie !\n");
                        goto menu; // Sortie du programme après une connexion réussie
                    } else {
                        printf("Nom d'utilisateur ou mot de passe incorrect !\n");
                    }
                }
                break;
            case 2:
                {
                    char username[MAX_USERNAME_LENGTH];
                    char password[MAX_PASSWORD_LENGTH];

                    printf("Nom d'utilisateur : ");
                    scanf("%s", username);
                    printf("Mot de passe : ");
                    scanf("%s", password);

                    if (registerUser(users, &num_users, username, password)) {
                        printf("Inscription réussie !\n");
                        saveUsersToFile(users, num_users); // Enregistrer les utilisateurs dans un fichier
                         goto menu; // Sortie du programme après une inscription réussie
                    } 
                }
                break;
            case 3:
                printf("Au revoir !\n");
                return 0; // Sortie du programme
            default:
                printf("Choix invalide. Veuillez entrer 1, 2 ou 3.\n");
        }}
    menu :
clearScreen();
	int m;
	 
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
    printf("                                                                  ________________________________________________\n");
    printf("                                                                 |                                                |\n");
    printf("                                                                 |                      MENU                      |\n"); 
    printf("                                                                 |                                                |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 |  1: saisir les medicaments.                    |\n");
	printf("                                                                 |________________________________________________|\n");                                                                       
	printf("                                                                 |  2: Ajouter un nouveau medicaments.            |\n");
	printf("                                                                 |________________________________________________|\n");  
    printf("                                                                 |  3: Supprimer medicaments par ID.              |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 |  4: Modifier un Medicaments.                   |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 |  5: Rechercher des medicaments par le Nom.     |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 |  6: trier des medicaments par prix unitaire.   |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 |  7: trier des medicaments par leur Nom.        |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 |  8: Affichage des medicaments perimee.         |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 |  9: Service ordonnance.                        |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 | 10: Analyse des vente.                         | \n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 | 11: Stasitique de vente.                       |\n");
    printf("                                                                 |________________________________________________|\n");
    printf("                                                                 | 12: Quitter.                                   |\n");
    printf("                                                                 |________________________________________________|\n");
   

    printf("---> Veuilez saisir un nombre: ");
    scanf("%d", &m);
    clearScreen();
     FILE *fichier;
    fichier = fopen("GestionDeStockDesMedicament.csv", "a"); 

    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    int nbProduits;
    
    int idMed;
    char NomMed[30];
    float totalVentes;
        switch (m) {
        	case 1:
        		printf("Combien de produits voulez-vous saisir ? ");
                scanf("%d", &nbProduits);
                for (int i = 0; i < nbProduits; ++i) {
    	            printf("\nMedicament %d\n", i + 1);
    	            saisirProduit(fichier);
    	            
    	        }
    	        break;
    	        
            case 2:
        		printf("Combien de produits voulez-vous saisir ? ");
                scanf("%d", &nbProduits);
                for (int i = 0; i < nbProduits; ++i) {
    	            printf("\nMedicament %d\n", i + 1);
    	            saisirProduit(fichier);
    	           
    	        }
    	        
    	        break;
    	        
            case 3:
        		printf("Entrer l'ID du produit que vous voulez supprimer : ");
        		scanf("%d", &idMed);
        		supprimerProduitByID(idMed);
        		goto menu;
        		
        		
        	case 4:
        		ModifierMedicament();
        		
        		break;
        		
        	case 5:
        		printf("Entrez le nom du produit que vous recherchez : ");
        		scanf("%s", NomMed);
        		rechercherProduitParNom(NomMed);
        		
        		break;
        		
            case 6:
                trierProduitsParPrixDecroissant();
                
                break;
                
            case 7:
                trierProduitsParNom();
                
                break;
                
            case 8:
                afficherProduitsPerimes();
                
                break;
            case 9:
                servirOrdonnance();
               
                break;
            case 10:
                date dateDebut, dateFin;
                printf("Entrez la date de debut de la periode d'analyse :\n");
                dateDebut = saisirDate();
                printf("Entrez la date de fin de la periode d'analyse :\n");
                dateFin = saisirDate();
                totalVentes = calculerVentesPeriode(dateDebut, dateFin);
                afficherVentesPeriode(totalVentes);
                
                break;
            case 11:
                statistiqueVente();
                
                break;   
			case 12:
                return 0;
                
                break;  
            default:
                printf("ERREUR !\n");
        }
        
    fclose(fichier);
    printf("Fichier CSV cree avec succees.\n");
	goto menu;
        return 0;
        
}

