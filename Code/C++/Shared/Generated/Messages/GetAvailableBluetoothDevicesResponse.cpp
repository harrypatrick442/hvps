#include "./GetAvailableBluetoothDevicesResponse.hpp"
const char* GetAvailableBluetoothDevicesResponse::TYPE = "tkd";
GetAvailableBluetoothDevicesResponse::GetAvailableBluetoothDevicesResponse(
    std::optional<int32_t> failedReason, 
    uint64_t ticket):
        _failedReason(failedReason),
        _ticket(ticket){
}
std::optional<int32_t> GetAvailableBluetoothDevicesResponse::getFailedReason(){
    return this->_failedReason;
}
uint64_t GetAvailableBluetoothDevicesResponse::getTicket(){
    return this->_ticket;
}
cJSON* GetAvailableBluetoothDevicesResponse::toJSON(){
    cJSON *j = cJSON_CreateObject();
    JHelper::addNullableInt32(j, "s", this->_failedReason);
    JHelper::addUInt64(j, "tckt", this->_ticket);
    JHelper::addString(j, "tpe", TYPE);
    return j;
}
std::shared_ptr<GetAvailableBluetoothDevicesResponse> GetAvailableBluetoothDevicesResponse::fromJSON(cJSON* j){
    bool s = true;
    std::optional<int32_t> failedReason = JHelper::getNullableInt32(j, "s", s);
    uint64_t ticket = JHelper::getUInt64(j, "tckt", s);
    return std::make_shared<GetAvailableBluetoothDevicesResponse>(failedReason, ticket);
}
GetAvailableBluetoothDevicesResponse::~GetAvailableBluetoothDevicesResponse(){
}
