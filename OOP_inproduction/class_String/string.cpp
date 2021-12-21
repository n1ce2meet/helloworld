#include "string.hpp"

String::String(int size) : size_(size), cstr_(new char[size]) {}

String::String(const char *cstr) : size_(std::strlen(cstr) + 1)
{
    cstr_ = new char[size_];
    for (size_t i = 0; i < size_; ++i)
    {
        cstr_[i] = cstr[i];
    }
}

String::String(const String &other) : String(other.cstr_) {}

String::~String()
{
    delete[] cstr_;
}

String &String::operator=(const String &other)
{
    if (this == &other)
        return *this;
    delete[] cstr_;
    size_ = other.size_;
    cstr_ = new char[size_];
    for (size_t i = 0; i < size_; ++i)
    {
        cstr_[i] = other.cstr_[i];
    }
    return *this;
}

String &String::operator+=(const String &other)
{
    return *this = *this + other;
}

const char &String::operator[](int i) const { return cstr_[i]; }
char &String::operator[](int i) { return cstr_[i]; }

int String::get_size() const { return size_; }

const char *String::get_cstr() const { return cstr_; }
char *String::get_cstr() { return cstr_; }

String operator+(const String &first, const String &second)
{
    String result(first.get_size() + second.get_size());
    for (size_t i = 0; i < first.get_size(); ++i)
    {
        result.get_cstr()[i] = first.get_cstr()[i];
    }
    for (size_t i = first.get_size() - 1, j = 0; i < first.get_size() + second.get_size() - 1; ++i, ++j)
    {
        result.get_cstr()[i] = second.get_cstr()[j];
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const String &str)
{
    os << str.get_cstr();
    return os;
}

std::istream &operator>>(std::istream &is, String &str)
{
    char buf[1024]{};
    is >> buf;
    str = buf;
    return is;
}

std::istream &getline(std::istream &is, String &str)
{
    char buf[1024]{};
    is.getline(buf, 1024);
    str = buf;
    return is;
}
