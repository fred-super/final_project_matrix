#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <span>
#include <type_traits>
#include <vector>

#ifdef USE_EIGEN
#include <Eigen/Dense>
#endif

namespace matrix
{
    enum class CalculationBackend
    {
        OwnGauss,
        Eigen
    };

    template <typename type>
    class MatrixMemoryManager 
    {
    protected:
        size_t rows, cols;
        type* data;

        void allocateMemory() 
        {
            if (rows > 0 && cols > 0) 
            {
                data = new type[rows * cols]{};
            } 
            else 
            {
                data = nullptr;
            }
        }

        void clearMemory() 
        {
            if (data != nullptr) 
            {
                delete[] data;
                data = nullptr;
            }
        }

        void swapMemory(MatrixMemoryManager& other) noexcept
        {
            using std::swap;
            swap(rows, other.rows);
            swap(cols, other.cols);
            swap(data, other.data);
        }

    public:
        MatrixMemoryManager(): rows(0), cols(0), data(nullptr) 
        {
        }
        
        MatrixMemoryManager(size_t r, size_t c): rows(r), cols(c), data(nullptr) 
        {
            if (rows > 0 && cols > 0) 
            {
                allocateMemory();
            }
        }
        
        MatrixMemoryManager(const MatrixMemoryManager&) = delete;

        MatrixMemoryManager& operator=(const MatrixMemoryManager&) = delete;
        
        MatrixMemoryManager(MatrixMemoryManager&& other) noexcept : MatrixMemoryManager() 
        {
            swapMemory(other);
        }
        
        MatrixMemoryManager& operator=(MatrixMemoryManager&& other) noexcept 
        {
            if (this != &other) 
            {
                clearMemory();
                swapMemory(other);
            }
            return *this;
        }
        
        ~MatrixMemoryManager() 
        {
            clearMemory();
        }
    };

    template <typename type>
    class Matrix : private MatrixMemoryManager<type>
    {
    private:
        void swap(Matrix& other) noexcept
        {
            this->swapMemory(other);
        }

    public:
        Matrix(): MatrixMemoryManager<type>() 
        {
        }

        Matrix(size_t rows, size_t cols): MatrixMemoryManager<type>(rows, cols) 
        {
        }
        
        Matrix(size_t rows, size_t cols, std::span<const type> flat_array): MatrixMemoryManager<type>(rows, cols) 
        {
            if (rows > 0 && cols > 0) 
            {
                size_t required_elements = rows * cols;
                if (flat_array.size() != required_elements) 
                {
                    throw std::invalid_argument("Flat array wrong size");
                }
                
                for (size_t i = 0; i < rows; ++i) 
                {
                    for (size_t j = 0; j < cols; ++j) 
                    {
                        this->data[i * cols + j] = flat_array[i * cols + j];
                    }
                }
            }
        }

        Matrix(std::initializer_list<std::initializer_list<type>> init_list) 
        {
            if (init_list.size() == 0) 
            {
                return; 
            }

            auto first_row = init_list.begin();
            this->cols = first_row->size();
            this->rows = init_list.size();
            
            for (const auto& row : init_list) 
            {
                if (row.size() != this->cols) 
                {
                    throw std::invalid_argument("All rows must have the same number of columns");
                }
            }

            this->allocateMemory();
            
            size_t i = 0;
            for (const auto& row_list : init_list) 
            {
                size_t j = 0;
                for (const auto& element : row_list) 
                {
                    this->data[i * this->cols + j] = element;
                    ++j;
                }
                ++i;
            }
        }

        template <typename InputIt>
        Matrix(size_t rows, size_t cols, InputIt first, InputIt last): MatrixMemoryManager<type>(rows, cols)
        {
            size_t total_elements = std::distance(first, last);
            if (rows * cols != total_elements)
            {
                throw std::invalid_argument("Flat array wrong size");
            }
            auto it = first;
            for (size_t i = 0; i < rows; ++i) 
            {
                for (size_t j = 0; j < cols; ++j) 
                {
                    if (it != last) 
                    {
                        this->data[i * cols + j] = *it++;
                    }
                }
            }
        }

        Matrix(size_t rows, size_t cols, const type& value): MatrixMemoryManager<type>(rows, cols) 
        {
            if (rows > 0 && cols > 0) 
            {
                for (size_t i = 0; i < rows; ++i) 
                {
                    for (size_t j = 0; j < cols; ++j) 
                    {
                        this->data[i * cols + j] = value;
                    }
                }
            }
        }

