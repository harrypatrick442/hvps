// ===== Generic delegate type =====
#pragma once
#include <utility>
template<class Sig>
struct Delegate;                          // primary template (unspecified)

template<class R, class... Args>
struct Delegate<R(Args...)> {             // specialization for any signature
    using Fn = R(*)(void*, Args...);

    void* ctx = nullptr;
    Fn    fn  = nullptr;

    R operator()(Args... args) const {
        return fn(ctx, std::forward<Args>(args)...);
    }

    explicit operator bool() const { return fn != nullptr; }
};

// Helper: bind an object + member function pointer into a Delegate
template<class T, class R, class... Args>
Delegate<R(Args...)> make_delegate(T* obj, R (T::*mf)(Args...)) {
    return { obj,
        +[](void* c, Args... as) -> R {
            return (static_cast<T*>(c)->*mf)(std::forward<Args>(as)...);
        }
    };
}

// const-qualified member:
template<class T, class R, class... Args>
Delegate<R(Args...)> make_delegate(const T* obj, R (T::*mf)(Args...) const) {
    return { const_cast<T*>(obj),  // safe for calling const member
        +[](void* c, Args... as) -> R {
            return (static_cast<const T*>(c)->*mf)(std::forward<Args>(as)...);
        }
    };
}

/*
template<class R, class... Args>
struct Delegate<R(Args...)> {
  using Fn = R(*)(void*, Args...);
  RefCounted* ctx = nullptr;
  Fn fn = nullptr;

  Delegate() = default;
  Delegate(RefCounted* c, Fn f): ctx(c), fn(f) { if (ctx) ctx->addRef(); }
  Delegate(const Delegate& d): ctx(d.ctx), fn(d.fn) { if (ctx) ctx->addRef(); }
  Delegate& operator=(const Delegate& d){ if (this!=&d){ if (ctx) ctx->release(); ctx=d.ctx; fn=d.fn; if (ctx) ctx->addRef(); } return *this; }
  ~Delegate(){ if (ctx) ctx->release(); }

  R operator()(Args... as) const { return fn(ctx, std::forward<Args>(as)...); }
  explicit operator bool() const { return fn != nullptr; }
};
*/