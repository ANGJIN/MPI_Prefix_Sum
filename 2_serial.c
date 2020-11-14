#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    clock_t startTime, endTime;
    int i, n;
    int *prefixSum;
    int showResult = 1;

    startTime = clock();

    n = atoi(argv[1]);

    prefixSum = (int*) malloc(sizeof(int) * n);

    if(showResult)  printf("random numbers : ");
    srand(time(NULL));
    for ( i=0; i<n; i++) {
        prefixSum[i] = rand() % 10;
        if(showResult)  printf("%d ", prefixSum[i]);
		if (i>0) prefixSum[i] += prefixSum[i-1];
    }
    if(showResult)  printf("\n");

    if(showResult){
        printf("prefix sum : ");
        for( i=0; i<n; i++ ) printf("%d ", prefixSum[i]);
        printf("\n");
    }


    endTime = clock();
    double executeTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
	printf("total execute time : %f\n", executeTime);

    return 0;
}

