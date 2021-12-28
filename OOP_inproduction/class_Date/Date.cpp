#include <algorithm>
#include <string>

// Date - точка во времени, Period - временной интервал между двумя Date

const unsigned DAYS_PER_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class Date
{
    friend class Period;

    unsigned year_;
    unsigned month_;
    unsigned day_;

public:
    Date(unsigned day, unsigned month, unsigned year) : year_(year), month_(month), day_(day) {}
    Date(const Date &other) : year_(other.year_), month_(other.month_), day_(other.day_) {}
    ~Date() {}

    Date &operator=(const Date &other)
    {
        if (this != &other)
        {
            year_ = other.year_;
            month_ = other.month_;
            day_ = other.day_;
        }
        return *this;
    }

    bool operator==(const Date &other) const
    {
        return year_ == other.year_ and month_ == other.month_ and day_ == other.day_;
    }

    bool operator!=(const Date &other) const
    {
        return !(*this == other);
    }

    bool operator<(const Date &other) const
    {
        if (year_ < other.year_)
            return true;
        else if (year_ == other.year_)
        {
            if (month_ < other.month_)
                return true;
            else if (month_ == other.month_)
                if (day_ < other.day_)
                    return true;
        }
        
        return false;
    }

    bool operator>(const Date &other) const
    {
        return other < *this;
    }

    unsigned year() const
    {
        return year_;
    }

    unsigned month() const
    {
        return month_;
    }

    unsigned day() const
    {
        return day_;
    }

    std::string to_string() const
    {
        return "Year: " + std::to_string(year_) + " Month: " + std::to_string(month_) + " Day: " + std::to_string(day_);
    }

    static bool isLeapYear(unsigned year)
    // Распределение високосных годов:
    //     год, номер которого кратен 400, — високосный;
    //     остальные годы, номер которых кратен 100, — невисокосные (например, годы 1700, 1800, 1900, 2100, 2200, 2300);
    //     остальные годы, номер которых кратен 4, — високосные.
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
                return true;
        }
        else if (year % 4 == 0)
            return true;

        return false;
    }
};

class Period
{
    Date from_;
    Date to_;
    unsigned differenceInYears_ = 0;
    unsigned differenceInMonths_ = 0;
    unsigned differenceInDays_ = 0;
    unsigned long long days_ = 0;

public:
    Period(const Date &from, const Date &to) : from_(from), to_(to) //конструктор к сожалению получился очень громоздкий
    {

        for (unsigned i = from.year_; i < to.year_; ++i)
        {
            ++differenceInYears_;
            days_ += (Date::isLeapYear(i) ? 366 : 365);
        }

        if (differenceInYears_)
        {
            differenceInMonths_ = to.month_ - 1;
            for (unsigned i = 0; i < differenceInMonths_; ++i)
            {
                days_ += DAYS_PER_MONTH[i];
            }
            if (Date::isLeapYear(to.year_) and to.month_ > 2)
                ++days_;
        }
        else
        {
            differenceInMonths_ = from.month_ - to.month_ - 1;
            for (unsigned i = from.month_; i < to.month_; ++i)
            {
                days_ += DAYS_PER_MONTH[i];
            }
            if (Date::isLeapYear(to.year_) and from.month_ < 2 and to.month_ > 2)
                ++days_;
        }

        if (!differenceInYears_ and !differenceInMonths_)
        {
            differenceInDays_ = from.day_ - to.day_;
            days_ += differenceInDays_;
        }
        else
        {
            differenceInDays_ = to.day_;
            days_ += differenceInDays_;
        }
    }

    unsigned long long days()
    {
        return days_;
    }

    bool contains(const Date &date)
    {
        return from_ < date and to_ > date;
    }
};

Period operator-(const Date &start, const Date &end)
{
    if (start < end)
        return Period(start, end);
    else
        return Period(end, start);
}
