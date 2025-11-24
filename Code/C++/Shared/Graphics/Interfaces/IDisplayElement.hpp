#pragma once
class IDisplayElement {
public:
    virtual ~IDisplayElement() = default;

	virtual	void getPixels(size_t& length, volatile uint32_t** pixels) = 0;
};