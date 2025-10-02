#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel
    std::cout << "Hello from " << omp_get_thread_num() << "\n";
}
