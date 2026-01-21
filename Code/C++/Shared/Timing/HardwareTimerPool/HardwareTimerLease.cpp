#include "HardwareTimerLease.hpp"

HardwareTimerLease::HardwareTimerLease(
    timer_group_t group,
    timer_idx_t index,
    std::function<void()> releaseFunc
)
    : _group(group)
    , _index(index)
    , _released(false)
    , _releaseFunc(std::move(releaseFunc))
{
}

HardwareTimerLease::~HardwareTimerLease()
{
    if (_released) {
		return;
	}
	_releaseFunc();
	_released = true;
}

timer_group_t HardwareTimerLease::getGroup() const
{
    return _group;
}

timer_idx_t HardwareTimerLease::getIndex() const
{
    return _index;
}
