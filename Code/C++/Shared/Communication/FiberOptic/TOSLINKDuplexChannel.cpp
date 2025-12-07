#include "TOSLINKDuplexChannel.hpp"

TOSLINKDuplexChannel::TOSLINKDuplexChannel(
	int txPin,
	int rxPin): RMTDuplexChannel(txPin, rxPin, 60, false, true) {
};