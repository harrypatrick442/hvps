export default class GetAvailableBluetoothDevicesResponse
{
 static fromJSON(o){
    const r = {};
    r.devices=o["d"];
    r.failedReason=o["s"];
   return r;
 }
}