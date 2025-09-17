#include "CleanupBucket.hpp"
#include <algorithm> // for std::reverse

// Bring in cJSON only here, to keep the header light.
#include "../cJSON/cJSON.h"

CleanupBucket::~CleanupBucket() {
    // Strongly exception-safe: never throw from a destructor.
    runAndClear();
}

CleanupBucket::CleanupBucket(CleanupBucket&& other) noexcept
: actions_(std::move(other.actions_)) {
    other.actions_.clear();
}

CleanupBucket& CleanupBucket::operator=(CleanupBucket&& other) noexcept {
    if (this != &other) {
        runAndClear();                 // clean current
        actions_ = std::move(other.actions_);
        other.actions_.clear();
    }
    return *this;
}

void CleanupBucket::addCallback(std::function<void()> fn) {
    if (fn) actions_.emplace_back(std::move(fn));
}

void CleanupBucket::addCJSON(cJSON* node) {
    if (!node) return;
    actions_.emplace_back([node]() noexcept { cJSON_Delete(node); });
}

void CleanupBucket::runAndClear() noexcept {
    // Run in reverse registration order (stack unwinding style).
    // Make a local copy to remain safe if callbacks add more callbacks.
    auto actions = std::move(actions_);
    actions_.clear();

    // reverse so last-added runs first
    std::reverse(actions.begin(), actions.end());

    for (auto& f : actions) {
        //try {
            if (f) f();
        //} catch (...) {
            // swallow to keep destructor noexcept
        //}
    }
}
