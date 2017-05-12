#include "test.h"
#include <stdlib.h>

int main() {

    std::cout << "Performing vector tests" << std::endl;
    if (!testVectorOperations()) {
        std::cout << "Failed vector test" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Performing matrix tests" << std::endl;
    if (!testMatrixOperations()) {
        std::cout << "Failed matrix test" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "All tests passed" << std::endl;
}