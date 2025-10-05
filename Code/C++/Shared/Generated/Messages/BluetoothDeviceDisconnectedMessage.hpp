#ifndef BLUETOOTHDEVICEDISCONNECTEDMESSAGE_HPP
#define BLUETOOTHDEVICEDISCONNECTEDMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
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
        static std::shared_ptr<BluetoothDeviceDisconnectedMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //BLUETOOTHDEVICEDISCONNECTEDMESSAGE_HPP
