#include "string.cpp"

#define CONSTRUCTORS_CHECK
//#define OPERATORS_CHECK
//#define INPUT_CHECK

int main()
{
#ifdef CONSTRUCTORS_CHECK
    String str1; // Default constructor
    std::cout << str1 << std::endl;

    String str2(25); // Single-argument constructor
    std::cout << str2 << std::endl;

    String str3 = "Hello";
    str3 = str3;
    std::cout << str3 << std::endl;

    String str4 = str3; // Copy constructor
    // Deep copy
    // Shallow copy
    //Debug Assertation Failed
    std::cout << str4 << std::endl;

    String str5;
    str5 = str3; // Needed copyAssignment (operator =)
    std::cout << str5 << std::endl;
#endif // CONSTRUCTORS_CHECK

#ifdef OPERATORS_CHECK
    String str1 = "Hello";
    String str2 = "World";
    String str3 = str1 + str2;
    std::cout << str3 << std::endl;
    str1 += str2;
    std::cout << str1 << std::endl;
#endif // OPERATORS_CHECK

#ifdef INPUT_CHECK
    String str;
    std::cout << "Enter the line: " << std::endl;
    //cin >> str;
    getline(std::cin, str);
    std::cout << str << std::endl;
#endif // INPUT_CHECK
}