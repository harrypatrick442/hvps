#include "ArrayHelper.hpp"

// Nothing here yet, since templates must stay in the header.
// You could add explicit instantiations here if you want to reduce compile times:

// template double ArrayHelper::average<int>(const int* arr, size_t length);
template double ArrayHelper::average<double>(const double* arr, size_t length);
// template double ArrayHelper::average<uint64_t>(const uint64_t* arr, size_t length);
