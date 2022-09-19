#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h> 
#include <unistd.h>
#include <math.h>

// Fonction permettant d'afficher les 10 premiers elements d'une liste
int print_liste(float* liste) {
    int loop;

    for(loop = 0; loop < 10; loop++)
        printf("%f ", liste[loop]);
    
    printf("\n");
    return 0;
}

// Fonction calculcant la moyenne d'une liste de 10 elements
float moyenne(float* liste) {
    float moyenne = 0;
    for(int i = 0; i < 10; i++)
        moyenne += liste[i];
    
    moyenne /= 10;
    return moyenne;
}


int main(int argc, char* argv[]) {
    int my_rank; /* rang du processus */
    int p; /* nombre de processus */
    int source; /* rang de l’emetteur */
    int dest; /* rang du recepteur */
    int tag = 0; /* etiquette du message */
    char message[100];
    
    
    MPI_Status status;
    /* Initialisation */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    // printf("Processus n° : %d créé\n", my_rank);
    
    // Creation des differentes table de calcul
    float table[p*10]; 
    float table_recu[10];
    float table_moyenne[10];
    

    if (my_rank == 0) {
        /* Creation du tableau d'entiers */
        for (int i = 0; i<p*10; i++) {
            table[i] = ((float)rand()/(float)(RAND_MAX)) * 20;
        }
    }

    /* Utilisation de MPI_SCATTER pour répartir le tableau de données entre tous les processus */
    MPI_Scatter(table, 10, MPI_FLOAT, table_recu, 10, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* Calcul de la moyenne pour chaque sous divison de la table*/
    float moy = moyenne(table_recu);
    printf("La moyenne du tableau du processeur %i est %f\n", my_rank, moy);
    MPI_Gather(&moy, 1, MPI_FLOAT, table_moyenne, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        float moy_tab = moyenne(table_moyenne);
        printf("La moyenne finale est de : %f\n\n", moy_tab);
    }
    

    /* Desactivation */
    MPI_Finalize();
    return 0;
}