#include <string>

#define WORKER_TAKE_PARAMETERS const std::string &name, const std::string &position, int age
#define WORKER_GIVE_PARAMETERS name, position, age

class Worker
{
    std::string name_;
    std::string position_;
    int age_;

protected:
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
};

class Pieceworker : public Worker
{
    int paymentPerHour_;
    int workTime_;

public:
    Pieceworker(WORKER_TAKE_PARAMETERS, int paymentPerHour, int hoursWorked);
    void setPayment(int payment);
    int getPayment() const;
    void setWorkTime(int hours);
    int getWorkTime() const;
    int calculateSalary() const override;
};

class Employee : public Worker
{
    int salary_;

public:
    Employee(WORKER_TAKE_PARAMETERS, int salary);
    void setSalary(int payment);
    int getSalary() const;
    int calculateSalary() const override;
};

Worker::Worker(WORKER_TAKE_PARAMETERS) : name_(name), position_(position), age_(age) {}

void Worker::setName(const std::string &name) { this->name_ = name; }

const std::string &Worker::getName() const { return name_; }

void Worker::setPosition(const std::string &position) { this->position_ = position; }

const std::string &Worker::getPosition() const { return position_; }

void Worker::setAge(int age) { this->age_ = age; }

int Worker::getAge() const { return age_; }

Pieceworker::Pieceworker(WORKER_TAKE_PARAMETERS, int paymentPerHour, int workTime)
    : Worker(WORKER_GIVE_PARAMETERS), paymentPerHour_(paymentPerHour), workTime_(workTime) {}

void Pieceworker::setPayment(int payment) { this->paymentPerHour_ = payment; }

int Pieceworker::getPayment() const { return paymentPerHour_; }

void Pieceworker::setWorkTime(int hours) { this->workTime_ = hours; }

int Pieceworker::getWorkTime() const { return workTime_; }

int Pieceworker::calculateSalary() const { return workTime_ * paymentPerHour_; }

Employee::Employee(WORKER_TAKE_PARAMETERS, int salary) : Worker(WORKER_GIVE_PARAMETERS), salary_(salary) {}

void Employee::setSalary(int salary) { this->salary_ = salary; }

int Employee::getSalary() const { return salary_; }

int Employee::calculateSalary() const { return salary_; }