import Dialog from '../components/Dialog';
import Storage from '../core/Storage';
const dialog = new Dialog({
	preventInterraction:false,
	closeOnClickedOff:true,
	disposeElseHideOnClose:false
});
document.body.appendChild(dialog.element);
export default class Dialogs{
	static show({options, message, title, type, onClickClose, onClickOff}){
		dialog.show({
			options, message, title, type, onClickClose, onClickOff
		});
	}
}