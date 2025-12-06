HardwareRMTReader::HardwareRMTReader(size_t bufferSize){
	_ringBufferHandle = xRingbufferCreate(STAGING_BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);
	if (_ringBufferHandle == nullptr) {
		Aborter::safeAbort(TAG, "Failed to create staging byte buffer");
		return false;
	}
	
}
size_t HardwareRMTReader::readBytes(char* destination, size_t maxLength, uint32_t timeoutMs) {
	
	
	if(hasBufferedItems){
		readBytesFromBufferedItems();
	}
    if (_rb == nullptr) return 0;

	uint32_t  duration;
	size_t nextDestinationIndex = 0;
	rmt_item32_t* item = nullptr;
    while (true) {
		size_t maxNItemsRetrieve = maxLength-nextDestinationIndex;
		if(maxNItemsRetrieve<=0){
			break;
		}
		if(maxNItemsRetrieve>MAX_N_ITEM_RETRIEVE_FROM_RING_BUFFER_AT_ONCE){
			maxNItemsRetrieve = MAX_N_ITEM_RETRIEVE_FROM_RING_BUFFER_AT_ONCE;
		}
        size_t outSize = 0;
        auto* items = (rmt_item32_t*)xRingbufferReceive(
            _rb,
            &outSize,
            pdMS_TO_TICKS(timeoutMs),
        );
			//sizeof(rmt_item32_t)*maxNItemsRetrieve
        if (!items) break;
        size_t nItems = outSize / sizeof(rmt_item32_t);
		size_t nextItemIndex = 0;
		while(nextItemIndex<nItems){
			item = &items[nextItemIndex++];
			duration = item->level0>0?item->duration0:item->duration1;
			if(duration<=_syncPulseMaxUs){
				if(_nextNBit>0){
					if(_nextNBit!=8){
						handleMalformedByte(_nextNBit);
					}
					destination[nextDestinationIndex++]=_currentByte;
					if(nextDestinationIndex>=maxLength){
						break;
					}
					_nextNBit = 0;
				}
				_currentByte = 0;
				continue;
			}
			if(_nextNBit<8){
				uint8_t bit = ((duration>_shortPulseMaxUs)^_invertRx)?1:0;
				_currentByte = (_currentByte << 1) | bit;
			}
            _nextNBit++;
        }
        vRingbufferReturnItem(_rb, (void*)items);
    }

    return nextDestinationIndex;
}