import BluetoothDevice from './BluetoothDevice';
export default class GetAvailableBluetoothDevicesResponse
{
 static fromJSON(o){
    const r = {};
    const n = (v)=>v!==undefined&&v!==null;
    let v;
    v = o["d"];
    if(n(v))
        r.devices=v.filter(n).map(a=>BluetoothDevice.fromJSON(a));
    r.failedReason=o["s"];
   return r;
 }
}