        Matrix(const Matrix& other) : MatrixMemoryManager<type>() 
        {
            if (other.data != nullptr) 
            {
                type* new_data = nullptr;
                try 
                {
                    new_data = new type[other.rows * other.cols];
                    
                    for (size_t i = 0; i < other.rows; ++i) 
                    {
                        for (size_t j = 0; j < other.cols; ++j) 
                        {
                            new_data[i * other.cols + j] = other.data[i * other.cols + j];
                        }
                    }
                    
                    this->rows = other.rows;
                    this->cols = other.cols;
                    this->data = new_data;
                } 
                catch (...) 
                {
                    if (new_data != nullptr) 
                    {
                        delete[] new_data;
                    }
                    throw; 
                }
            }
        }

        Matrix& operator=(Matrix other) 
        {
            swap(other);
            return *this;
        }    

        Matrix(Matrix&& other) noexcept : Matrix() 
        {
            swap(other);
        }   

        Matrix& operator=(Matrix&& other) noexcept 
        {
            if (this != &other) 
            {
                swap(other);
            }
            return *this;
        }

        type& at(size_t row, size_t col) 
        {
            if (row >= this->rows || col >= this->cols) 
            {
                throw std::out_of_range("Matrix indices out of range");
            }
            return this->data[row * this->cols + col];
        }

        const type& at(size_t row, size_t col) const 
        {
            if (row >= this->rows || col >= this->cols) 
            {
                throw std::out_of_range("Matrix indices out of range");
            }
            return this->data[row * this->cols + col];
        }

        size_t getRows() const 
        { 
            return this->rows; 
        }

        size_t getCols() const 
        { 
            return this->cols; 
        }

        void print() const 
        {
            for (size_t i = 0; i < this->rows; ++i) 
            {
                for (size_t j = 0; j < this->cols; ++j) 
                {
                    std::cout << this->data[i * this->cols + j] << " ";
                }
                std::cout << std::endl;
            }
        }

        type determinant(CalculationBackend backend = CalculationBackend::OwnGauss) const
        {
            if (this->rows != this->cols || this->rows == 0)
            {
                throw std::invalid_argument("Matrix must be square for determinant calculation");
            }
        
            using CalcType = std::conditional_t<std::is_integral_v<type>, double, type>;
        
        #ifdef USE_EIGEN
            if (backend == CalculationBackend::Eigen)
            {
                Eigen::Matrix<CalcType, Eigen::Dynamic, Eigen::Dynamic> eigenMatrix(
                    static_cast<Eigen::Index>(this->rows),
                    static_cast<Eigen::Index>(this->cols)
                );
        
                for (size_t i = 0; i < this->rows; ++i)
                {
                    for (size_t j = 0; j < this->cols; ++j)
                    {
                        eigenMatrix(
                            static_cast<Eigen::Index>(i),
                            static_cast<Eigen::Index>(j)
                        ) = static_cast<CalcType>(this->at(i, j));
                    }
                }
        
                CalcType det = eigenMatrix.partialPivLu().determinant();
        
                if (std::abs(det) < static_cast<CalcType>(1e-10))
                {
                    return static_cast<type>(0);
                }
        
                if constexpr (std::is_integral_v<type>)
                {
                    return static_cast<type>(std::round(det));
                }
                else
                {
                    return static_cast<type>(det);
                }
            }
        #else
            if (backend == CalculationBackend::Eigen)
            {
                throw std::runtime_error("Eigen backend is not available. Build project with -DUSE_EIGEN=ON.");
            }
        #endif
        
            Matrix<CalcType> temp(this->rows, this->cols);
        
            for (size_t i = 0; i < this->rows; ++i)
            {
                for (size_t j = 0; j < this->cols; ++j)
                {
                    temp.at(i, j) = static_cast<CalcType>(this->at(i, j));
                }
            }
        
            CalcType det = 1;
            const CalcType epsilon = static_cast<CalcType>(1e-10);
        
            for (size_t i = 0; i < this->rows; ++i)
            {
                size_t pivot = i;
        
                for (size_t j = i + 1; j < this->rows; ++j)
                {
                    if (std::abs(temp.at(j, i)) > std::abs(temp.at(pivot, i)))
                    {
                        pivot = j;
                    }
                }
        
                if (std::abs(temp.at(pivot, i)) < epsilon)
                {
                    return static_cast<type>(0);
                }
        
                if (pivot != i)
                {
                    for (size_t k = 0; k < this->cols; ++k)
                    {
                        std::swap(temp.at(i, k), temp.at(pivot, k));
                    }
        
                    det = -det;
                }
        
                det *= temp.at(i, i);
        
                for (size_t j = i + 1; j < this->rows; ++j)
                {
                    CalcType factor = temp.at(j, i) / temp.at(i, i);
        
                    for (size_t k = i + 1; k < this->cols; ++k)
                    {
                        temp.at(j, k) -= factor * temp.at(i, k);
                    }
                }
            }
        
            if constexpr (std::is_integral_v<type>)
            {
                return static_cast<type>(std::round(det));
            }
            else
            {
                return static_cast<type>(det);
            }
        }

