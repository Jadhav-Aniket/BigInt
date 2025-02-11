#include "../include/BigInt.hpp"
#include <iomanip> // Include this header for setfill and setw


BigInt::BigInt()
{
    isNegative = false;
    //chunks.push_back(0);
    chunks.clear();
}

// Constructor to convert a string to a BigInt
// Store data in chunks of base_len digits in reverse order (Lsd to Msd)
BigInt::BigInt(const std::string &number) 
{
    chunks.clear();
    isNegative = number[0] == '-' ? true : false;
    int start = isNegative ? 1 : 0;
    for(int i = number.size() - 1; i >= start; i -= BASE_LEN) 
    {
        uint32_t chunk = 0;
        int j = std::max(start, i - (BASE_LEN - 1));
        chunk = std::stoul(number.substr(j, i - j + 1));
        chunks.push_back(chunk);
    }
    removeLeadingZeros();
}

// Constructor to convert an int64_t to a BigInt
BigInt::BigInt(const int32_t &number) 
{
    chunks.clear();
    isNegative = number < 0 ? true : false;
    int32_t num = std::abs(number);
    if (num == 0) 
    {
        chunks.push_back(0);
    }
    while(num > 0) 
    {
        chunks.push_back(num % BASE);
        num /= BASE;
    }
    removeLeadingZeros();
}

// multiply by base n number of times.
BigInt BigInt::multiplyByBase(int n) 
{
    BigInt result(*this);
    result.chunks.insert(result.chunks.begin(), n, 0);
    result.removeLeadingZeros();
    return result;
}

// // Bigint to string
// std::string BigInt::BigInt_to_string() const
// {
//     std::string result;
//     // if(isNegative) 
//     // {
//     //     result += '-';
//     // }
//     // result += std::to_string(chunks.back());
//     // for(int i = chunks.size() - 2; i >= 0; i--) 
//     // {
//     //     result += std::setfill('0') + std::to_string(chunks[i]);
//     // }
//     return result;
// }

// Print the BigInt
void BigInt::print() const 
{
    if(isNegative) 
    {
        std::cout << '-';
    }
    std::cout << chunks.back();
    std::cout<<"_";
    for(int i = chunks.size() - 2; i >= 0; i--) 
    {
        std::cout << std::setfill('0') << std::setw(BASE_LEN) << chunks[i];
        //std::cout<<chunks[i];
        if(i!=0) std::cout<<"_";
    }
    std::cout << std::endl;
}

// print the BigInt in reverse order
void BigInt::printReverse() const 
{
    if(isNegative) 
    {
        std::cout << '-';
    }
    for(int i = 0; i < chunks.size(); i++) 
    {
        std::cout << chunks[i];
        if(i != chunks.size() - 1) 
        {
            std::cout << '_';
        }
    }
    std::cout << std::endl;
}

// Remove leading zeros
void BigInt::removeLeadingZeros() 
{
    while(chunks.size() > 1 && chunks.back() == 0) 
    {
        chunks.pop_back();
    }

    // If the number is zero, make sure it is not negative
    isNegative = chunks.size() == 1 && chunks[0] == 0 ? false : isNegative;
}