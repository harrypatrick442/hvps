import Dialog from '../..'components/Dialog';
export default class Dialogs{
	constructor({model}){
		model.addEventListener('showNew', (model)=>{
			const dialog = new Dialog({model});
			document.body.appendChild(dialog.element);
		});
	}
}