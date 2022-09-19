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

    //Cette fois ci, il faut que les processus autre que 0 recoivent les messages avant d'en envoyer

    /* Initialisation envoi du message par le processus 0 */

    if (my_rank == 0) {
        /* Creation du message */
        printf("Processus n°%d\n", my_rank);
        sprintf(message, "Coucou du processus %d!", my_rank);
        dest = my_rank+1;
        MPI_Ssend(message, strlen(message)+1, MPI_CHAR,
        dest, tag, MPI_COMM_WORLD);
        /* Préparation de la réception d'un message */
        MPI_Recv(message, 100, MPI_CHAR, p-1, tag,
        MPI_COMM_WORLD, &status);
        printf("%s\n", message);
    }

    if (my_rank < p-1 && my_rank != 0) {
        /* Préparation de la réception d'un message */
        MPI_Recv(message, 100, MPI_CHAR, my_rank-1, tag,
        MPI_COMM_WORLD, &status);
        printf("%s\n", message);
        /* Creation du message */
        printf("Processus n°%d\n", my_rank);
        sprintf(message, "Coucou du processus %d!", my_rank);
        dest = my_rank+1;
        MPI_Ssend(message, strlen(message)+1, MPI_CHAR,
        dest, tag, MPI_COMM_WORLD);
        printf("Message envoyé");
        
    } else {
        /* Préparation de la réception d'un message */
        MPI_Recv(message, 100, MPI_CHAR, my_rank-1, tag,
        MPI_COMM_WORLD, &status);
        printf("%s\n", message);
        /* Creation du message */
        printf("Processus n°%d\n", my_rank);
        sprintf(message, "Coucou du processus %d!", my_rank);
        dest = 0;
        MPI_Ssend(message, strlen(message)+1, MPI_CHAR,
        dest, tag, MPI_COMM_WORLD);
        
    }
    /* Desactivation */
    MPI_Finalize();
    return 0;
}