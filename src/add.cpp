#include "../include/BigInt.hpp"

// operator+ overload for BigInt + BigInt
BigInt BigInt::operator+(const BigInt &other) const
{
    BigInt result;
    result.chunks.clear();
    if(isNegative == other.isNegative) 
    {
        add(other,result);
        result.isNegative = isNegative;
    }
    else
    {
        // if both numbers are equal in magnitude but different in sign, return 0
        if(*this == other)
        {
            //return BigInt(0);
            result.chunks.push_back(0);
        }

        // If the signs are different, subtract the numbers
        if(*this > other)
        {
            subtract(other, result);
            result.isNegative = isNegative;
        }
        else
        {
            other.subtract(*this, result);
            result.isNegative = other.isNegative;
        }
    }
    result.removeLeadingZeros();
    return result;
}

// operator+ overload for BigInt + uint64_t
BigInt BigInt::operator+(const uint64_t &other) const
{
    BigInt otherBigInt(other);
    otherBigInt = *this + otherBigInt;
    otherBigInt.removeLeadingZeros();
    return otherBigInt;
}

// add two BigInts
void BigInt::add(const BigInt &other, BigInt &result) const
{
    // BigInt result;
    result.chunks.clear();
    // result.chunks.reserve(std::max(chunks.size(), other.chunks.size()) + 1);
    uint32_t carry = 0;
    //uint32_t sum = 0;
    uint64_t sum = 0;
    uint32_t max_size = std::max(chunks.size(), other.chunks.size());
    for(int i = 0; i < max_size; i++) 
    {
        uint32_t a = i < chunks.size() ? chunks[i] : 0;
        uint32_t b = i < other.chunks.size() ? other.chunks[i] : 0;
        sum = a + b + carry;
        carry = sum / BASE;
        sum %= BASE;
        result.chunks.push_back(sum);

        // Debug prints
        // std::cout << "a: " << a << ", b: " << b << ", sum: " << sum << ", carry: " << carry << std::endl;

    }
    if(carry) 
    {
        result.chunks.push_back(carry);
    }

    result.removeLeadingZeros();
    //return result;
}