#ifndef CJsonRAII_hpp
#define CJsonRAII_hpp
#include "../cJSON/cJSON.h"

class CJsonRAII {
public:
    explicit CJsonRAII(cJSON* ptr) noexcept : json_(ptr) {}
    ~CJsonRAII();

    // Non-copyable
    CJsonRAII(const CJsonRAII&) = delete;
    CJsonRAII& operator=(const CJsonRAII&) = delete;

    // Allow move semantics
    CJsonRAII(CJsonRAII&& other) noexcept : json_(other.json_) {
        other.json_ = nullptr;
    }
    CJsonRAII& operator=(CJsonRAII&& other) noexcept {
        if (this != &other) {
            cleanup();
            json_ = other.json_;
            other.json_ = nullptr;
        }
        return *this;
    }

    cJSON* get() const noexcept { return json_; }

private:
    void cleanup() noexcept;
    cJSON* json_;
};
#endif