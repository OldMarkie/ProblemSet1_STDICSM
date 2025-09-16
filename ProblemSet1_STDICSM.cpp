#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "MatrixMultiplier.h"

using namespace std;
using namespace std::chrono;

int main() {
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Error: Could not open input file.\n";
        return 1;
    }

    auto A = MatrixMultiplier::readMatrix(input);
    auto B = MatrixMultiplier::readMatrix(input);
    input.close();

    try {
        unsigned int hwThreads = std::thread::hardware_concurrency();
        if (hwThreads == 0)
            std::cout << "Could not detect hardware concurrency." << std::endl;
        else
            std::cout << "Hardware concurrency (threads available): "
            << hwThreads << std::endl;

        // Standard version timing
        auto start1 = high_resolution_clock::now();
        auto C1 = MatrixMultiplier::multiplyStandard(A, B);
        auto end1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(end1 - start1).count();

        // Multithreaded version timing
        auto start2 = high_resolution_clock::now();
        auto C2 = MatrixMultiplier::multiplyParallel(A, B);
        auto end2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(end2 - start2).count();

        cout << "Standard Running time: " << duration1 << " microsecond\n";
        cout << "Multithreaded Running time: " << duration2 << " microsecond\n";

        // Save multithreaded result only
        MatrixMultiplier::writeMatrix(C2, "output.txt");
        cout << "Matrix multiplication completed. Result saved to output.txt\n";
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
