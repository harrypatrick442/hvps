export default class BluetoothDeviceDisconnectedMessage
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="bdd";
    r["a"]=o.address;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.address=o["a"];
   return r;
 }
}