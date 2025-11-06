#pragma once
#include "../interfaces/IChannelEvents.hpp"
#include "Core/Event.hpp"
#include "ChannelEventArgs.hpp"
#include "Tasks/TransientWorker.hpp"
class ChannelEvents: public virtual IChannelEvents{
	private:
		std::shared_ptr<TransientWorker> _worker{ std::make_shared<TransientWorker>(32, 1000, true) };
		Event<ChannelEventArgs> onOpened;
		Event<ChannelEventArgs> onClosed;
	public:
		EventConnection addOnOpenedHandler(std::function<void(const ChannelEventArgs&)> handler) override;
		EventConnection addOnClosedHandler(std::function<void(const ChannelEventArgs&)> handler) override;
	
	protected:
		void dispatchOnOpened();
		void dispatchOnClosed();
};