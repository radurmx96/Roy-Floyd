#include "stdafx.h"
#include<time.h>
#include<mpi.h>
#include<iostream>

using namespace std;

#define N		4
#define INF		9999
#define MASTER	0

int G[N][N] = {
	0,9999,5,9999,
	1,0,0,9999,
	6,9999,0,5,
	7,9,9999,7,
};

int main(int argc, char **argv)
{
	int  num_procs, proc_id;
	int i, j, k;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

	for (k = 0; k < N; k++)
	{
		for (j = 0; j < N; j++)
		{
			if (G[proc_id][j] > G[proc_id][k] + G[k][j])
			{
				G[proc_id][j] = G[proc_id][k] + G[k][j];
			}
		}
		MPI_Allgather(G[proc_id], N, MPI_INT, G, N, MPI_INT, MPI_COMM_WORLD);
	}
	if (MASTER == proc_id)
	{
		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
					printf("%d ", G[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
	return 0;
}
