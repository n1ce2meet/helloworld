#include <string>
#include <array>
#include <cmath>

static constexpr double PI = 3.1415;

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
    std::array<double, 3> angle_;

    double calculate_angle_(double &sideA, double &sideB, double &sideC) // теорема косинусов
    {
        double resultInRad;
        resultInRad = ((sideA * sideA + sideB * sideB) - sideC * sideC) / 2 * sideA * sideB;
        return std::cos(resultInRad * PI / 180);
    }

    double calculate_side_(const Point3D &first, const Point3D second)
    {
        return std::sqrt(std::pow((first.x_ - second.x_), 2) + std::pow((first.y_ - second.y_), 2));
    }

public:
    Triangle(std::array<Point3D, 3> &&points) : vertices_(points)
    {
        sides_[0] = calculate_side_(vertices_[0], vertices_[1]);
        sides_[1] = calculate_side_(vertices_[0], vertices_[2]);
        sides_[2] = calculate_side_(vertices_[1], vertices_[2]);

        angle_[0] = calculate_angle_(sides_[0], sides_[1], sides_[2]);
        angle_[1] = calculate_angle_(sides_[0], sides_[2], sides_[1]);
        angle_[2] = calculate_angle_(sides_[1], sides_[2], sides_[0]);
    }

    Triangle(const Triangle &) = default;
    Triangle(Triangle &&) = default;

    Triangle &operator=(const Triangle &) = default;
    Triangle &operator=(Triangle &&) = default;

    virtual double area() const override
    {
        double p = this->perimeter() / 2;
        return std::sqrt(p * (p - sides_[0]) * (p - sides_[1]) * (p - sides_[2]));
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
        std::string name;
        std::string sides("AB - " + std::to_string(sides_[0]) + ", AC - " + std::to_string(sides_[1]) + ", BC - " + std::to_string(sides_[2]));
        std::string angles("alpha - " + std::to_string(angle_[0]) + ", beta - " + std::to_string(angle_[1]) + ", gamma - " + std::to_string(angle_[2]));

        if (isRight())
            name = "Right triangle.";
        else if (isIsosceles())
            name = "Isosceles triangle.";
        else if (isEquilateral())
            name = "Equilateral triangle.";
        else
            name = "Free triangle.";

        return (name + "Sides: " + sides + "\n\t Angles: " + angles);
    }

    bool isRight() const
    {
        return angle_[0] == 90 or angle_[1] == 90 or angle_[2] == 90;
    }

    bool isIsosceles() const
    {
        return sides_[0] == sides_[1] or sides_[0] == sides_[2] or sides_[1] == sides_[2];
    }

    bool isEquilateral() const
    {
        return sides_[0] == sides_[1] and sides_[0] == sides_[2];
    }
};