        std::vector<double> solve(
            const std::vector<double>& b,
            CalculationBackend backend = CalculationBackend::OwnGauss
        ) const
        {
            if (this->rows != this->cols)
            {
                throw std::invalid_argument("Matrix must be square");
            }
        
            if (b.size() != this->rows)
            {
                throw std::invalid_argument("Vector B size mismatch");
            }
        
            size_t n = this->rows;
        
        #ifdef USE_EIGEN
            if (backend == CalculationBackend::Eigen)
            {
                Eigen::MatrixXd A(
                    static_cast<Eigen::Index>(n),
                    static_cast<Eigen::Index>(n)
                );
        
                Eigen::VectorXd B(static_cast<Eigen::Index>(n));
        
                for (size_t i = 0; i < n; ++i)
                {
                    B(static_cast<Eigen::Index>(i)) = b[i];
        
                    for (size_t j = 0; j < n; ++j)
                    {
                        A(
                            static_cast<Eigen::Index>(i),
                            static_cast<Eigen::Index>(j)
                        ) = static_cast<double>(this->at(i, j));
                    }
                }
        
                Eigen::FullPivLU<Eigen::MatrixXd> lu(A);
                lu.setThreshold(1e-12);
        
                if (!lu.isInvertible())
                {
                    throw std::runtime_error("Degenerate matrix");
                }
        
                Eigen::VectorXd solution = lu.solve(B);
        
                std::vector<double> x(n);
        
                for (size_t i = 0; i < n; ++i)
                {
                    x[i] = solution(static_cast<Eigen::Index>(i));
                }
        
                return x;
            }
        #else
            if (backend == CalculationBackend::Eigen)
            {
                throw std::runtime_error("Eigen backend is not available. Build project with -DUSE_EIGEN=ON.");
            }
        #endif
        
            Matrix<double> A(n, n);
            std::vector<double> x(n);
            std::vector<double> temp_b = b;
        
            for (size_t i = 0; i < n; ++i)
            {
                for (size_t j = 0; j < n; ++j)
                {
                    A.at(i, j) = static_cast<double>(this->at(i, j));
                }
            }
        
            for (size_t i = 0; i < n; ++i)
            {
                size_t pivot = i;
        
                for (size_t j = i + 1; j < n; ++j)
                {
                    if (std::abs(A.at(j, i)) > std::abs(A.at(pivot, i)))
                    {
                        pivot = j;
                    }
                }
        
                if (std::abs(A.at(pivot, i)) < 1e-12)
                {
                    throw std::runtime_error("Degenerate matrix");
                }
        
                std::swap(temp_b[i], temp_b[pivot]);
        
                for (size_t k = 0; k < n; ++k)
                {
                    std::swap(A.at(i, k), A.at(pivot, k));
                }
        
                for (size_t j = i + 1; j < n; ++j)
                {
                    double factor = A.at(j, i) / A.at(i, i);
        
                    temp_b[j] -= factor * temp_b[i];
        
                    for (size_t k = i; k < n; ++k)
                    {
                        A.at(j, k) -= factor * A.at(i, k);
                    }
                }
            }
        
            for (int i = static_cast<int>(n) - 1; i >= 0; --i)
            {
                double sum = 0;
        
                for (size_t j = static_cast<size_t>(i) + 1; j < n; ++j)
                {
                    sum += A.at(static_cast<size_t>(i), j) * x[j];
                }
        
                x[static_cast<size_t>(i)] =
                    (temp_b[static_cast<size_t>(i)] - sum) /
                    A.at(static_cast<size_t>(i), static_cast<size_t>(i));
            }
        
            return x;
        }

        friend void swap(Matrix& first, Matrix& second) noexcept
        {
            first.swap(second);
        }
    };

    template <typename typet>
    int create_matrix(int n)
    {
        matrix::Matrix<typet> mat(n, n);
        for (size_t i = 0; i < n; ++i) 
        {
            for (size_t j = 0; j < n; ++j) 
            {
                std::cin >> mat.at(i, j);
                if (!std::cin) 
                {
                    std::cerr << "Error: Invalid input for matrix element at position (" 
                            << i << ", " << j << "). Expected a number." << std::endl;
                    return 1;
                }
            }
        }
    
        if (!std::cin) 
        {
            std::cerr << "Error: Failed to read matrix elements." << std::endl;
            return 1;
        }

        try 
        {
            typet det = mat.determinant();
            std::cout << det << std::endl;
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error calculating determinant: " << e.what() << std::endl;
            return 1;
        }
        return 0;
    }
}

#endif