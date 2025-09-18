import RunningMode from '../RunningMode';
import ConstantsGenerated from '../constants/Generated';
const DEBUG_WITH_LIVE_SERVER=false;
export default class Urls{
	static get backendPort(){
		const thisPagePort = window.location.port;
		return RunningMode.isDevelopment?5299:(thisPagePort===80?null:thisPagePort);
	}
	static _getWebSocketRootThisDomain(liveUrl){
		if(RunningMode.isDevelopment){
			if(DEBUG_WITH_LIVE_SERVER)	return liveUrl;
			else
			{
				if(window.location.hostname==='10.0.2.2')	
					return `ws://10.0.2.2:8080`;
				else
					return `ws://${window.location.hostname}:8080`;
			}
		}
		return liveUrl;
	}
}