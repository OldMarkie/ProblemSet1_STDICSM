#include <iostream>
#include <fstream>
#include "MatrixMultiplier.h"

using namespace std;

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
        auto C = MatrixMultiplier::multiply(A, B);
        MatrixMultiplier::writeMatrix(C, "output.txt");
        cout << "Matrix multiplication completed. Result saved to output.txt\n";
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
