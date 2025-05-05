#include <mpi.h>

double calculate_pi_mpi(long long n_terms) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    long long terms_per_process = n_terms / size;
    long long start_index = rank * terms_per_process;
    long long end_index = (rank == size - 1) ? n_terms : start_index + terms_per_process;
    
    double local_pi = 0.0;
    double numerator = 1.0;
    
    for (long long i = start_index; i < end_index; ++i) {
        double denominator = 2 * i + 1;
        double term = numerator / denominator;
        if (i % 2 == 0) {
            local_pi += term;
        } else {
            local_pi -= term;
        }
    }
    
    double global_pi;
    MPI_Reduce(&local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        return 4 * global_pi;
    } else {
        return 0.0;
    }
}

void test_mpi() {
    const long long n_terms = 100'000'000;
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    auto start = std::chrono::high_resolution_clock::now();
    double pi = calculate_pi_mpi(n_terms);
    auto end = std::chrono::high_resolution_clock::now();
    
    if (rank == 0) {
        std::chrono::duration<double> duration = end - start;
        
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        
        std::cout.precision(15);
        std::cout << "π ≈ " << pi 
                  << ", время: " << duration.count() << " сек"
                  << ", процессов: " << size << std::endl;
    }
}
