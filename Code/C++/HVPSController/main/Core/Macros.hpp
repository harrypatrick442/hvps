#pragma once

// Delete copy & move ctor/assign for a class
#define DISALLOW_COPY_MOVE(Class)        \
    Class(const Class&) = delete;         \
    Class& operator=(const Class&) = delete; \
    Class(Class&&) = delete;              \
    Class& operator=(Class&&) = delete
#define DISALLOW_COPY(Class)             \
    Class(const Class&) = delete;         \
    Class& operator=(const Class&) = delete

#define DISALLOW_MOVE(Class)             \
    Class(Class&&) = delete;              \
    Class& operator=(Class&&) = delete