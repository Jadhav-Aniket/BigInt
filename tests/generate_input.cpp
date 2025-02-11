
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

#include "generate_input.hpp"

// Function to generate a random string of digits of a fixed size
std::string generateRandomString(size_t size) {
    static const char digits[] = "0123456789";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<size_t> dist(0, sizeof(digits) - 2);

    std::string result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result += digits[dist(rng)];
    }
    return result;
}

// Function to generate a 2D vector of random strings
void generate2DVector(size_t stringSize, size_t matrixSize, std::vector<std::vector<std::string>> &matrix)
{
    //std::vector<std::vector<std::string>> matrix(matrixSize, std::vector<std::string>(matrixSize));
    for (size_t i = 0; i < matrixSize; ++i) {
        for (size_t j = 0; j < matrixSize; ++j) {
            matrix[i][j] = generateRandomString(stringSize);
        }
    }
}