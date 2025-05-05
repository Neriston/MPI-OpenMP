#include <iostream>
#include <chrono>

double calculate_pi_sequential(long long n_terms) {
    double pi = 0.0;
    double numerator = 1.0;
    
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

void test_sequential() {
    const long long n_terms = 100'000'000;
    
    auto start = std::chrono::high_resolution_clock::now();
    double pi = calculate_pi_sequential(n_terms);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    
    std::cout.precision(15);
    std::cout << "π ≈ " << pi 
              << ", время: " << duration.count() << " сек" << std::endl;
}
