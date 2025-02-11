#include "../include/BigInt.hpp"

// operator* overload for BigInt * BigInt

BigInt BigInt::operator*(const BigInt &other) const
{
    BigInt result;

    // if any of the numbers is zero, return 0
    if(*this == BigInt(0) || other == BigInt(0))
    {
        return BigInt(0);
    }

    result.chunks.clear();
    result.isNegative = isNegative ^ other.isNegative;

    // if the number of chunks is less than 32, use schoolbook multiplication
    if(chunks.size() < 32 || other.chunks.size() < 32)
    {
        result.chunks.resize(chunks.size() + other.chunks.size(), 0);
        for (size_t i = 0; i < chunks.size(); ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < other.chunks.size(); ++j) {
                size_t pos = i + j;
                uint64_t product = static_cast<uint64_t>(chunks[i]) * other.chunks[j];
                uint64_t sum = product + result.chunks[pos] + carry;
                result.chunks[pos] = sum % BASE;
                carry = sum / BASE;
            }
            // Propagate remaining carry
            size_t pos = i + other.chunks.size();
            while (carry > 0) {
                if (pos >= result.chunks.size()) {
                    result.chunks.push_back(0);
                }
                uint64_t sum = result.chunks[pos] + carry;
                result.chunks[pos] = sum % BASE;
                carry = sum / BASE;
                ++pos;
            }
        }
    }
    else
    {
        // perform toom cook multiplication
        multiply(other,result);
    }
    result.removeLeadingZeros();
    return result;
}

// operator* overload for BigInt * uint64_t
BigInt BigInt::operator*(const uint64_t &other) const
{
    BigInt otherBigInt(other);
    return *this * otherBigInt;
}


