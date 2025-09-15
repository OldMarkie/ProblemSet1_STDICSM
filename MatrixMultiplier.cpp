#include "MatrixMultiplier.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <iomanip>
#include <mutex>

using namespace std;

std::mutex coutMutex;

vector<vector<double>> MatrixMultiplier::readMatrix(ifstream& in) {
    vector<vector<double>> matrix;
    string line;
    while (getline(in, line)) {
        if (line.empty()) break;
        stringstream ss(line);
        vector<double> row;
        double val;
        while (ss >> val) row.push_back(val);
        matrix.push_back(row);
    }
    return matrix;
}

vector<vector<double>> MatrixMultiplier::multiplyStandard(
    const vector<vector<double>>& A,
    const vector<vector<double>>& B)
{
    if (A.empty() || B.empty())
        throw invalid_argument("Error: One of the matrices is empty.");

    size_t aCols = A[0].size();
    size_t bCols = B[0].size();

    if (aCols != B.size())
        throw invalid_argument("Error: Invalid dimensions. Matrix A columns must equal Matrix B rows.");

    int m = A.size();
    int n = bCols;
    vector<vector<double>> C(m, vector<double>(n, 0.0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0;
            for (int k = 0; k < aCols; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

void MatrixMultiplier::multiplyRow(const vector<vector<double>>& A,
    const vector<vector<double>>& B,
    vector<vector<double>>& C,
    int row) {
    int n = B[0].size();
    int k = B.size();
    for (int j = 0; j < n; j++) {
        double sum = 0;
        for (int x = 0; x < k; x++) {
            sum += A[row][x] * B[x][j];
        }
        C[row][j] = sum;
    }
}

vector<vector<double>> MatrixMultiplier::multiplyParallel(
    const vector<vector<double>>& A,
    const vector<vector<double>>& B)
{
    if (A.empty() || B.empty())
        throw invalid_argument("Error: One of the matrices is empty.");

    size_t aCols = A[0].size();
    size_t bCols = B[0].size();

    if (aCols != B.size())
        throw invalid_argument("Error: Invalid dimensions. Matrix A columns must equal Matrix B rows.");

    int m = A.size();
    int n = bCols;
    vector<vector<double>> C(m, vector<double>(n, 0.0));

    vector<thread> threads;
    for (int i = 0; i < m; i++) {
        threads.emplace_back(multiplyRow, cref(A), cref(B), ref(C), i);
    }
    for (auto& t : threads) t.join();

    return C;
}

void MatrixMultiplier::writeMatrix(const vector<vector<double>>& matrix,
    const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: Could not open output file.\n";
        return;
    }

    for (const auto& row : matrix) {
        for (size_t j = 0; j < row.size(); j++) {
            out << fixed << setprecision(1) << row[j]
                << (j + 1 == row.size() ? "" : " ");
        }
        out << "\n";
    }
    out.close();
}
