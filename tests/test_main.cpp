// custom headers
#include "../include/BigInt.hpp"
#include "generate_input.hpp"

// necessary headers
#include <omp.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <gmpxx.h>  // Use gmpxx.h for mpz_class

using namespace std;

// Define constants
typedef int TileSize;       // Tile size for tiling
typedef int LoadBalancingFactor; // Load balancing factor
typedef int NumThreads;     // Number of threads

// Constants (can be adjusted based on hardware)
// const TileSize TILE_SIZE = 32;          // Tile size for rows and columns
const LoadBalancingFactor LOAD_BALANCE_FACTOR = 2; // Load balancing factor
const NumThreads NUM_THREADS = 24;        // Number of threads

// Matrix multiplication for BigInt
void matmulRowColParallel(vector<vector<BigInt>> &left,
                          vector<vector<BigInt>> &right,
                          vector<vector<BigInt>> &result) {
    int size = left.size();
    int TILE_SIZE = size > NUM_THREADS ? size / NUM_THREADS : size;

    #pragma omp parallel for shared(result, left, right, size, TILE_SIZE) default(none) \
        collapse(2) num_threads(NUM_THREADS) schedule(static, LOAD_BALANCE_FACTOR)
    for (int rowTile = 0; rowTile < size; rowTile += TILE_SIZE) {
        for (int colTile = 0; colTile < size; colTile += TILE_SIZE) {
            for (int innerTile = 0; innerTile < size; innerTile += TILE_SIZE) {
                int rowTileEnd = min(rowTile + TILE_SIZE, size);
                int colTileEnd = min(colTile + TILE_SIZE, size);
                int innerTileEnd = min(innerTile + TILE_SIZE, size);

                for (int row = rowTile; row < rowTileEnd; row++) {
                    for (int inner = innerTile; inner < innerTileEnd; inner++) {
                        for (int col = colTile; col < colTileEnd; col++) {
                            result[row][col] = result[row][col] + left[row][inner] * right[inner][col];
                        }
                    }
                }
            }
        }
    }
}

// Matrix multiplication for GMP's mpz_class
void matmulRowColParallelGMP(vector<vector<mpz_class>> &left,
                             vector<vector<mpz_class>> &right,
                             vector<vector<mpz_class>> &result) {
    int size = left.size();
    int TILE_SIZE = size > NUM_THREADS ? size / NUM_THREADS : size;
    #pragma omp parallel for shared(result, left, right, size, TILE_SIZE) default(none) \
        collapse(2) num_threads(NUM_THREADS) schedule(static, LOAD_BALANCE_FACTOR)
    for (int rowTile = 0; rowTile < size; rowTile += TILE_SIZE) {
        for (int colTile = 0; colTile < size; colTile += TILE_SIZE) {
            for (int innerTile = 0; innerTile < size; innerTile += TILE_SIZE) {
                int rowTileEnd = min(rowTile + TILE_SIZE, size);
                int colTileEnd = min(colTile + TILE_SIZE, size);
                int innerTileEnd = min(innerTile + TILE_SIZE, size);

                for (int row = rowTile; row < rowTileEnd; row++) {
                    for (int inner = innerTile; inner < innerTileEnd; inner++) {
                        for (int col = colTile; col < colTileEnd; col++) {
                            // result[row][col] += left[row][inner] * right[inner][col];
                            mpz_addmul(result[row][col].get_mpz_t(), left[row][inner].get_mpz_t(), right[inner][col].get_mpz_t());
                        }
                    }
                }
            }
        }
    }
}

void naivematrixmultiplication(vector<vector<BigInt>> &matrix1, vector<vector<BigInt>> &matrix2, vector<vector<BigInt>> &result)
{
    for(int i = 0; i < matrix1.size(); i++)
    {
        vector<BigInt> row;
        for(int j = 0; j < matrix2[0].size(); j++)
        {
            BigInt sum = 0;
            for(int k = 0; k < matrix1[0].size(); k++)
            {
                sum = sum + matrix1[i][k] * matrix2[k][j];
            }
            row.push_back(sum);
        }
        result.push_back(row);
    }
}

int main() 
{
    // Matrix sizes and integer lengths to test
    vector<size_t> matrixSizes = {64, 128, 256, 512, 768, 1024};
    vector<size_t> integerLengths = {256, 340, 512, 768, 1024};

    // vector<size_t> matrixSizes = {8, 16, 32, 64, 128, 256};
    // vector<size_t> integerLengths = {4, 8, 16, 32, 64, 128};

    

    for (int size : matrixSizes) {
        for (int len : integerLengths) {
            // Generate random matrices
            vector<vector<string>> matrix1(size, vector<string>(size));
            vector<vector<string>> matrix2(size, vector<string>(size));
            generate2DVector(len, size, matrix1);
            generate2DVector(len, size, matrix2);

            // Convert to custom BigInt
            vector<vector<BigInt>> leftCustom(size, vector<BigInt>(size));
            vector<vector<BigInt>> rightCustom(size, vector<BigInt>(size));
            vector<vector<BigInt>> resultCustom(size, vector<BigInt>(size, BigInt(0)));

            for (size_t i = 0; i < size; ++i) {
                for (size_t j = 0; j < size; ++j) {
                    leftCustom[i][j] = BigInt(matrix1[i][j]);
                    rightCustom[i][j] = BigInt(matrix2[i][j]);
                }
            }

            // Convert to GMP (mpz_class)
            vector<vector<mpz_class>> leftGMP(size, vector<mpz_class>(size));
            vector<vector<mpz_class>> rightGMP(size, vector<mpz_class>(size));
            vector<vector<mpz_class>> resultGMP(size, vector<mpz_class>(size, 0));

            for (size_t i = 0; i < size; ++i) {
                for (size_t j = 0; j < size; ++j) {
                    leftGMP[i][j].set_str(matrix1[i][j], 10);
                    rightGMP[i][j].set_str(matrix2[i][j], 10);
                }
            }

            // measure time for custom BigInt
            auto start = chrono::high_resolution_clock::now();
            matmulRowColParallel(leftCustom, rightCustom, resultCustom);
            auto end = chrono::high_resolution_clock::now();
            double time_taken_custome = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            // measure time for GMP
            start = chrono::high_resolution_clock::now();
            matmulRowColParallelGMP(leftGMP, rightGMP, resultGMP);
            end = chrono::high_resolution_clock::now();
            double time_taken_gmp = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            // save data to file
            // open file and clear previous data
            
            FILE* file = fopen("timings_output.txt", "a");

            if (file != nullptr) {
                // Write the timings to the file
                fprintf(file, "size: %d, len: %d, time_custom: %f, time_gmp: %f\n", size, len, time_taken_custome, time_taken_gmp);
                  // Don't forget to close the file
                fclose(file);
            } else {
                // Handle error opening the file
                printf("Error opening file for writing.\n");
            }

        }
    }
    
    return 0;
}