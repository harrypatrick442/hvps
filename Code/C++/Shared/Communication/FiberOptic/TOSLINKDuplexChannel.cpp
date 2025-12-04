#include "TOSLINKDuplexChannel.hpp"

TOSLINKDuplexChannel::TOSLINKDuplexChannel(
	int txChannel,
	int rxChannel,
	int txPin,
	int rxPin): RMTDuplexChannel(txChannel, rxChannel, txPin, rxPin, false, true) {
};