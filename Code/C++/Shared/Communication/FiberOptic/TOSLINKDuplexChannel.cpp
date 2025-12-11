#include "TOSLINKDuplexChannel.hpp"

TOSLINKDuplexChannel::TOSLINKDuplexChannel(
	int txPin,
	int rxPin): MRTDuplexChannel(txPin, rxPin, 600, false, true) {
};