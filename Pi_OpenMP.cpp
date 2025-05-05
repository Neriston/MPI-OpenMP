#include <omp.h>

double calculate_pi_openmp(long long n_terms, int num_threads) {
    double pi = 0.0;
    double numerator = 1.0;
    
    #pragma omp parallel for reduction(+:pi) num_threads(num_threads)
    for (long long i = 0; i < n_terms; ++i) {
        double denominator = 2 * i + 1;
        double term = numerator / denominator;
        if (i % 2 == 0) {
            pi += term;
        } else {
            pi -= term;
        }
    }
    
    return 4 * pi;
}

void test_openmp() {
    const long long n_terms = 100'000'000;
    int max_threads = omp_get_max_threads();
    
    for (int threads = 1; threads <= max_threads; threads *= 2) {
        omp_set_num_threads(threads);
        
        auto start = std::chrono::high_resolution_clock::now();
        double pi = calculate_pi_openmp(n_terms, threads);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> duration = end - start;
        
        std::cout.precision(15);
        std::cout << "π ≈ " << pi 
                  << ", время: " << duration.count() << " сек"
                  << ", потоков: " << threads << std::endl;
    }
}
