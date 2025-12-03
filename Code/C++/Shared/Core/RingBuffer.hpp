#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class RingBuffer {
public:
    RingBuffer(size_t size);               // allocates via heap_caps_malloc
    ~RingBuffer();

    size_t push(const uint8_t* data, size_t length); // overwrite oldest on full
    size_t take(uint8_t* dest, size_t maxNBytes);    // remove up to maxNBytes
    size_t peek(uint8_t* dest, size_t maxNBytes) const;

    size_t size() const;        // bytes stored
    size_t capacity() const;    // usable capacity = _size - 1
    void clear();

    bool overflowed() const { return _overflowFlag; }
    void clearOverflowFlag() { _overflowFlag = false; }

private:
    uint8_t* _buffer;
    size_t _size;
    size_t _head;
    size_t _tail;

    mutable SemaphoreHandle_t _lock;
    bool _overflowFlag = false;

    bool isEmpty() const;
    size_t safeSize() const; // internal size() without locking
};