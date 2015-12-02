#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

enum req {
	LECTURE=0,
	ECRITURE_PAR_CLIENT=1,
	ECRITURE_PAR_SERVEUR=2,
	REPONSE_CLIENT=3
};

int main(int argc, char** argv)
{
	MPI_Init(NULL, NULL);
	
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	char nom_machine[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(nom_machine, &name_len);

	if (world_size < 4) {
		fprintf(stderr, "Minimum 3 machines\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	int requete;
	int msg;
	int ordre = 0;

	if (world_rank == 0) {
		// envoyer des requetes
		printf("[%s :: %i] Je demande de voir un produit\n", nom_machine, world_rank);
		msg = LECTURE;
		MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);	
		printf("[%s :: %i] Je reçoit la description du produit\n", nom_machine, world_rank);
		MPI_Recv(&requete, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("[%s :: %i] Je demande d\'acheter le produit\n", nom_machine, world_rank);
		msg = ECRITURE_PAR_CLIENT;
		MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		
	} else {
		while (1) {
			MPI_Recv(&requete, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("[DEBUG][%s :: %i] received %i\n", nom_machine, world_rank, requete);
			switch(requete) {
				case LECTURE:
					printf("[%s :: %i] Client demande une lecture sur machine\n", nom_machine, world_rank);
					msg = REPONSE_CLIENT;
					MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
					break;
				case ECRITURE_PAR_CLIENT:
					printf("[%s :: %i] Client demande une ecriture sur machine\n", nom_machine, world_rank);
					for (int i = 1; i < world_size; i++) {
						msg = ECRITURE_PAR_SERVEUR;
						if (i != world_rank) {
							MPI_Send(&msg, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
						}
					}
					break;
				case ECRITURE_PAR_SERVEUR:
					printf("[%s :: %i] Serveur demande une ecriture sur machine\n", nom_machine, world_rank);
					break;
			}
		}
	}

	MPI_Finalize();
}
