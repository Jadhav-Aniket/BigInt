#include "../include/BigInt.hpp"
#include<algorithm>
#include<iostream>
// comaprison operators

// if equal function
bool BigInt::operator==(const BigInt &other) const
{
    if(chunks.size() != other.chunks.size())
    {
        // std::cout<<std::endl;
        // std::cout<<chunks.size()<<" "<<other.chunks.size()<<std::endl;
        // std::cout<<"Chunks size not equal";
        return false;
    }

    for(int i = 0; i < chunks.size(); i++)
    {
        if(chunks[i] != other.chunks[i])
        {
            return false;
        }
    }
    return true;
}

// if greater than function 
// we just need to check maginitude of the number
bool BigInt::operator>(const BigInt &other) const
{
    if(chunks.size() > other.chunks.size())
    {
        return true;
    }
    else if(chunks.size() < other.chunks.size())
    {
        return false;
    }

    for(int i = chunks.size() - 1; i >= 0; i--)
    {
        if(chunks[i] > other.chunks[i])
        {
            return true;
        }
        else if(chunks[i] < other.chunks[i])
        {
            return false;
        }
    }
    return false;
}

// divide by 2 function
BigInt BigInt::divideBy2() const
{
    BigInt result;
    result.chunks.clear();
    uint32_t carry = 0;
    for(int i = chunks.size() - 1; i >= 0; i--)
    {
        uint32_t chunk = chunks[i] + carry * BASE;
        result.chunks.push_back(chunk / 2);
        carry = chunk % 2;
    }
    std::reverse(result.chunks.begin(), result.chunks.end());
    result.removeLeadingZeros();
    return result;
}

// divide by 3 function
BigInt BigInt::divideBy3() const
{
    BigInt result;
    result.chunks.clear();
    uint32_t carry = 0;
    for(int i = chunks.size() - 1; i >= 0; i--)
    {
        uint32_t chunk = chunks[i] + carry * BASE;
        result.chunks.push_back(chunk / 3);
        carry = chunk % 3;
    }
    std::reverse(result.chunks.begin(), result.chunks.end());
    result.removeLeadingZeros();
    return result;
}

// check if all private functions work
void BigInt::checkprivatefunctions() const
{
    BigInt result;
    result.chunks.clear();
    result = divideBy2();
    std::cout<<"Divide by 2: ";
    result.print();

    std::cout<<std::endl;
    std::cout<<"Divide by 3: ";
    result = divideBy3();
    result.print();
}