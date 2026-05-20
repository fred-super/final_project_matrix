#include "matrix.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <matrix_file.txt or matrix_file.csv>\n";
        std::cerr << "Example: " << argv[0] << " matrix_50x50_test.txt\n";
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file: " << fileName << std::endl;
        return 1;
    }

    std::vector<std::vector<double>> data;
    std::string line;

    while (std::getline(file, line))
    {
        size_t firstSymbol = line.find_first_not_of(" \t\r\n");

        if (firstSymbol == std::string::npos)
        {
            continue;
        }

        if (line[firstSymbol] == '#')
        {
            continue;
        }

        std::replace(line.begin(), line.end(), ',', ' ');

        std::stringstream ss(line);
        double value;
        std::vector<double> row;

        while (ss >> value)
        {
            row.push_back(value);
        }

        if (!row.empty())
        {
            data.push_back(row);
        }
    }

    if (data.empty())
    {
        std::cerr << "Error: file is empty or has invalid format." << std::endl;
        return 1;
    }

    size_t rows = data.size();
    size_t cols = data[0].size();

    for (const auto &row : data)
    {
        if (row.size() != cols)
        {
            std::cerr << "Error: matrix rows have different number of elements." << std::endl;
            return 1;
        }
    }

    try
    {
        if (cols == rows)
        {
            matrix::Matrix<double> A(rows, cols);

            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    A.at(i, j) = data[i][j];
                }
            }

            double det = A.determinant();

            std::cout << "Loaded square matrix: " << rows << " x " << cols << std::endl;
            std::cout << "Determinant: " << det << std::endl;
        }
        else if (cols == rows + 1)
        {
            matrix::Matrix<double> A(rows, rows);
            std::vector<double> B(rows);

            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < rows; ++j)
                {
                    A.at(i, j) = data[i][j];
                }

                B[i] = data[i][rows];
            }

            std::vector<double> x = A.solve(B);

            std::cout << "Loaded augmented matrix for SLAE: " << rows << " x " << cols << std::endl;
            std::cout << "Solution: ";

            for (size_t i = 0; i < x.size(); ++i)
            {
                std::cout << "x" << i + 1 << " = " << x[i];

                if (i + 1 < x.size())
                {
                    std::cout << "; ";
                }
            }

            std::cout << std::endl;
        }
        else
        {
            std::cerr << "Error: unsupported matrix format." << std::endl;
            std::cerr << "Expected N x N matrix for determinant or N x (N+1) matrix for SLAE."
                      << std::endl;
            std::cerr << "Your file has: " << rows << " rows and " << cols << " columns."
                      << std::endl;
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Calculation error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
