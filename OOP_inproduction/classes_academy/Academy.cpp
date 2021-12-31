#include <iostream>
#include <string>
#include <list>
using namespace std;

class Human
{
    std::string last_name;
    std::string first_name;
    unsigned int age;

public:
    const std::string &get_last_name() const
    {
        return last_name;
    }
    const std::string &get_first_name() const
    {
        return first_name;
    }
    unsigned int get_age() const
    {
        return age;
    }
    void set_last_name(const std::string &last_name)
    {
        this->last_name = last_name;
    }
    void set_first_name(const std::string &first_name)
    {
        this->first_name = first_name;
    }
    void set_age(unsigned int age)
    {
        this->age = age;
    }

    //					Constructors:
    Human(const std::string &last_name, const std::string &first_name, unsigned int age)
    {
        set_last_name(last_name);
        set_first_name(first_name);
        set_age(age);
        std::cout << "HConstructor:\t" << this << std::endl;
    }
    ~Human()
    {
        std::cout << "HDestructor:\t" << this << std::endl;
    }

    //				Methods:
    void print() const
    {
        std::cout << last_name << " " << first_name << " " << age << std::endl;
    }
};

class Student : public Human
{
    std::string speciality;
    std::string group;
    double rating;
    double attendance;

public:
    const std::string &get_speciality() const
    {
        return speciality;
    }
    const std::string &get_group() const
    {
        return group;
    }
    double get_rating() const
    {
        return rating;
    }
    double get_attendance() const
    {
        return attendance;
    }
    void set_speciality(const std::string &speciality)
    {
        this->speciality = speciality;
    }
    void set_group(const std::string &group)
    {
        this->group = group;
    }
    void set_rating(double rating)
    {
        this->rating = rating;
    }
    void set_attendance(double attendance)
    {
        this->attendance = attendance;
    }

    //					Constructors:
    Student(
        const std::string &last_name, const std::string &first_name, unsigned int age,
        const std::string &speciality, const std::string &group, double rating, double attendance) : Human(last_name, first_name, age)
    {
        set_speciality(speciality);
        set_group(group);
        set_rating(rating);
        set_attendance(attendance);
        std::cout << "SConstructor:\t" << this << std::endl;
    }
    ~Student()
    {
        std::cout << "SDestructor:\t" << this << std::endl;
    }

    void print() const
    {
        Human::print();
        std::cout << speciality << " " << group << " " << rating << " " << attendance << std::endl;
    }
};

class Teacher : public Human
{
    std::string speciality_;
    std::string academicRank_;
    std::list<std::string> groups_;

public:
    const std::string &get_speciality() const
    {
        return speciality_;
    }
    const std::string &academicRank() const
    {
        return academicRank_;
    }
    const std::list<std::string> &get_groups() const
    {
        return groups_;
    }
    void set_speciality(const std::string &speciality)
    {
        this->speciality_ = speciality;
    }
    void set_academicRank(const std::string &academicRank)
    {
        this->academicRank_ = academicRank;
    }
    void add_group(const std::string &group)
    {
        this->groups_.push_back(group);
    }

    Teacher(
        const std::string &last_name, const std::string &first_name, unsigned int age,
        const std::string &speciality, const std::string &academicRank, const std::string &group) : Human(last_name, first_name, age)
    {
        set_speciality(speciality);
        set_academicRank(academicRank);
        add_group(group);
        std::cout << "SConstructor:\t" << this << std::endl;
    }

    ~Teacher()
    {
        std::cout << "SDestructor:\t" << this << std::endl;
    }

    void print() const
    {
        Human::print();
        std::cout << speciality_ << " " << academicRank_ << " groups: ";
        for(auto it = groups_.begin(); it != groups_.end(); ++it) {
            std::cout << *it << " , ";
        }
        std::cout << std::endl;
    }
};

class Graduate : public Student {
    std::string graduationThesis_;

public:
    const std::string &get_thesis() const
    {
        return graduationThesis_;
    }

    void set_thesis(const std::string &thesis)
    {
        this->graduationThesis_ = thesis;
    }

    Graduate(
        const std::string &last_name, const std::string &first_name, unsigned int age,
        const std::string &speciality, const std::string &group, double rating, double attendance,
        const std::string &thesis) : Student(last_name, first_name, age, speciality, group, rating, attendance)
    {
        set_thesis(thesis);
        std::cout << "GConstructor:\t" << this << std::endl;
    }

    ~Graduate()
    {
        std::cout << "GDestructor:\t" << this << std::endl;
    }

    void print() const
    {
        Human::print();
        Student::print();
        std::cout << graduationThesis_ << std::endl;
    }
};

int main()
{
    setlocale(LC_ALL, "");
    Human human("Connor", "John", 18);
    human.print();

    Student stud("Pinkman", "Jessie", 25, "Chemistry", "WW_123", 85, 95);
    stud.print();

    Graduate grd("Pinkman", "Jessie", 25, "Chemistry", "WW_123", 85, 95, "abcd");
    grd.print();
    
}