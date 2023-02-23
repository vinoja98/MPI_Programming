//mpicc -g -Wall -o mpi_square reciprocal_square_sum_mpi.c
//mpiexec -n 4 ./mpi_square

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000000

double squareReciprocalsSum(int* nums, int num) {
    double tot = 0;
    int i;
    for (i = 0; i < num; i++) {
        tot += 1.0 / (nums[i]*nums[i]);
    }
    return tot;
}

int main(void) {
    MPI_Init(NULL, NULL);

    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int local_n = N/comm_sz;

    double local_sum = 0;
    double global_sum;
    int* local_nums;
    if(my_rank==0){
        int nums[N];
        for (int i = 0; i < N; i++) {
            nums[i] = i+1;
        }
        local_nums = (int*) malloc(sizeof(int*) * local_n);
        MPI_Scatter(nums, local_n, MPI_INT, local_nums, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        local_sum = squareReciprocalsSum(local_nums, local_n);
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  
    }
    else{
        local_nums = (int*) malloc(sizeof(int*) * local_n);
        MPI_Scatter(NULL, local_n, MPI_INT, local_nums, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        local_sum = squareReciprocalsSum(local_nums, local_n);
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        printf("Sum of the reciprocals of the first one million squared numbers: %f\n", global_sum);
    }

    free(local_nums);
    MPI_Finalize();
    return 0;
}
