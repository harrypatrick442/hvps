#pragma once
#include <functional>
#include <vector>
#include <cstddef>
#include <utility>
#include <type_traits>

// Forward-declare cJSON so the header doesn't force-include it
struct cJSON;

class CleanupBucket {
public:
    CleanupBucket() = default;
    ~CleanupBucket();                               // runs all pending cleanups (LIFO)

    // Non-copyable, but movable
    CleanupBucket(const CleanupBucket&) = delete;
    CleanupBucket& operator=(const CleanupBucket&) = delete;
    CleanupBucket(CleanupBucket&& other) noexcept;
    CleanupBucket& operator=(CleanupBucket&& other) noexcept;

    // Introspection
    std::size_t size() const noexcept { return actions_.size(); }
    bool empty() const noexcept { return actions_.empty(); }

    // Add a generic cleanup callback (executed on destruction or runAndClear()).
    void addCallback(std::function<void()> fn);

    // Add a pointer + deleter (deleter(ptr) will be called if ptr != nullptr).
    template <class T, class Deleter>
    void add(T* ptr, Deleter deleter) {
        if (!ptr) return;
        actions_.emplace_back([ptr, deleter]() noexcept { deleter(ptr); });
    }

    // Convenience helpers
    template <class T>
    void addDelete(T* ptr) { add(ptr, [](T* p) noexcept { delete p; }); }

    template <class T>
    void addDeleteArray(T* ptr) { add(ptr, [](T* p) noexcept { delete[] p; }); }

    // cJSON helper (defined in .cpp where cJSON_Delete is available)
    void addCJSON(cJSON* node);

    // Execute all cleanups now, then clear.
    void runAndClear() noexcept;

    // Discard all pending cleanups *without executing them*.
    // (Use with care: this intentionally leaks whatever was registered.)
    void clearWithoutRunning() noexcept { actions_.clear(); }

    void swap(CleanupBucket& other) noexcept { actions_.swap(other.actions_); }

private:
    std::vector<std::function<void()>> actions_;    // LIFO stack of cleanups
};

