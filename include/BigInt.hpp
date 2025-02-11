#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <vector>
#include <string>
// #include<bits/stdc++.h>

constexpr int BASE_LEN = 9;
constexpr uint32_t BASE = 1e9;


class BigInt {
public:
    // Constructors
    BigInt();

    BigInt(const std::string &number);
    BigInt(const std:: int32_t &number);

    // Bigint to string
    //std::string BigInt_to_string() const;

    // print function
    void print() const;
    void printReverse() const;

    // Arithmetic operators
    BigInt operator+(const BigInt &other) const;
    BigInt operator+(const uint64_t &other) const;
    
    BigInt operator-(const BigInt &other) const;
    BigInt operator-(const uint64_t &other) const;
    
    BigInt operator*(const BigInt &other) const;
    BigInt operator*(const uint64_t &other) const;

    // multiply bigint by base n number of times.
    BigInt multiplyByBase(int n);

    // Comparison operators
    bool operator==(const BigInt &other) const;
    bool operator>(const BigInt &other) const;

    // check if all private function works
    void checkprivatefunctions() const;
    
private:
    std::vector<uint32_t> chunks;
    bool isNegative;

    // Helper functions
    void removeLeadingZeros();
    void add(const BigInt &other, BigInt &result) const;
    void subtract(const BigInt &other, BigInt &result) const;
    void multiply(const BigInt &other, BigInt &result) const;
    BigInt divideBy2() const;
    BigInt divideBy3() const;
};

#endif // BIGINT_HPP