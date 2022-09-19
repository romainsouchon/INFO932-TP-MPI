#include <stdio.h>
#include <string.h>
#include <mpi.h>
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
    printf("Nombre de processus : %d\n", p);

    if (my_rank == 1) {
        /* Creation du tableau d'entiers */
        int table[p]; 
        for (int i = 0; i<p; i++)
        {
            table[i] = rand() % 20; //On rempli le tableau avec les entier aléatoires
        }
        /* Creation du message */
        printf("Processus n°%d\n", my_rank);
        for (int i = 0; i<p; i++) //Pour tous les processus
        {
            if (i ==1){}
            else {
            sprintf(message, "Processus n° %i a reçu l'entier : %i", i , table[i]);
            dest = i;
            MPI_Ssend(message, strlen(message)+1, MPI_CHAR,
            dest, tag, MPI_COMM_WORLD);
            }
        }
    }
        
    else {
        /* Préparation de la réception d'un message */
        MPI_Recv(message, 100, MPI_CHAR, 1, tag,
        MPI_COMM_WORLD, &status);
        printf("%s\n", message); //Affiche l'entier reçu
    }
    /* Desactivation */
    MPI_Finalize();
    return 0;
}