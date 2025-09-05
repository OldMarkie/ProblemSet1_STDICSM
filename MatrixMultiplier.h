#pragma once
#include <vector>
#include <string>

class MatrixMultiplier {
public:
    // Reads a matrix from an input file stream until blank line
    static std::vector<std::vector<int>> readMatrix(std::ifstream& in);

    // Multiplies matrices A and B
    static std::vector<std::vector<int>> multiply(const std::vector<std::vector<int>>& A,
        const std::vector<std::vector<int>>& B);

    // Writes a matrix to output file
    static void writeMatrix(const std::vector<std::vector<int>>& matrix, const std::string& filename);

private:
    // Helper function for computing one row
    static void multiplyRow(const std::vector<std::vector<int>>& A,
        const std::vector<std::vector<int>>& B,
        std::vector<std::vector<int>>& C,
        int row);
};
