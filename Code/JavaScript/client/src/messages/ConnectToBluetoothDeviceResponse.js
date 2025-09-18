export default class ConnectToBluetoothDeviceResponse
{
 static fromJSON(o){
    const r = {};
    r.failedReason=o["s"];
    r.address=o["a"];
   return r;
 }
}