#pragma once
#include <iostream>
#include <cstring>

class String;
String operator+(const String &first, const String &second);
std::ostream &operator<<(std::ostream &os, const String &str);
std::istream &operator>>(std::istream &is, String &str);
std::istream &getline(std::istream &is, String &str);

class String
{
    int size_;
    char *cstr_;

public:
    explicit String(int size = 80);
    String(const char *cstr);
    String(const String &other);
    ~String();

    String &operator=(const String &other);
    String &operator+=(const String &other);
    const char &operator[](int i) const;
    char &operator[](int i);

    int get_size() const;
    const char *get_cstr() const;
    char *get_cstr();
};
