#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    double startTime, endTime;
    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int i, n, n_local;
	int *prefixSum, localSum=0, *sendBuf, recvBuf;
    MPI_Request req; MPI_Status stat;
    int showResult = 1;
	void* attr_value;
	int flag;

    startTime = MPI_Wtime();

    // MPI Initialize
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);

    n = atoi(argv[1]);
    if (n<numprocs)	numprocs = n;

    n_local = n / numprocs;
    if (rank == numprocs-1) {
        n_local += n % numprocs;
    }
    
    int *cnt, *disp;
    cnt = (int*) malloc(sizeof(int) * numprocs);
    disp = (int*) malloc(sizeof(int) * numprocs);

    for (i=0; i<numprocs; i++) {
        cnt[i]=n_local;
        disp[i]=n_local*i;
        if (i == numprocs-1) {
            cnt[i] += n % numprocs;
        }
    }

    prefixSum = (int*) malloc(sizeof(int) * n_local);

	if ( rank == 0) { /* master */
		
		sendBuf = (int*) malloc(sizeof(int) * n);

		// generate random numbers
		if(showResult)  printf("generated numbers : ");
		srand(time(NULL));
		for ( i=0; i<n; i++) {
			sendBuf[i] = rand() % 10;
			if(showResult)  printf("%d ", sendBuf[i]);
		}
		if(showResult)  printf("\n");
    }

    MPI_Scatterv(sendBuf, cnt, disp, MPI_INT, prefixSum, n_local, MPI_INT, 0, MPI_COMM_WORLD);

    for ( i=0; i<n_local; i++) {
        localSum += prefixSum[i];
    }

    MPI_Scan(&localSum, &recvBuf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    prefixSum[0] += recvBuf - localSum;

    for( i=1; i<n_local; i++ ) {
        prefixSum[i] += prefixSum[i-1];
    }

    MPI_Gatherv(prefixSum, n_local, MPI_INT, sendBuf, cnt, disp, MPI_INT, 0, MPI_COMM_WORLD);

    if ( rank == 0) {
        if(showResult){
            printf("prefix sum : ");
            for( i=0; i<n; i++ ) printf("%d ", sendBuf[i]);
            printf("\n");
        }

        endTime = MPI_Wtime();
        double executeTime = (endTime - startTime);
        printf("total execute time : %lf\n", executeTime);
    }

    MPI_Finalize();
    return 0;
}

