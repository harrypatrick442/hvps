#include "CJsonRAII.hpp"
void CJsonRAII::cleanup() noexcept {
    if (json_) {
        cJSON_Delete(json_);
        json_ = nullptr;
    }
}

CJsonRAII::~CJsonRAII() {
    cleanup();
}
