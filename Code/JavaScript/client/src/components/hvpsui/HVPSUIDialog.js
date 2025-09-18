import Dialog from '../Dialog';
import Storage from '../../core/Storage';
import './HVPSUIDialog.scss';
const dialog = new Dialog({
	preventInterraction:false,
	className:'hvpsui-dialog',
	closeOnClickedOff:true,
	disposeElseHideOnClose:false
});
export default class HVPSUIDialog{
	static show({options, message, title, showDoNotShowAgain}){
		HVPSUIDialog._dialog.show({options, message, 
			title, onClickClose:HVPSUIDialog._dialog.hide});
	}
}
document.body.appendChild(dialog.element);
HVPSUIDialog._dialog = dialog;