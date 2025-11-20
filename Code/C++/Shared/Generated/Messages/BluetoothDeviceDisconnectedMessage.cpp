#include "./BluetoothDeviceDisconnectedMessage.hpp"
const char* BluetoothDeviceDisconnectedMessage::TYPE = "bdd";
BluetoothDeviceDisconnectedMessage::BluetoothDeviceDisconnectedMessage(
)
{
}
cJSON* BluetoothDeviceDisconnectedMessage::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
BluetoothDeviceDisconnectedMessage* BluetoothDeviceDisconnectedMessage::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    auto r = new BluetoothDeviceDisconnectedMessage();
    cleanupBucket.addDelete(r);
    return r;
}
BluetoothDeviceDisconnectedMessage::~BluetoothDeviceDisconnectedMessage(){
}
