export default class NativeStorageGetStringResponse
{
 static fromJSON(o){
    const r = {};
    r.value=o["v"];
   return r;
 }
}