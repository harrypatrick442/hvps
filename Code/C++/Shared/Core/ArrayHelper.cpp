#include "ArrayHelper.hpp"

// Nothing here yet, since templates must stay in the header.
// You could add explicit instantiations here if you want to reduce compile times:

// template double ArrayHelper::average<int>(const int* arr, size_t length);
template float ArrayHelper::average<float>(const float* arr, size_t length);
// template double ArrayHelper::average<uint64_t>(const uint64_t* arr, size_t length);
