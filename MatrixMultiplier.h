#pragma once
#include <vector>
#include <string>
#include <fstream>

class MatrixMultiplier {
public:
    // Reads a matrix from input file stream
    static std::vector<std::vector<double>> readMatrix(std::ifstream& in);

    // Single-threaded multiplication
    static std::vector<std::vector<double>> multiplyStandard(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B);

    // Multithreaded multiplication
    static std::vector<std::vector<double>> multiplyParallel(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B);

    // Writes matrix to file (1 decimal place)
    static void writeMatrix(const std::vector<std::vector<double>>& matrix,
        const std::string& filename);

private:
    // Helper function for parallel row computation
    static void multiplyRow(const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B,
        std::vector<std::vector<double>>& C,
        int row);
};
