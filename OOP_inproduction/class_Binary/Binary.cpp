#include <iostream>
typedef unsigned long size_t;
static size_t CAPASITY = 32;

class Binary
{
    bool *bits_;
    bool signBit_; //MSB

public:
    Binary() : bits_(new bool[CAPASITY]{}), signBit_(0) {}

    Binary(long long number) : Binary()
    {
        if (number < 0)
            signBit_ = 1;
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
        delete[] bits_;
        signBit_ = other.signBit_;
        bits_ = other.bits_;
        other.signBit_ = 0;
        other.bits_ = nullptr;
        return *this;
    }

    const bool &operator[](int index) const
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
        for (int i = CAPASITY - 1; i >= 0; --i)
        {
            result = result * 2 + bits_[i];
        }
        return signBit_ ? -result : result;
    }

    const bool &signBit() const
    {
        return signBit_;
    }

    bool &signBit()
    {
        return signBit_;
    }
};

Binary add(const Binary &left, const Binary &right) // сумматор
{
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

Binary operator~(const Binary &source)
{
    Binary result{};
    for (size_t i = 0; i < CAPASITY; ++i)
    {
        result[i] = !source[i];
    }
    return result;
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
    return result;
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
    return result;
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
    return result;
}

Binary operator<<(const Binary &bin, int shift)
{
    Binary result{};
    for (size_t i = shift; i < CAPASITY; ++i)
    {
        result[i] = bin[i - shift];
    }
    return result;
}

Binary operator>>(const Binary &bin, int shift)
{
    Binary result{};
    for (size_t i = CAPASITY - shift; i; --i)
    {
        result[i] = bin[i + shift];
    }
    return result;
}

bool operator>(const Binary &left, const Binary &right)
{
    for (int i = CAPASITY - 1; i >= 0; --i)
    {
        if (left[i] == right[i])
            continue;
        return left[i] ? true : false;
    }
    return false;
}

bool operator<(const Binary &left, const Binary &right)
{
    return !(left > right);
}

bool operator==(const Binary &left, const Binary &right)
{
    if (left.signBit() != right.signBit())
        return false;
    for (int i = CAPASITY; i >= 0; --i)
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

Binary operator-(const Binary bin)
{
    Binary result(bin);
    result.signBit() = !result.signBit();
    return result;
}

Binary operator-(const Binary &left, const Binary &right)
{
    Binary result;
    if (right > left)
        return (-right) - left;
    else if (right.signBit() != left.signBit())
        return result = (add(left, right));
    else
        result = (~(add(~left, right)));
    result.signBit() = left.signBit();
    return result;
}

Binary operator+(const Binary &left, const Binary &right)
{
    if (left.signBit() != right.signBit())
        return left.signBit() ? right - (-left) : left - (-right);
    Binary result = add(left, right);
    result.signBit() = left.signBit();
    return result;
}

Binary operator*(const Binary &left, const Binary &right)
{
    Binary result{};
    for (size_t i = 0; i < static_cast<int>(right); ++i)
    {
        result = add(result,left);
    }
    return result;
}

Binary operator/(const Binary &left, const Binary &right)
{
    if (!right)
        throw "Zero division error";
    int counter = 0;
    Binary temp(left);
    while (temp >= right)
    {
        temp = temp - right;
        ++counter;
    }
    Binary result(counter);
    return result;
}

std::ostream &operator<<(std::ostream &os, const Binary &bin)
{
    for (int i = CAPASITY - 1; i >= 0; --i)
    {
        os << bin[i];
        if (i % 4 == 0)
            os << ' ';
    }
    return os;
}

//#define CONSTRUCTORS_CHECK
#define OPERATORS_CHECK

int main()
{

#ifdef CONSTRUCTORS_CHECK
    Binary b1(20);
    Binary b2(b1);
    Binary b3(Binary(11));
#endif

#ifdef OPERATORS_CHECK
    Binary b4 = 40;
    Binary b5 = b4;
    Binary b6 = Binary(20);
    Binary b7 = b4 + b6;
    Binary b8 = b7 - b4;
    Binary b9 = b7 * Binary(4);
    Binary b10 = b9 / b6;

    std::cout << b4 << std::endl;
    std::cout << int(b4) << std::endl;
    std::cout << b5 << std::endl;
    std::cout << int(b5) << std::endl;
    std::cout << b6 << std::endl;
    std::cout << int(b6) << std::endl;
    std::cout << b7 << std::endl;
    std::cout << int(b7) << std::endl;
    std::cout << b8 << std::endl;
    std::cout << int(b8) << std::endl;
    std::cout << b9 << std::endl;
    std::cout << int(b9) << std::endl;
    std::cout << b10 << std::endl;
    std::cout << int(b10) << std::endl;
    std::cout << Binary(-3) + Binary(-3) << std::endl;
    std::cout << int(Binary(-3) + Binary(-3)) << std::endl;
    std::cout << Binary(3) + Binary(-3) << std::endl;
    std::cout << int(Binary(3) + Binary(-3)) << std::endl;
    std::cout << Binary(-3) - Binary(3) << std::endl;
    std::cout << int(Binary(-3) - Binary(3)) << std::endl;
#endif
}