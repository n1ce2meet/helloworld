#include <string>
#include <fstream>
#include <typeinfo>
#include <iostream>

#define WORKER_TAKE_PARAMETERS const std::string &name, const std::string &position, int age
#define WORKER_GIVE_PARAMETERS name, position, age

enum class TYPES
{
    Tpieceworker,
    Temployee
};

class Worker
{
    std::string name_;
    std::string position_;
    int age_;

protected:
    Worker();
    Worker(WORKER_TAKE_PARAMETERS);
    virtual ~Worker() = default;

public:
    void setName(const std::string &name);
    const std::string &getName() const;
    void setPosition(const std::string &position);
    const std::string &getPosition() const;
    void setAge(int age);
    int getAge() const;
    virtual int calculateSalary() const = 0;
    virtual void read(std::ifstream &file);
    virtual void write(std::ofstream &file);
    virtual void print(std::ostream &os) const;
};

class Pieceworker : public Worker
{
    int paymentPerHour_;
    int workTime_;

public:
    Pieceworker();
    Pieceworker(WORKER_TAKE_PARAMETERS, int paymentPerHour, int hoursWorked);
    void setPayment(int payment);
    int getPayment() const;
    void setWorkTime(int hours);
    int getWorkTime() const;
    int calculateSalary() const override;
    void read(std::ifstream &file) override;
    void write(std::ofstream &file) override;
    void print(std::ostream &os) const override;
};

class Employee : public Worker
{
    int salary_;

public:
    Employee();
    Employee(WORKER_TAKE_PARAMETERS, int salary);
    void setSalary(int payment);
    int getSalary() const;
    int calculateSalary() const override;
    void read(std::ifstream &file) override;
    void write(std::ofstream &file) override;
    void print(std::ostream &os) const override;
};

class Storage
{
    std::string fileName_;

public:
    Storage(const std::string &fileName);
    void store(Worker **toPreserve, size_t size);
    Worker **read(size_t size) const;
    Worker **readAll() const;
    static TYPES getType(const Worker *obj);
};

Worker::Worker() {}

Worker::Worker(WORKER_TAKE_PARAMETERS) : name_(name), position_(position), age_(age) {}

void Worker::setName(const std::string &name) { this->name_ = name; }

const std::string &Worker::getName() const { return name_; }

void Worker::setPosition(const std::string &position) { this->position_ = position; }

const std::string &Worker::getPosition() const { return position_; }

void Worker::setAge(int age) { this->age_ = age; }

int Worker::getAge() const { return age_; }

void Worker::read(std::ifstream &file)
{
    size_t nameSize;
    size_t positionSize;
    char tmp[256]{};
    file.read(reinterpret_cast<char *>(&nameSize), sizeof(size_t));
    file.read(tmp, nameSize);
    name_ = tmp;
    file.read(reinterpret_cast<char *>(&positionSize), sizeof(size_t));
    file.read(tmp, positionSize);
    position_ = tmp;
    file.read(reinterpret_cast<char *>(&age_), sizeof(int));
}

void Worker::write(std::ofstream &file)
{
    size_t nameSize = name_.size();
    size_t positionSize = position_.size();
    file.write(reinterpret_cast<char *>(&nameSize), sizeof(size_t));
    file.write(name_.c_str(), nameSize);
    file.write(reinterpret_cast<char *>(&positionSize), sizeof(size_t));
    file.write(position_.c_str(), positionSize);
    file.write(reinterpret_cast<char *>(&age_), sizeof(int));
}

void Worker::print(std::ostream &os) const
{
    os << name_ << ' ' << position_ << ' ' << age_ << std::endl;
}

Pieceworker::Pieceworker() : Worker() {}

Pieceworker::Pieceworker(WORKER_TAKE_PARAMETERS, int paymentPerHour, int workTime)
    : Worker(WORKER_GIVE_PARAMETERS), paymentPerHour_(paymentPerHour), workTime_(workTime) {}

