#include "../include/matrix.hpp"

int main() 
{
    int n;
    std::cin >> n;
    if (!std::cin || n < 0) 
    {
        std::cerr << "Error: Invalid matrix size. Expected non-negative integer." << std::endl;
        return 1;
    }
     
    int ret = matrix::create_matrix<double>(n);
    return ret;
   
}

