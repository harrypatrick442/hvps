#include "./ConnectToBluetoothDeviceResponse.hpp"
const char* ConnectToBluetoothDeviceResponse::TYPE = "tkd";
ConnectToBluetoothDeviceResponse::ConnectToBluetoothDeviceResponse(
    const char* address, 
    std::optional<int32_t> failedReason, 
    uint64_t ticket):
        _address(address),
        _failedReason(failedReason),
        _ticket(ticket){
}
const char* ConnectToBluetoothDeviceResponse::getAddress()const noexcept{
    return this->_address;
}
std::optional<int32_t> ConnectToBluetoothDeviceResponse::getFailedReason()const noexcept{
    return this->_failedReason;
}
uint64_t ConnectToBluetoothDeviceResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* ConnectToBluetoothDeviceResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addString(j, "a", this->_address);
    JHelper::addNullableInt32(j, "s", this->_failedReason);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
ConnectToBluetoothDeviceResponse* ConnectToBluetoothDeviceResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    const char* address = JHelper::getString(j, "a", s);
    cleanupBucket.addDeleteArray(address);
    std::optional<int32_t> failedReason = JHelper::getNullableInt32(j, "s", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new ConnectToBluetoothDeviceResponse(address, failedReason, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
ConnectToBluetoothDeviceResponse::~ConnectToBluetoothDeviceResponse(){
}