// multiply two BigInts
// Toom Cook multiplication
void BigInt::multiply(const BigInt &other, BigInt &result) const
{

    size_t len = std::max(chunks.size(), other.chunks.size());

    // split the numbers into 3 parts
    size_t n = len / 3;

    BigInt a2, a1, a0, b2, b1, b0;

    // Assign A
    a0.chunks.assign(chunks.begin(), chunks.begin() + n);
    a1.chunks.assign(chunks.begin() + n, chunks.begin() + 2 * n);
    a2.chunks.assign(chunks.begin() + 2 * n, chunks.end());

    // Assign B
    b0.chunks.assign(other.chunks.begin(), other.chunks.begin() + n);
    b1.chunks.assign(other.chunks.begin() + n, other.chunks.begin() + 2 * n);
    b2.chunks.assign(other.chunks.begin() + 2 * n, other.chunks.end());

    // // Debug prints
    // std::cout << "a0: "; a0.print();
    // std::cout << "a1: "; a1.print();
    // std::cout << "a2: "; a2.print();
    // std::cout << "b0: "; b0.print();
    // std::cout << "b1: "; b1.print();
    // std::cout << "b2: "; b2.print();

    // Evaluate at x = 0, 1, -1, -2, infinity
    BigInt p0 = a0 * b0;
    BigInt pInf = a2 * b2;
    BigInt p1 = (a0 + a1 + a2) * (b0 + b1 + b2);
    BigInt p_1 = (a0 - a1 + a2) * (b0 - b1 + b2);
    BigInt p2 = (a0 + (a1 * 2) + (a2 * 4)) * (b0 + (b1 * 2) + (b2 * 4));
    
    // //Debug prints
    // std::cout << "p0: "; p0.print();
    // std::cout << "p1: "; p1.print();
    // std::cout << "p_1: "; p_1.print();
    // std::cout << "p2: "; p2.print();
    // std::cout << "pInf: "; pInf.print();

    // Interpolation to solve coefficients
    p2 = (p2-p_1).divideBy3();
    p_1 = (p1-p_1).divideBy2();
    p1 = p1-p0;
    p2 = (p2-p1).divideBy2();
    p1 = p1-p_1;
    p1 = p1 - pInf;
    p2 = p2 - (pInf*2);
    p_1 = p_1 - p2;

    // // Interpolation to solve coefficients
    // //debug prints
    // std::cout << "Interpolation" << std::endl;
    // std::cout << "defore divide p2-p_1: ";
    // std::cout << "p2: "; p2.print();
    // std::cout << "p_1: "; p_1.print();
    // p2 = (p2-p_1).divideBy3();
    // std::cout << "after divide p2-p_1: ";
    // std::cout << "p2: "; p2.print();
    // std::cout << std::endl;


    // std::cout << "defore divide p1-p_1: ";
    // std::cout << "p1: "; p1.print();
    // std::cout << "p_1: "; p_1.print();
    // p_1 = (p1-p_1).divideBy2();
    // std::cout << "after divide p1-p_1: ";
    // std::cout << "p_1: "; p_1.print();
    // std::cout << std::endl;

    // std::cout << "p1: "; p1.print();
    // std::cout << "p0: "; p0.print();
    // p1 = p1-p0;
    // std::cout << "after p1-p0: ";
    // std::cout << "p1: "; p1.print();
    // std::cout << std::endl;

    // std::cout << "before divide p2-p1: ";
    // std::cout << "p2: "; p2.print();
    // std::cout << "p1: "; p1.print();
    // p2 = (p2-p1).divideBy2();
    // std::cout << "after divide p2-p1: ";
    // std::cout << "p2: "; p2.print();
    // std::cout << std::endl;

    // std::cout << "p1: "; p1.print();
    // std::cout << "p_1: "; p_1.print();
    // p1 = p1-p_1;
    // std::cout << "after p1-p_1: ";
    // std::cout << "p1: "; p1.print();
    // std::cout << std::endl;

    // std::cout << "p1: "; p1.print();
    // std::cout << "pInf: "; pInf.print();
    // p1 = p1 - pInf;
    // std::cout<<"after p1-pInf: ";
    // std::cout << "p1: "; p1.print();
    // std::cout << std::endl;

    // std::cout << "p2: "; p2.print();
    // std::cout << "pInf: "; pInf.print();
    // p2 = p2 - (pInf*2);
    // std::cout << "after p2-(pInf*2): ";
    // std::cout << "p2: "; p2.print();
    // std::cout << std::endl;


    // std::cout << "p_1: "; p_1.print();
    // std::cout << "p2: "; p2.print();
    // p_1 = p_1 - p2;
    // std::cout << "after p_1-p2: ";
    // std::cout << "does segmentation fault occur here?" << std::endl;
    // std::cout << "p_1: "; p_1.print();
    // std::cout << std::endl;

    BigInt r0 = p0, r1 = p_1, r2 = p1, r3 = p2, r4 = pInf;
    // // Debug prints
    // std::cout << "r0: "; r0.print();
    // std::cout << "r1: "; r1.print();
    // std::cout << "r2: "; r2.print();
    // std::cout << "r3: "; r3.print();
    // std::cout << "r4: "; r4.print();
    // std::cout << std::endl;


    // // Combine the results
    // BigInt basePower = BigInt(1);
    // for (size_t i = 0; i < n; ++i) {
    //     basePower = basePower * BASE;
    // }
    //std::cout << "Base Power: "; basePower.print();

    // result = r0 +
    //          (r1 * basePower) +
    //          (r2 * basePower * basePower) +
    //          (r3 * basePower * basePower * basePower) +
    //          (r4 * basePower * basePower * basePower * basePower);

    // BigInt result = BigInt(0);
    
    // result.chunks.clear();

    // //debug prints
    // std::cout << "result before r0: "; r0.print();
    // result = r0;
    // std::cout << "result after r0: "; result.print();
    // result = result + r1.multiplyByBase(n);
    // std::cout << "r1: "; r1.print();
    // std::cout << "result after r1: "; result.print();
    // result = result + (r2.multiplyByBase(2*n));
    // std::cout << "r2: "; r2.print();
    // std::cout << "result after r2: "; result.print();
    // result = result + (r3.multiplyByBase(3*n));
    // std::cout << "r3: "; r3.print();
    // std::cout << "result after r3: "; result.print();
    // result = result + (r4.multiplyByBase(4*n));
    // std::cout << "r4: "; r4.print();
    // std::cout << "result after r4: "; result.print();
    
    result.chunks.clear();

    result = r0 +
             (r1.multiplyByBase(n)) +
             (r2.multiplyByBase(2*n)) +
             (r3.multiplyByBase(3*n)) +
             (r4.multiplyByBase(4*n));

    // // debug result
    // std::cout << "r1: "; r1.print();
    // std::cout << "r2: "; r2.print();
    // std::cout << "r3: "; r3.print();
    // std::cout << "r4: "; r4.print();
    
    
    result.removeLeadingZeros();
    // return result;
}