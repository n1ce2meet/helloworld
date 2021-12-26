#include <iostream>
typedef unsigned long size_t;
static int CAPASITY = 64;

class Binary
{
    bool *bits_;
    bool signBit_; //MSB

public:
    Binary() : bits_(new bool[CAPASITY]{}), signBit_(0) {}

    Binary(long long number) : Binary()
    {
        for (size_t i = 0; number; ++i)
        {
            bits_[i] = number % 2;
            number /= 2;
        }
    }

    Binary(const Binary &other) : Binary()
    {
        signBit_ = other.signBit_;

        for (size_t i = 0; i < CAPASITY; ++i)
        {
            bits_[i] = other.bits_[i];
        }
    }

    Binary(Binary &&other)
    {
        bits_ = other.bits_;
        signBit_ = other.signBit_;
        other.signBit_ = 0;
        other.bits_ = nullptr;
    }

    ~Binary()
    {
        delete[] bits_;
    }

    Binary &operator=(const Binary &other)
    {
        if (this == &other)
            return *this;
        signBit_ = other.signBit_;
        for (size_t i = 0; i < CAPASITY; ++i)
        {
            bits_[i] = other.bits_[i];
        }
        return *this;
    }

    Binary &operator=(Binary &&other)
    {
        if (this == &other)
            return *this;
        delete [] bits_;
        signBit_ = other.signBit_;
        bits_ = other.bits_;
        other.signBit_ = 0;
        other.bits_ = nullptr;
        return *this;
    }

    bool operator[](int index) const
    {
        return bits_[index];
    }

    bool &operator[](int index)
    {
        return bits_[index];
    }

    operator bool() const
    {
        for (size_t i = 0; i < CAPASITY; ++i)
        {
            if (bits_[i])
                return true;
        }
        return false;
    }

    operator int() const
    {
        int result = 0;
        for (size_t i = CAPASITY - 1; i; --i)
        {
            result = result * 2 + bits_[i];
        }
        return signBit_ ? -result : result;
    }

    bool signBit() const
    {
        return signBit_;
    }

    bool &signBit()
    {
        return signBit_;
    }
};

Binary operator~(const Binary &source)
{
    Binary result{};
    for (size_t i = 0; i < CAPASITY; ++i)
    {
        result[i] = !source[i];
    }
}

Binary operator&(const Binary &left, const Binary &right)
{
    Binary result{};
    for (size_t i = 0; i < CAPASITY; ++i)
    {
        if (left[i] and right[i])
            result[i] = 1;
        else
            result[i] = 0;
    }
}

Binary operator|(const Binary &left, const Binary &right)
{
    Binary result{};
    for (size_t i = 0; i < CAPASITY; ++i)
    {
        if (left[i] or right[i])
            result[i] = 1;
        else
            result[i] = 0;
    }
}

Binary operator^(const Binary &left, const Binary &right)
{
    Binary result{};
    for (size_t i = 0; i < CAPASITY; ++i)
    {
        if (left[i] == right[i])
            result[i] = 0;
        else
            result[i] = 1;
    }
}

Binary operator<<(const Binary &bin, int shift)
{
    Binary result{};
    for (size_t i = shift; i < CAPASITY; ++i)
    {
        result[i] = bin[i - shift];
    }
}

Binary operator>>(const Binary &bin, int shift)
{
    Binary result{};
    for (size_t i = CAPASITY - shift; i; --i)
    {
        result[i] = bin[i + shift];
    }
}

bool operator>(const Binary &left, const Binary &right)
{
    for (size_t i = CAPASITY; i; --i)
    {
        if (left[i] == right[i])
            continue;
        return left[i] > right[i] ? true : false;
    }
}

bool operator<(const Binary &left, const Binary &right)
{
    return !(left > right);
}

bool operator==(const Binary &left, const Binary &right)
{
    if (left.signBit() != right.signBit())
        return false;
    for (size_t i = CAPASITY; i; --i)
    {
        if (left[i] != right[i])
            return false;
    }
    return true;
}

bool operator!=(const Binary &left, const Binary &right)
{
    return !(left == right);
}

bool operator>=(const Binary &left, const Binary &right)
{
    return (left > right) or (left == right);
}

bool operator<=(const Binary &left, const Binary &right)
{
    return (left < right) or (left == right);
}

Binary operator-(const Binary bin) {
    Binary result(bin);
    result.signBit() = !result.signBit();
    return result;
}

Binary operator+(const Binary &left, const Binary &right)
{
    if (left.signBit() != right.signBit())
        return left > right ? left - right : right - left;
    Binary result{};
    bool carryBit = 0;
    for (size_t i = 0; i < CAPASITY; ++i)
    {
        result[i] = left[i] ^ right[i] ^ carryBit;
        carryBit = (left[i] & (right[i] | carryBit)) | (right[i] & carryBit);
    }
    result.signBit() = left.signBit();
    return result;
}

Binary operator-(const Binary &left, const Binary &right)
{
    if (right > left) return right - left;
    Binary result = ~(~left + right);
    result.signBit() = left.signBit();
    return result;
}

Binary operator*(const Binary &left, const Binary &right)
{
    Binary result{};
    for (size_t i = 0; i < static_cast<int>(right); ++i) {
        result = result + result;
    }
    return result;
}

// Binary operator/(const Binary &left, const Binary &right)
// {
//     if (!right) throw "Zero division error";
//     Binary result{};
//     return result;
// }
