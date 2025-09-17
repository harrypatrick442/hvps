// SingletonBase.hpp
#pragma once
#include <mutex>
#include <utility>
#include <type_traits>
#include "../System/Aborter.hpp"

// detect Derived::TAG
template<class T, class = void>
struct has_tag : std::false_type {};
template<class T>
struct has_tag<T, std::void_t<decltype(T::TAG)>> : std::true_type {};

template<class Derived>
class SingletonBase {
public:
  template<class... Args>
  static Derived& initialize(const char* tag, Args&&... args) noexcept {
    bool ran = false;
    std::call_once(_once, [&]{
      _instance = new Derived(std::forward<Args>(args)...);
      _tag = tag;
      ran = true;
    });
    if (!ran) Aborter::safeAbort(_tag ? _tag : tag, "Already initialized");
    return *_instance;
  }

  // convenience overload: uses Derived::TAG
  template<class... Args>
  static Derived& initialize(Args&&... args) noexcept {
    static_assert(has_tag<Derived>::value,
                  "Derived must define static TAG or use initialize(tag, ...)");
    return initialize(Derived::TAG, std::forward<Args>(args)...);
  }

  static Derived& getInstance() noexcept {
    if (_instance) return *_instance;
    Aborter::safeAbort(_tag ? _tag : "Singleton", "getInstance() before initialize()");
    return *_instance; // unreachable post-abort
  }

protected:
  ~SingletonBase() = default;

private:
  static inline std::once_flag _once;
  static inline Derived*       _instance = nullptr;
  static inline const char*    _tag      = nullptr;
};
