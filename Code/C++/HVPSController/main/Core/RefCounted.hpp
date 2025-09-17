#pragma once

#include <atomic>
#include <type_traits>

class RefCounted {
public:
    // Increments the intrusive reference count.
    void addRef() const noexcept;

    // Decrements the intrusive reference count; deletes `this` at zero.
    void release() const noexcept;

protected:
    RefCounted() noexcept;                // starts with refcount = 1
    virtual ~RefCounted() = default;      // polymorphic delete

    RefCounted(const RefCounted&) = delete;
    RefCounted& operator=(const RefCounted&) = delete;
    RefCounted(RefCounted&&) = delete;
    RefCounted& operator=(RefCounted&&) = delete;

private:
    mutable std::atomic<int> _rc;
};
