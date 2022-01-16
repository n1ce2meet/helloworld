#include <string>
#include <array>
#include <cmath>

struct Point3D
{
    float x_;
    float y_;
    float z_;
    Point3D(float x, float y, float z) : x_(x), y_(y), z_(z) {}
};

class Shape
{
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string parameters() const = 0;
    // virtual void draw() const = 0; получаем вершины и передаем в opengl
};

class Rectangle : public Shape
{
    size_t width_;
    size_t height_;

public:
    Rectangle(size_t width, size_t height) : width_(width), height_(height) {}
    Rectangle(const Rectangle &) = default;
    Rectangle(Rectangle &&) = default;
    ~Rectangle() = default;

    Rectangle &operator=(const Rectangle &) = default;
    Rectangle &operator=(Rectangle &&) = default;

    virtual double area() const override { return width_ * height_; }
    virtual double perimeter() const override { return width_ + height_ * 2; }
    virtual std::string parameters() const override
    {
        return ("Rectangle. Width: " + std::to_string(width_) + ", height: " + std::to_string(height_));
    }

    size_t get_width() const { return width_; }
    size_t get_height() const { return height_; }
};

class Square : public Rectangle
{
public:
    Square(size_t size) : Rectangle(size, size) {}
    Square(const Square &) = default;
    Square(Square &&) = default;

    Square &operator=(const Square &) = default;
    Square &operator=(Square &&) = default;

    virtual std::string parameters() const override
    {
        return ("Square. Size: " + std::to_string(this->get_width()));
    }
};

class Circle : public Shape
{
    size_t radius_;

public:
    static constexpr double PI = 3.1415;

    Circle(size_t radius) : radius_(radius) {}
    Circle(const Circle &) = default;
    Circle(Circle &&) = default;
    ~Circle() = default;

    Circle &operator=(const Circle &) = default;
    Circle &operator=(Circle &&) = default;

    virtual double area() const override { return radius_ * radius_ * PI; }
    virtual double perimeter() const override { return radius_ * PI * 2; }
    virtual std::string parameters() const override
    {
        return ("Circle. Radius: " + std::to_string(radius_));
    }

    size_t get_radius() const { return radius_; }
};

class Triangle : public Shape
{
    std::array<Point3D, 3> vertices_;
    std::array<double, 3> sides_;

public:
    Triangle(std::array<Point3D, 3> &&points) : vertices_(points)
    {
        sides_[0] = std::sqrt(std::pow((vertices_[0].x_ - vertices_[1].x_), 2) + std::pow((vertices_[0].y_ - vertices_[1].y_), 2));
        sides_[1] = std::sqrt(std::pow((vertices_[0].x_ - vertices_[2].x_), 2) + std::pow((vertices_[0].y_ - vertices_[2].y_), 2));
        sides_[2] = std::sqrt(std::pow((vertices_[1].x_ - vertices_[2].x_), 2) + std::pow((vertices_[1].y_ - vertices_[2].y_), 2));
    }
    Triangle(const Triangle &) = default;
    Triangle(Triangle &&) = default;

    Triangle &operator=(const Triangle &) = default;
    Triangle &operator=(Triangle &&) = default;

    virtual double area() const override
    {
        double p = this->perimeter() / 2;
        return std::sqrt(p*(p-sides_[0])*(p-sides_[1])*(p-sides_[2]));
    }

    virtual double perimeter() const override
    {
        double result = 0;

        for (auto i : sides_)
        {
            result += i;
        }

        return result;
    }

    virtual std::string parameters() const override
    {
        return ("Triangle. Sides: " + std::to_string(sides_[0]) + ' ' + std::to_string(sides_[1]) + ' ' + std::to_string(sides_[2]));
    }
};