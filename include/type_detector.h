#ifndef TYPE_DETECTOR_HPP
#define TYPE_DETECTOR_HPP

#include <string_view>
#include <cctype>
#include <algorithm>

namespace matrix 
{

enum class DataType 
{
    Invalid = -1,
    Int = 0,
    Double = 1,
    LongDouble = 2
};

class TypeDetector 
{
public:
    static DataType detectSingleValue(std::string_view str) 
    {
        if (str.empty()) return DataType::Invalid;

        bool has_dot = false;
        bool has_exp = false;
        size_t digit_count = 0;

        for (char c : str) 
        {
            if (c == '.') 
            {
                if (has_dot) return DataType::Invalid; 
                has_dot = true;
            } 
            else if (c == 'e' || c == 'E') {
                if (has_exp) return DataType::Invalid; 
                has_exp = true;
            } 
            else if (std::isdigit(c)) 
            {
                digit_count++;
            } 
            else if (c != '+' && c != '-' && !std::isspace(c)) 
            {
                return DataType::Invalid; 
            }
        }

        if (has_dot || has_exp) 
        { 
            if (digit_count > 15) 
            {
                return DataType::LongDouble;
            }
            return DataType::Double;
        }

       
        if (digit_count >= 10) 
        {
            return DataType::Double;
        }

        return DataType::Int;
    }

    static DataType promoteTypes(DataType a, DataType b) 
    {
        if (a == DataType::Invalid || b == DataType::Invalid) return DataType::Invalid;
        return static_cast<DataType>(std::max(static_cast<int>(a), static_cast<int>(b)));
    }
};

}

#endif 