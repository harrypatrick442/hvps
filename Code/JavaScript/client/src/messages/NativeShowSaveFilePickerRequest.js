import FileInfo from './FileInfo';
export default class NativeShowSaveFilePickerRequest
{
 static toJSON(o){ 
    const r = {};
   r["tpe"]="nsaf";
    const n = (v)=>v!==undefined&&v!==null;
    if(n(o.fileInfo))
        r["f"]=FileInfo.toJSON(o.fileInfo);
    return r;
 }
}