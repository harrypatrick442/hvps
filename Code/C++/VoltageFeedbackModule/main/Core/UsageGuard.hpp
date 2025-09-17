#include <type_traits>
#include "RefCounted.hpp"
template <class T>
class UsageGuard { 
    using U = std::remove_cv_t<T>;
    static_assert(std::is_base_of<U, U>::value, "");                // no-op
    static_assert(std::is_base_of<RefCounted, U>::value,
                  "UsageGuard<T>: T must derive from RefCounted");
public:
    explicit UsageGuard(const T* p) noexcept : _p(p) { if (_p) _p->addRef(); }
    ~UsageGuard() { if (_p) _p->release(); }

    UsageGuard(const UsageGuard&) = delete;
    UsageGuard& operator=(const UsageGuard&) = delete;
    UsageGuard(UsageGuard&& other) noexcept : _p(other._p) { other._p = nullptr; }
    UsageGuard& operator=(UsageGuard&& other) noexcept {
        if (this != &other) { if (_p) _p->release(); _p = other._p; other._p = nullptr; }
        return *this;
    }

    const T* get() const noexcept { return _p; }
    const T& operator*()  const noexcept { return *_p; }
    const T* operator->() const noexcept { return _p; }

    void reset() noexcept { if (_p) { _p->release(); _p = nullptr; } }

private:
    const T* _p;   // note: const T* so it works from const methods
};
