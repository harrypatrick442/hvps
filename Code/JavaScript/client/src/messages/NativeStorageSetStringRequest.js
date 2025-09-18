export default class NativeStorageSetStringRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="nsss";
    r["k"]=o.key;
    r["v"]=o.value;
    return r;
 }
}