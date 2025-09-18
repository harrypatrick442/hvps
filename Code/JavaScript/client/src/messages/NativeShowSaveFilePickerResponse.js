export default class NativeShowSaveFilePickerResponse
{
 static fromJSON(o){
    const r = {};
    r.success=o["s"];
    r.fileIdentifier=o["fi"];
    r.fileName=o["fn"];
    r.fileType=o["ft"];
   return r;
 }
}