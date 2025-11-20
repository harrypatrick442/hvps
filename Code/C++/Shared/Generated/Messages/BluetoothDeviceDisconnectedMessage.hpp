#ifndef BLUETOOTHDEVICEDISCONNECTEDMESSAGE_HPP
#define BLUETOOTHDEVICEDISCONNECTEDMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class BluetoothDeviceDisconnectedMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        BluetoothDeviceDisconnectedMessage(
) noexcept;
        ~BluetoothDeviceDisconnectedMessage();
        static BluetoothDeviceDisconnectedMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //BLUETOOTHDEVICEDISCONNECTEDMESSAGE_HPP
