#include "RingBuffer.hpp"
#include <algorithm>     // <-- required for std::min

RingBuffer::RingBuffer(size_t size)
    : _size(size), _head(0), _tail(0) {
    if (_size < 2) _size = 2; // ensure validity

    // Allocate in uncacheable memory for ISR-safe DMA-like usage
    _buffer = (uint8_t*) heap_caps_malloc(_size, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);

    _lock = xSemaphoreCreateMutex();
}

RingBuffer::~RingBuffer() {
    if (_buffer) free(_buffer);
    if (_lock) vSemaphoreDelete(_lock);
}

bool RingBuffer::isEmpty() const {
    return _head == _tail;
}

size_t RingBuffer::safeSize() const {
    return (_head + _size - _tail) % _size;
}

size_t RingBuffer::size() const {
    //xSemaphoreTake(_lock, portMAX_DELAY);
    size_t s = safeSize();
    //xSemaphoreGive(_lock);
    return s;
}

size_t RingBuffer::capacity() const {
    return _size - 1;
}

void RingBuffer::clear() {
    //xSemaphoreTake(_lock, portMAX_DELAY);
    _head = _tail = 0;
    _overflowFlag = false;
    //xSemaphoreGive(_lock);
}

size_t RingBuffer::push(const uint8_t* data, size_t length) {
    //xSemaphoreTake(_lock, portMAX_DELAY);

    for (size_t i = 0; i < length; i++) {
        _buffer[_head] = data[i];
        _head = (_head + 1) % _size;
        if (_head == _tail) {
            _tail = (_tail + 1) % _size;
            _overflowFlag = true;
        }
    }

    //xSemaphoreGive(_lock);
    return length;
}

size_t RingBuffer::take(uint8_t* dest, size_t maxNBytes) {
    //xSemaphoreTake(_lock, portMAX_DELAY);

    size_t bytesAvailable = safeSize();
    size_t toCopy = (maxNBytes < bytesAvailable) ? maxNBytes : bytesAvailable;

    size_t firstChunk = std::min(toCopy, _size - _tail);
    memcpy(dest, &_buffer[_tail], firstChunk);

    size_t secondChunk = toCopy - firstChunk;
    if (secondChunk > 0) {
        memcpy(dest + firstChunk, &_buffer[0], secondChunk);
    }

    _tail = (_tail + toCopy) % _size;

    //xSemaphoreGive(_lock);
    return toCopy;
}

size_t RingBuffer::peek(uint8_t* dest, size_t maxNBytes) const {
    //xSemaphoreTake(_lock, portMAX_DELAY);

    size_t bytesAvailable = safeSize();
    size_t toCopy = (maxNBytes < bytesAvailable) ? maxNBytes : bytesAvailable;

    size_t tempTail = _tail;

    size_t firstChunk = std::min(toCopy, _size - tempTail);
    memcpy(dest, &_buffer[tempTail], firstChunk);

    size_t secondChunk = toCopy - firstChunk;
    if (secondChunk > 0) {
        memcpy(dest + firstChunk, &_buffer[0], secondChunk);
    }

    //xSemaphoreGive(_lock);
    return toCopy;
}
