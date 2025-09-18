export default class NativePickFileRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="npf";
    return r;
 }
}