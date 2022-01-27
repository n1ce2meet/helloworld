#include <iostream>
#include <fstream>
#include <string>
#include <list>

#define HUMAN_TAKE_PARAMETERS const std::string &last_name, const std::string &first_name, unsigned int age
#define HUMAN_GIVE_PARAMETERS last_name, first_name, age

std::istream &skip(std::istream &is)
{
    is.ignore();
    return is;
}

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
    Human(HUMAN_TAKE_PARAMETERS)
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
    virtual void print(std::ostream &os = std::cout, const char delimeter = ',') const
    {
        os << last_name << delimeter << first_name << delimeter << age;
    }

    virtual void read_from(std::istream &is, const char delimeter = ',')
    {
        std::getline(is,last_name,delimeter);
        std::getline(is,first_name,delimeter);
        is >> age >> skip;
    }
};

#define STUDENT_TAKE_PARAMETERS const std::string &speciality, const std::string &group, double rating, double attendance
#define STUDENT_GIVE_PARAMETERS speciality, group, rating, attendance

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
        HUMAN_TAKE_PARAMETERS,
        STUDENT_TAKE_PARAMETERS) : Human(HUMAN_GIVE_PARAMETERS)
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


    virtual void print(std::ostream &os = std::cout, const char delimeter = ',') const override
    {
        Human::print(os, delimeter);
        os << delimeter << speciality << delimeter << group << delimeter << rating << delimeter << attendance;
    }

    virtual void read_from(std::istream &is, const char delimeter = ',') override
    {
        Human::read_from(is,delimeter);
        std::getline(is,speciality,delimeter);
        std::getline(is,group,delimeter);
        is >> rating >> skip >> attendance >> skip;
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
        HUMAN_TAKE_PARAMETERS,
        const std::string &speciality, const std::string &academicRank, const std::string &group) : Human(HUMAN_GIVE_PARAMETERS)
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

    virtual void print(std::ostream &os = std::cout, const char delimeter = ',') const override
    {
        Human::print(os, delimeter);
        os << delimeter << speciality_ << delimeter << academicRank_ << delimeter;
        for (auto i : groups_)
        {
            os << i << ' ';
        }
        os << '\n';
    }

    virtual void read_from(std::istream &is, const char delimeter = ',') override
    {
        Human::read_from(is,delimeter);
        std::getline(is,speciality_,delimeter);
        std::getline(is,academicRank_,delimeter);
        while (is.peek() != '\n')
        {
            std::string temp;
            is >> temp;
            groups_.emplace_back(temp);
        }
    }
};

class Graduate : public Student
{
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
        HUMAN_TAKE_PARAMETERS,
        STUDENT_TAKE_PARAMETERS,
        const std::string &thesis) : Student(HUMAN_GIVE_PARAMETERS, STUDENT_GIVE_PARAMETERS)
    {
        set_thesis(thesis);
        std::cout << "GConstructor:\t" << this << std::endl;
    }

    ~Graduate()
    {
        std::cout << "GDestructor: \t" << this << std::endl;
    }

    virtual void print(std::ostream &os = std::cout, const char delimeter = ',') const override
    {
        Student::print(os, delimeter);
        os << delimeter << graduationThesis_;
    }

    virtual void read_from(std::istream &is, const char delimeter = ',') override
    {
        Student::read_from(is,delimeter);
        is >> graduationThesis_;
    }
};

#define HUMAN_EMPTY "", "", 0
#define STUDENT_EMPTY HUMAN_EMPTY, "", "", 0, 0
#define GRADUATE_EMPTY STUDENT_EMPTY, ""
#define TEACHER_EMPTY HUMAN_EMPTY, "", "", ""

namespace HumansHash {
    size_t typeHuman = typeid(Human).hash_code();
    size_t typeStudent = typeid(Student).hash_code();
    size_t typeGraduate = typeid(Graduate).hash_code();
    size_t typeTeacher = typeid(Teacher).hash_code();
}


Human * HumanFactory(size_t hash)
{
    if (hash == HumansHash::typeHuman)
    {
        return new Human(HUMAN_EMPTY);
    }
    if (hash == HumansHash::typeStudent)
    {
        return new Student(STUDENT_EMPTY);
    }
    if (hash == HumansHash::typeGraduate)
    {
        return new Graduate(GRADUATE_EMPTY);
    }
    if (hash == HumansHash::typeTeacher)
    {
        return new Teacher(TEACHER_EMPTY);
    }
    else
    {
        throw "Class error";
    }
}

template <class Array>
void write(std::string path, const Array &arr)
{
    std::ofstream file;
    file.open(path, std::ios_base::app);
    if (!file)
        throw "error file open";
    for (auto ph : arr)
    {
        file << typeid(*ph).hash_code() << ';';
        ph->print(file, ';');
        file << '\n';
    }
    file.close();
}

std::list<Human *> read(std::string path)
{
    std::ifstream file;
    std::list<Human *> result{};
    file.open(path);
    if (!file)
        throw "error file open";

    size_t id;
    while (!((file >> id >> skip).fail()) and file.peek() != EOF)
    {
        Human *temp = HumanFactory(id);
        temp->read_from(file, ';');
        result.emplace_back(temp);
    }
    file.close();
    return result;
}


int main()
{
    std::ofstream i;
    std::ostream &b = i;
    setlocale(LC_ALL, "");
    Human human("Connor", "John", 18);
    human.print();

    Student stud("Pinkman", "Jessie", 25, "Chemistry", "WW_123", 85, 95);
    stud.print();

    Graduate grd("Pinkman", "Jessie", 25, "Chemistry", "WW_123", 85, 95, "abcd");
    grd.print();

    std::list<Human *> hl = {&human, &stud, &grd};
    write("test.txt", hl);
    auto testl = read("test.txt");
    for (auto ph : testl) {
        ph->print();
        std::cout << '\n';
    }
}