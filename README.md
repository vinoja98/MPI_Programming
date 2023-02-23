# Compilation
mpicc -g -Wall -o mpi_fib reciprocal_fibonacci_sum_mpi.c

#Execution
mpiexec -n 4 ./mpi_fib
