export default class BluetoothDevice
{
 static toJSON(o){ 
    const r = {};
    r["n"]=o.name;
    r["a"]=o.address;
    return r;
 }
 static fromJSON(o){
    const r = {};
    r.name=o["n"];
    r.address=o["a"];
   return r;
 }
}