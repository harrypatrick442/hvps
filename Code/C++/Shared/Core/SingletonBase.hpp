#pragma once
#include <mutex>
#include <utility>
#include <type_traits>
#include "System/SafeAbort.hpp"

// Trait to check for static getTag() method
template<typename T>
class has_static_getTag {
private:
    template<typename U>
    static auto test(int) -> decltype(
        std::is_same<
            decltype(U::getTag()),
            const char*
        >{},
        std::true_type{}
    );
    template<typename>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template<class Derived>
class SingletonBase {
public:
    template<class... Args>
    static Derived& initialize(Args&&... args) noexcept {
        static_assert(has_static_getTag<Derived>::value,
            "Derived must implement: static const char* getTag()");
        bool ran = false;
        std::call_once(_once, [&]{
            _instance = new Derived(std::forward<Args>(args)...);
            ran = true;
        });
        if (!ran) {
            SAFE_ABORT("%s Already initialized", Derived::getTag());
        }
        return *_instance;
    }

    static Derived& getInstance() noexcept {
        if (_instance) return *_instance;
        SAFE_ABORT("%s getInstance() before initialize()", Derived::getTag());
        return *_instance; // unreachable
    }

protected:
    ~SingletonBase() = default;

private:
    static inline std::once_flag _once;
    static inline Derived* _instance = nullptr;
};
