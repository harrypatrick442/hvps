#include "./GetAvailableBluetoothDevicesResponse.hpp"
const char* GetAvailableBluetoothDevicesResponse::TYPE = "tkd";
GetAvailableBluetoothDevicesResponse::GetAvailableBluetoothDevicesResponse(
    , 
    std::optional<int32_t> failedReason, 
    uint64_t ticket):
        _failedReason(failedReason),
        _ticket(ticket){
}
std::optional<int32_t> GetAvailableBluetoothDevicesResponse::getFailedReason()const noexcept{
    return this->_failedReason;
}
uint64_t GetAvailableBluetoothDevicesResponse::getTicket()const noexcept{
    return this->_ticket;
}
cJSON* GetAvailableBluetoothDevicesResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableInt32(j, "s", this->_failedReason);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
GetAvailableBluetoothDevicesResponse* GetAvailableBluetoothDevicesResponse::fromJSON(cJSON* j, CleanupBucket& cleanupBucket){
    bool s = true;
    std::optional<int32_t> failedReason = JHelper::getNullableInt32(j, "s", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    auto r = new GetAvailableBluetoothDevicesResponse(devices, failedReason, ticket);
    cleanupBucket.addDelete(r);
    return r;
}
GetAvailableBluetoothDevicesResponse::~GetAvailableBluetoothDevicesResponse(){
}