void Pieceworker::setPayment(int payment) { this->paymentPerHour_ = payment; }

int Pieceworker::getPayment() const { return paymentPerHour_; }

void Pieceworker::setWorkTime(int hours) { this->workTime_ = hours; }

int Pieceworker::getWorkTime() const { return workTime_; }

int Pieceworker::calculateSalary() const { return workTime_ * paymentPerHour_; }

void Pieceworker::read(std::ifstream &file)
{
    Worker::read(file);
    file.read(reinterpret_cast<char *>(&paymentPerHour_), sizeof(int));
    file.read(reinterpret_cast<char *>(&workTime_), sizeof(int));
}

void Pieceworker::write(std::ofstream &file)
{
    Worker::write(file);
    file.write(reinterpret_cast<char *>(&paymentPerHour_), sizeof(int));
    file.write(reinterpret_cast<char *>(&workTime_), sizeof(int));
}

void Pieceworker::print(std::ostream &os) const
{
    Worker::print(os);
    os << paymentPerHour_ << ' ' << workTime_ << std::endl;
}

Employee::Employee() : Worker() {}

Employee::Employee(WORKER_TAKE_PARAMETERS, int salary) : Worker(WORKER_GIVE_PARAMETERS), salary_(salary) {}

void Employee::setSalary(int salary) { this->salary_ = salary; }

int Employee::getSalary() const { return salary_; }

int Employee::calculateSalary() const { return salary_; }

void Employee::read(std::ifstream &file)
{
    Worker::read(file);
    file.read(reinterpret_cast<char *>(&salary_), sizeof(int));
}

void Employee::write(std::ofstream &file)
{
    Worker::write(file);
    file.write(reinterpret_cast<char *>(&salary_), sizeof(int));
}

void Employee::print(std::ostream &os) const
{
    Worker::print(os);
    os << salary_ << std::endl;
}

Storage::Storage(const std::string &fileName) : fileName_(fileName) {}

void Storage::store(Worker **toPreserve, size_t size)
{
    std::ofstream file;
    file.open(this->fileName_, std::ios::binary);
    if (!file)
        throw "error file open";
    TYPES type;
    file.write(reinterpret_cast<char *>(&size), sizeof(size_t)); // пишем размер массива
    for (size_t i = 0; i < size; ++i)
    {
        type = Storage::getType(toPreserve[i]);
        file.write(reinterpret_cast<char *>(&type), sizeof(type)); // пишем тип объекта
        toPreserve[i]->write(file);
    }
    file.close();
}

Worker **Storage::read(size_t size) const
{
    std::ifstream file;
    file.open(this->fileName_, std::ios::binary);
    if (!file)
        throw "error file open";

    TYPES type;
    size_t fullSize;
    file.read(reinterpret_cast<char *>(&fullSize), sizeof(size_t));

    if (size > fullSize)
        throw "limit is exceeded";

    Worker **result = new Worker *[size];

    for (size_t i = 0; i < fullSize; ++i)
    {
        file.read(reinterpret_cast<char *>(&type), sizeof(type));

        if (type == TYPES::Tpieceworker)
        {
            result[i] = new Pieceworker;
        }
        else if (type == TYPES::Temployee)
        {
            result[i] = new Employee;
        }

        result[i]->read(file);
    }

    file.close();
    return result;
}

Worker **Storage::readAll() const
{
    std::ifstream file;
    file.open(this->fileName_, std::ios::binary);
    if (!file)
        throw "error file open";
    size_t fullSize;
    file.read(reinterpret_cast<char *>(&fullSize), sizeof(size_t));
    file.close();
    return Storage::read(fullSize);
}

TYPES Storage::getType(const Worker *obj)
{
    if (typeid(*obj) == typeid(Pieceworker))
    {
        return TYPES::Tpieceworker;
    }
    return TYPES::Temployee;
}

std::ostream &operator<<(std::ostream &os, const Worker &obj)
{
    obj.print(os);
    return os;
}