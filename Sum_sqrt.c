#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv) {
    int rank;
    int size, k, local_sum, total_sum, elements_per_process;
    int source, destination;
    int tag1 = 50, tag2 = 60, tag3 = 70;
    int total_elements;
    int data[100];
    int local_data[100];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter number of elements:\n");
        scanf("%d", &total_elements);
        printf("Enter %d numbers:\n", total_elements);
        for (k = 0; k < total_elements; k++)
            scanf("%d", &data[k]);
        
        for (destination = 1; destination < size; destination++)
            MPI_Send(&total_elements, 1, MPI_INT, destination, tag1, MPI_COMM_WORLD);
        
        elements_per_process = total_elements / size;
        k = elements_per_process;
        
        for (destination = 1; destination < size; destination++) {
            MPI_Send(&data[k], elements_per_process, MPI_INT, destination, tag2, MPI_COMM_WORLD);
            k += elements_per_process;
        }
        
        for (k = 0; k < elements_per_process; k++)
            local_data[k] = data[k];
    }
    else {
        MPI_Recv(&total_elements, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);
        elements_per_process = total_elements / size;
        MPI_Recv(&local_data[0], elements_per_process, MPI_INT, 0, tag2, MPI_COMM_WORLD, &status);
    }

    local_sum = 0;
    for (k = 0; k < elements_per_process; k++)
        local_sum += (local_data[k] * local_data[k]);

    if (rank != 0) {
        MPI_Send(&local_sum, 1, MPI_INT, 0, tag3, MPI_COMM_WORLD);
    }
    else {
        total_sum = local_sum;
        printf("\n Result from process %d: %d\n", rank, local_sum);
        for (source = 1; source < size; source++) {
            MPI_Recv(&local_sum, 1, MPI_INT, source, tag3, MPI_COMM_WORLD, &status);
            total_sum += local_sum;
            printf("\n Result from process %d: %d\n", source, local_sum);
        }
        printf("\n\n Final Result: %d\n", total_sum);
    }

    MPI_Finalize();
    return 0;
}