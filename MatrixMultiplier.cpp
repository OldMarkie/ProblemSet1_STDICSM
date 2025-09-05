#include "MatrixMultiplier.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

std::mutex coutMutex;

vector<vector<int>> MatrixMultiplier::readMatrix(ifstream& in) {
    vector<vector<int>> matrix;
    string line;
    while (getline(in, line)) {
        if (line.empty()) break;
        stringstream ss(line);
        vector<int> row;
        int val;
        while (ss >> val) row.push_back(val);
        matrix.push_back(row);
    }
    return matrix;
}

void MatrixMultiplier::multiplyRow(const vector<vector<int>>& A,
    const vector<vector<int>>& B,
    vector<vector<int>>& C,
    int row) {
    int n = B[0].size();
    int k = B.size();
    for (int j = 0; j < n; j++) {
        int sum = 0;
        for (int x = 0; x < k; x++) {
            sum += A[row][x] * B[x][j];
        }
        C[row][j] = sum;

        // Debug
        std::lock_guard<std::mutex> guard(coutMutex);
        cout << "Thread " << row
            << " computed C[" << row << "][" << j
            << "] = " << C[row][j] << "\n";

    }
}

vector<vector<int>> MatrixMultiplier::multiply(const vector<vector<int>>& A,
    const vector<vector<int>>& B) {

    if (A.empty() || B.empty()) {
        throw invalid_argument("Error: One of the matrices is empty.");
    }

    // Check A if all rows same length
    size_t aCols = A[0].size();
    for (const auto& row : A) {
        if (row.size() != aCols) {
            throw invalid_argument("Error: Matrix A is not rectangular.");
        }
    }

    // Check B if all rows same length
    size_t bCols = B[0].size();
    for (const auto& row : B) {
        if (row.size() != bCols) {
            throw invalid_argument("Error: Matrix B is not rectangular.");
        }
    }

    // Check multiplication rule: A = m * k, B = k * n
    if (aCols != B.size()) {
        throw invalid_argument("Error: Invalid dimensions. "
            "Matrix A columns must equal Matrix B rows.");
    }


    int m = A.size();
    int n = B[0].size();
    vector<vector<int>> C(m, vector<int>(n, 0));

    vector<thread> threads;
    for (int i = 0; i < m; i++) {
        threads.emplace_back(multiplyRow, cref(A), cref(B), ref(C), i);
    }

    for (auto& t : threads) t.join();

    return C;
}

void MatrixMultiplier::writeMatrix(const vector<vector<int>>& matrix, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: Could not open output file.\n";
        return;
    }

    cout << "\n===== Result Matrix C =====\n";

    for (const auto& row : matrix) {
        for (size_t j = 0; j < row.size(); j++) {
            // Write to file
            out << row[j] << (j + 1 == row.size() ? "" : " ");
            // Print to console
            cout << row[j] << (j + 1 == row.size() ? "" : " ");
        }
        out << "\n";
        cout << "\n";
    }

    cout << "===========================\n\n";
    cout << "Result also saved to " << filename << "\n";

    out.close();
}

