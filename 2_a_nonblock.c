#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    double startTime, endTime;
    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int i, j, res, n_local, n;
    int *prefixSum;
	int *sendBuf;
    MPI_Request *req; MPI_Status *stat;
	int showResult = 1;


    // MPI Initialize
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);

	req = (MPI_Request*) malloc( sizeof(MPI_Request) * numprocs);
	stat = (MPI_Status*) malloc( sizeof(MPI_Status) * numprocs);

    n = atoi(argv[1]);
    if (n<numprocs)	numprocs = n;

    n_local = n / numprocs;
    if (rank == numprocs-1) {
        n_local +=  n % numprocs;
    }

    prefixSum = (int*) malloc(sizeof(int) * n_local);
	
	if ( rank == 0 ) { /* master */

		startTime = MPI_Wtime();

		sendBuf = (int*) malloc(sizeof(int) * n);

		if(showResult)	printf("generated numbers : ");
		srand(time(NULL));
		for ( i=0; i<n; i++) {
			sendBuf[i] = rand() % 10;
			if(showResult)	printf("%d ", sendBuf[i]);
		}
		if(showResult)	printf("\n");

		// send random numbers
		for (i=0; i<numprocs; i++) {
			if ( i == 0 ) {
				for (j = 0; j<n_local; j++)	prefixSum[j] = sendBuf[j];
			} else if ( i == numprocs-1) {
				MPI_Isend(sendBuf+n_local*i, n_local + n % numprocs, MPI_INT, i, 0, MPI_COMM_WORLD, req+i-1);
			} else {
				MPI_Isend(sendBuf+n_local*i, n_local, MPI_INT, i, 0, MPI_COMM_WORLD, req+i-1);
			}
		}

	} else { /* worker */
		// receive random numbers
		MPI_Irecv(prefixSum, n_local, MPI_INT, 0, 0, MPI_COMM_WORLD, req);
		MPI_Wait(req, stat);
	}

	// receive previous node's data
	if (rank != 0) {
		MPI_Irecv(&res, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD, req);
		MPI_Wait(req, stat);
		prefixSum[0] += res;
	}
	
	// calculate prefix sum
	for (i=1; i<n_local; i++) {
		prefixSum[i] += prefixSum[i-1];
	}

	// send result to next node
	if (rank != numprocs-1)
		MPI_Isend(&prefixSum[n_local-1], 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD, req);

	// send result to master node
	if (rank == 0) { /* master */
		for (i=0; i<numprocs; i++) {
			if (i==0) {
				for (j=0; j<n_local; j++)	sendBuf[j] = prefixSum[j];
			} else if (i == numprocs-1) {
				MPI_Irecv(sendBuf+n_local*i, n_local + n % numprocs, MPI_INT, i, 2, MPI_COMM_WORLD, req+i-1);
			} else {
				MPI_Irecv(sendBuf+n_local*i, n_local , MPI_INT, i, 2, MPI_COMM_WORLD, req+i-1);
			}
		}		
		
		MPI_Waitall(numprocs-1, req, stat);
		if(showResult){
			printf("prefix Sum : ");
			for (i=0; i<n; i++) {
				printf("%d ", sendBuf[i]);
			}
			printf("\n");
		}

		endTime = MPI_Wtime();
		double executeTime = (endTime - startTime);
		printf("total execute time : %lf\n", executeTime);

	} else {
		MPI_Isend(prefixSum, n_local, MPI_INT, 0, 2, MPI_COMM_WORLD, req+1);
		MPI_Waitall(2,req,stat);
	}

	MPI_Finalize();

	return 0;
}

