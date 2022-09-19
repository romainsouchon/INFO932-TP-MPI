#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h> 
#include <unistd.h>
#include <math.h>

// Fonction permettant d'afficher les 10 premiers elements d'une liste
int print_liste(float* liste, int n) {
    int loop;

    for(loop = 0; loop < n; loop++)
        printf("%f ", liste[loop]);
    
    printf("\n");
    return 0;
}

// Fonction calculcant la moyenne d'une liste de 10 elements
float moyenne_finale(float* liste, int p) {
    float moyenne = 0;
    for(int i = 0; i < p; i++)
        moyenne += liste[i];
    
    moyenne /= p;
    return moyenne;
}

// Fonction calculcant la moyenne d'une liste de 10 elements
float moyenne(float* liste, int n) {
    float moyenne = 0;
    for(int i = 0; i < n; i++)
        moyenne += liste[i];
    
    moyenne /= n;
    return moyenne;
}



int main(int argc, char* argv[]) {
    int my_rank; /* rang du processus */
    int p; /* nombre de processus */
    int source; /* rang de l’emetteur */
    int dest; /* rang du recepteur */
    int tag = 0; /* etiquette du message */
    char message[100];
    int nb_int_per_proc = 10000;
    
    
    MPI_Status status;
    /* Initialisation */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    printf("Processus n° : %d créé\n", my_rank);
    
    // Creation des differentes table de calcul
    float table[p*nb_int_per_proc]; 
    float table_recu[nb_int_per_proc];
    float table_moyenne[nb_int_per_proc];


    if (my_rank == 0) {
        /* Creation du tableau d'entiers */
        for (int i = 0; i<p*nb_int_per_proc; i++) {
            table[i] = ((float)rand()/(float)(RAND_MAX)) * 20;
        }
    }

    /* Utilisation de MPI_SCATTER pour répartir le tableau de données entre tous les processus */
    MPI_Scatter(table, nb_int_per_proc, MPI_FLOAT, table_recu, nb_int_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* Calcul de la moyenne pour chaque sous divison de la table*/
    float moy = moyenne(table_recu, nb_int_per_proc);
    printf("La moyenne du tableau du processeur %i est %f\n", my_rank, moy);
    MPI_Gather(&moy, 1, MPI_FLOAT, table_moyenne, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        float moy_tab = moyenne_finale(table_moyenne, p);
        printf("La moyenne finale est de : %f\n\n", moy_tab);
    }
    

    /* Desactivation */
    MPI_Finalize();
    return 0;
}