#include "../include/BigInt.hpp"

// operator- overload for BigInt - BigInt
BigInt BigInt::operator-(const BigInt &other) const
{
    BigInt result;
    result.chunks.clear();
    if(isNegative == other.isNegative) 
    {
        if(*this == other)
        {
            result.chunks.push_back(0);
        }

        if(*this > other)
        {
            subtract(other,result);
            result.isNegative = isNegative;
        }
        else
        {
            other.subtract(*this,result);
            result.isNegative = !isNegative;
        }
    }
    else
    {
        // if numbers have different signs, add them
        add(other, result);
        result.isNegative = isNegative;
    }
    result.removeLeadingZeros();
    return result;
}

// operator- overload for BigInt - uint64_t
BigInt BigInt::operator-(const uint64_t &other) const
{
    BigInt otherBigInt(other);
    otherBigInt = *this - otherBigInt;
    // otherBigInt.removeLeadingZeros();
    return otherBigInt;
}

// subtract two BigInts
void BigInt::subtract(const BigInt &other, BigInt &result) const
{
    //BigInt result;
    result.chunks.clear();
    int64_t borrow = 0;
    for (size_t i = 0; i < chunks.size(); ++i) {
        int64_t a = chunks[i];
        int64_t b = (i < other.chunks.size()) ? other.chunks[i] : 0;
        int64_t diff = a - b - borrow;
        if (diff < 0) {
            diff += BASE;        
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.chunks.push_back(diff);
    }
    result.removeLeadingZeros();
    // return result;
}