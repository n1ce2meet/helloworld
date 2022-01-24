#include "employees.hpp"

int calculateDepartmentSalary(Worker **department)
{
    int result = 0;
    for (size_t i = 0; i < sizeof(department); ++i)
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

    for (size_t i = 0; i < SIZE; ++i) {
        std::cout << *dep1[i] << "\n";
    }
    std::cout << std::endl;

    Storage s("test.txt");

    s.store(dep1, SIZE);
    
    Worker ** dep1copy = s.readAll();

    for (size_t i = 0; i < SIZE; ++i) {
        std::cout << *dep1copy[i] << "\n";
    }
    std::cout << std::endl;

    Employee input_test{};
    std::cin >> input_test;
    std::cout << input_test;
}