#include "MatrixMultiplier.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <iomanip>
#include <mutex>

using namespace std;

std::mutex coutMutex;

static const int MAX_DIM = 1000;

vector<vector<double>> MatrixMultiplier::readMatrix(ifstream& in) {
    vector<vector<double>> matrix;
    string line;
    while (getline(in, line)) {
        if (line.empty()) break;
        stringstream ss(line);
        vector<double> row;
        double val;
        while (ss >> val) row.push_back(val);

        if ((int)row.size() > MAX_DIM) {
            throw invalid_argument("Error: Matrix exceeds maximum column size of 1000.");
        }

        matrix.push_back(row);

        if ((int)matrix.size() > MAX_DIM) {
            throw invalid_argument("Error: Matrix exceeds maximum row size of 1000.");
        }
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

    int m = A.size();        // rows of A
    int n = bCols;           // cols of B

    unsigned int hwThreads = std::thread::hardware_concurrency();
    if (hwThreads == 0) hwThreads = 2; 

    int numThreads = std::min<int>(m, hwThreads);

    int blockSize = (m + numThreads - 1) / numThreads;

    vector<vector<double>> C(m, vector<double>(n, 0.0));

    auto worker = [&](int rowStart, int rowEnd) {
        for (int i = rowStart; i < rowEnd; i++) {
            for (int j = 0; j < n; j++) {
                double sum = 0.0;
                for (int k = 0; k < aCols; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
        };

    vector<thread> threads;

    for (int t = 0; t < numThreads; t++) {
        int rowStart = t * blockSize;
        int rowEnd = min(rowStart + blockSize, m);
        if (rowStart < m) {
            threads.emplace_back(worker, rowStart, rowEnd);
        }
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
