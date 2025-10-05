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
std::shared_ptr<BluetoothDeviceDisconnectedMessage> BluetoothDeviceDisconnectedMessage::fromJSON(cJSON* j){
    bool s = true;
    return std::make_shared<BluetoothDeviceDisconnectedMessage>();
}
BluetoothDeviceDisconnectedMessage::~BluetoothDeviceDisconnectedMessage(){
}
