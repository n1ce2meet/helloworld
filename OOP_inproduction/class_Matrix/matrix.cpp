#include <cstdlib>
#include <iostream>
typedef unsigned long size_t;

class Random
{
    int min_;
    int max_;

public:
    Random(int start, int end) : min_(start), max_(end) {}
    int operator()()
    {
        return min_ + std::rand() % (max_ - min_ + 1);
    }
};

template <typename T>
class Matrix
{
    T **data_;
    size_t rows_;
    size_t cols_;
    size_t size_;

    size_t col_max(Matrix &tempMatrix, int col) const// Нахождение колонны с максимальным элементом
    {
        T max = std::abs(data_[col][col]);
        size_t maxPos = col;
        for (size_t i = col + 1; i < rows_; ++i)
        {
            T element = std::abs(data_[i][col]);
            if (element > max)
            {
                max = element;
                maxPos = i;
            }
        }
        return maxPos;
    }

    size_t triangulation(Matrix &tempMatrix) const// https://en.wikipedia.org/wiki/Triangular_matrix для квадратной матрицы
    {
        size_t swapCount = 0;
        for (size_t i = 0; i < cols_ - 1; ++i)
        {
            size_t imax = col_max(tempMatrix, i);
            if (i != imax)
            {
                T *tempLink = tempMatrix[i];
                tempMatrix[i] = tempMatrix[imax];
                tempMatrix[imax] = tempLink;
                ++swapCount;
            }
            for (size_t j = i + 1; j < rows_; ++j)
            {
                T mul = -tempMatrix[j][i] / tempMatrix[i][i];
                for (size_t k = i; k < rows_; ++k)
                {
                    tempMatrix[j][k] += tempMatrix[i][k] * mul;
                }
            }
        }
        return swapCount; // для определения знака определителя
    }

public:
    Matrix(size_t rows = 2, size_t cols = 2) : rows_(rows), cols_(cols), size_(rows_ * cols_)
    {
        data_ = new T *[rows_];
        data_[0] = new T[size_]{};

        for (size_t i = 1; i < rows_; ++i)
        {
            data_[i] = data_[0] + cols_ * i;
        }
    }

    Matrix(Random rand, size_t rows = 2, size_t cols = 2) : Matrix(rows, cols)
    {
        for (size_t i = 0; i < size_; ++i)
        {
            data_[0][i] = rand();
        }
    }

    Matrix(const Matrix &other) : Matrix(other.rows_, other.cols_)
    {
        for (size_t i = 0; i < size_; ++i)
        {
            data_[0][i] = other.data_[0][i];
        }
    }

    ~Matrix()
    {
        delete[] data_[0];
        delete[] data_;
        data_ = nullptr;
    }

    Matrix &operator=(const Matrix &other)
    {
        if (this == &other)
            return *this;
        delete[] data_[0];
        delete[] data_;
        rows_ = other.rows_;
        cols_ = other.cols_;
        size_ = other.size_;
        data_ = new T *[rows_];
        data_[0] = new T[size_];
        for (size_t i = 1; i < rows_; ++i)
        {
            data_[0][i] = other.data_[0][i];
        }
        return *this;
    }

    T *&operator[](int index)
    {
        return data_[index];
    }

    const T *operator[](int index) const
    {
        return data_[index];
    }

    T gauss_determinant() const // Нахождение детерминанта методом гаусса https://www.youtube.com/watch?v=fE-PJkuE0S0
                          //в большинстве случаев реализация работает с дробными числами
    {
        Matrix<T> tempMatrix(*this);
        T* tempMStartLink = tempMatrix[0];
        size_t swapCount = triangulation(tempMatrix); // собираем верхнюю треугольную матрицу
        T determinant = 1;
        if (swapCount % 2)
            determinant = -1;
        for (size_t i = 0; i < rows_; ++i) //Перемножаем диагональ
        {
            determinant *= tempMatrix[i][i];
        }
        tempMatrix[0] = tempMStartLink;
        return determinant;
    }

    Matrix inverse() const{
        T determinant = this->gauss_determinant();
        Matrix result(*this);
        for (size_t i = 0; i < result.size(); ++i)
        {
            result[0][i] /= determinant;
        }
        return result;
    }

    size_t size() const { return size_; }
    size_t height() const { return rows_; }
    size_t width() const { return cols_; }
};

template <typename T>
Matrix<T> operator+(const Matrix<T> &first, const Matrix<T> &second)
{
    if (first.width() != second.width() or first.height() != second.height())
        throw "Matrices must be the same size!";
    Matrix result(first);
    for (size_t i = 0; i < result.size(); ++i)
    {
        result[0][i] += second[0][i];
    }
    return result;
}

template <typename T>
Matrix<T> operator-(const Matrix<T> &first, const Matrix<T> &second)
{
    if (first.width() != second.width() or first.height() != second.height())
        throw "Matrices must be the same size!";
    Matrix result(first);
    for (size_t i = 0; i < result.size(); ++i)
    {
        result[0][i] -= second[0][i];
    }
    return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T> &matrix, int number)
{
    Matrix result(matrix);
    for (size_t i = 0; i < result.size(); ++i)
    {
        result[0][i] *= number;
    }
    return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T> &first, const Matrix<T> &second)
{
    if (first.width() != second.height())
        throw "The width of the first matrix must be equal to the height of the second!";
    Matrix<T> result(first.height(), second.width());
    for (size_t row = 0; row < result.height(); ++row)
    {
        for (size_t col = 0; col < result.width(); ++col)
        {
            for (size_t mul_row = 0, mul_col = 0; mul_row < first.height(), mul_col < second.width(); ++mul_row, ++mul_col)
            {
                result[row][col] += first[row][mul_col] * second[mul_row][col];
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> operator/(const Matrix<T> &first, const Matrix<T> &second)
{
    return first * second.inverse();
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &m)
{
    for (size_t row = 0; row < m.height(); ++row)
    {
        for (size_t col = 0; col < m.width(); ++col)
        {
            os << m[row][col] << ' ';
        }
        os << '\n';
    }
    os << std::endl;
    return os;
}

//#define CONSTRUCTORS_CHECK
#define OPERATORS_CHECK
#define LOGIC_CHECK

int main()
{
    Random r(1, 10);

#ifdef CONSTRUCTORS_CHECK

    Matrix<double> m1(r, 4, 4);
    std::cout << m1;
    Matrix<double> m2(5, 5);
    std::cout << m2;
    Matrix<double> m3(m2);
    std::cout << m3;

#endif

#ifdef OPERATORS_CHECK

    Matrix<double> m4(r,3,3);
    Matrix<double> m5(r,3,3);
    std::cout << m4;
    std::cout << m5;
    std::cout << m4 + m5;
    std::cout << m4 - m5;
    std::cout << m4 * 3;
    std::cout << m4 * m5;
    std::cout << m4 / m5;

#endif
}
