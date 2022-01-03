#include <iostream>
#include <fstream>
#include "employees.hpp"

int calculateDepartmentSalary(const Worker **department)
{
    int result = 0;
    for (size_t i = 0; i < sizeof(department) / sizeof(Worker); ++i)
    {
        result += department[i]->calculateSalary();
    }
    return result;
}

int main()
{

    const int SIZE = 8;
    Worker **dep1 = new Worker *[SIZE]
    {
        new Employee("John", "manager", 31, 3000),
        new Employee("Ann", "manager", 25, 2500),
        new Employee("Sam", "department head", 40, 5000),
        new Pieceworker("Adam", "programmist", 27, 20, 120),
        new Pieceworker("Robin", "programmist", 24, 15, 100),
        new Pieceworker("Alex", "programmist", 39, 20, 60),
        new Pieceworker("Mark", "programmist", 33, 20, 120),
        new Pieceworker("Jessie", "programmist", 28, 25, 40)
    };

    std::cout << "Total by department 1: " << calculateDepartmentSalary(dep1) << std::endl;

    std::fstream file;
    file.open("test.txt", std::ios::binary);
    file.write(reinterpret_cast<char *>(SIZE), sizeof(int));
    for (size_t i = 0; i < SIZE; ++i)
    {
        file.write(reinterpret_cast<char*>(dep1[i]), sizeof(typeid(*dep1[i])));
    }

    // НЕЗАКОНЧЕНО
}