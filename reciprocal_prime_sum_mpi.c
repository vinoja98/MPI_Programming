//mpicc -g -Wall -o mpi_fib reciprocal_prime_sum_mpi.c
//mpiexec -n 4 ./mpi_fib

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000000

double fibonacciReciprocalsSum(int* fibonacci, int num) {
    double tot = 0;
    int i;
    for (i = 0; i < num; i++) {
        tot += 1.0 / fibonacci[i];
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
    int* local_fibonacci;
    if(my_rank==0){
        int fibonacci[N];
        fibonacci[0] = 1;
        fibonacci[1] = 1;
        int i;
        for (i = 2; i < N; i++) {
            fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
        }
        local_fibonacci = (int*) malloc(sizeof(int*) * local_n);
        MPI_Scatter(fibonacci, local_n, MPI_INT, local_fibonacci, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        local_sum = fibonacciReciprocalsSum(local_fibonacci, local_n);
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  
    }
    else{
        local_fibonacci = (int*) malloc(sizeof(int*) * local_n);
        MPI_Scatter(NULL, local_n, MPI_INT, local_fibonacci, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        local_sum = fibonacciReciprocalsSum(local_fibonacci, local_n);
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        printf("Sum of the reciprocals of the first one million Fibonacci numbers: %f\n", global_sum);
    }

    free(local_fibonacci);
    MPI_Finalize();
    return 0;
}
