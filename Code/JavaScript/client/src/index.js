import './index.css';
import HVPSUIView from './views/hvpsui/HVPSUI';
import HVPSUIViewModel from './view_models/hvpsui/HVPSUIViewModel';
import HVPSUIIcons from './themes/HVPSUIIcons';
import Icons from './icons/Icons';
import NativeAPI from './api/NativeAPI';
import './HVPSUI.css';
class HVPSUI{
	constructor(){
		Icons.update(HVPSUIIcons);
		this._hvpsuiViewModel = new HVPSUIViewModel();
		this._hvpsuiView = new HVPSUIView({model:this._hvpsuiViewModel});
	}
};
window._root = new HVPSUI();