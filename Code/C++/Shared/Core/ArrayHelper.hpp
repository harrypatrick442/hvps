#pragma once
#include <type_traits>
#include <cstddef>

class ArrayHelper {
public:
    // Generic average function for arithmetic types only
    template <typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value, float>::type
    average(const T* arr, size_t length) {
        if (length == 0) return 0.0;
        float sum = 0.0;
        for (size_t i = 0; i < length; i++) {
            sum += static_cast<float>(arr[i]);
        }
        return sum / static_cast<float>(length);
    }
};
