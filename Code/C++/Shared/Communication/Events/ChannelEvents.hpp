#pragma once
#include "../interfaces/IChannelEvents.hpp"
#include "Core/Event.hpp"
#include "ChannelEventArgs.hpp"
class ChannelEvents: public IChannelEvents{
	private:
		std::shared_ptr<TransientWorker> _worker{ std::make_shared<TransientWorker>(1000) };
		Event<ChannelEventArgs> onOpened;
		Event<ChannelEventArgs> onClosed;
	public:
		EventConnection addOnOpenedHandler(std::function<void(const ChannelEventArgs&)> handler) override;
		EventConnection addOnClosedHandler(std::function<void(const ChannelEventArgs&)> handler) override;
	
	protected:
		void dispatchOnOpened();
		void dispatchOnClosed();
};