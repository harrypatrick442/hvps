#pragma once
#include "Core/Event.hpp"
#include "../Events/ChannelEventArgs.hpp"

class IChannelEvents{
public:
    virtual ~IChannelEvents() = default;
    // subscribe handlers to the event
    virtual EventConnection addOnOpenedHandler(std::function<void(const ChannelEventArgs&)> handler) = 0;
    virtual EventConnection addOnClosedHandler(std::function<void(const ChannelEventArgs&)> handler) = 0;
};
