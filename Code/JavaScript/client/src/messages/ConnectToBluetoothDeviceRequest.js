export default class ConnectToBluetoothDeviceRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="ctbd";
    r["a"]=o.address;
    return r;
 }
}