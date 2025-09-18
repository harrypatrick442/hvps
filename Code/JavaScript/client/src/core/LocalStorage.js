function LocalStorage(settingsName, callbackReset)
{
    this.get = function (name)
    {
        try
        {
        //return JSON.parse(getCookie(settingsName + '_' + name));
        return JSON.parse(localStorage.getItem(settingsName + '_' + name));
		}
		catch(ex)
		{
			return undefined;
		}
    };
    this.set = function (name, obj)
    {
        try
        {
        //setCookie(settingsName + '_' + name, JSON.stringify(obj));
        localStorage.setItem(settingsName + '_' + name, JSON.stringify(obj));
    }
   catch(ex)
   {
       console.error(ex);
   }
    };
    this.reset=callbackReset;
    LocalStorage.instances.push(this);
}
LocalStorage.getAll=function(){
    return localStorage;
};
LocalStorage.addRange=function(obj)
{
    for(var key in obj)
    {
        localStorage.setItem(key, JSON.stringify(obj[key]));
    }
};
LocalStorage.instances=[];
LocalStorage.resetAll = function ()
{
    
};
export default LocalStorage;