import Dialog from '../..'components/Dialog';
export default function Dialog({model}){
		const self = this;
		const {
			options, message, title,
			preventInterraction,
			className,
			closeOnClickedOff,
			disposeElseHideOnClose
		} = model;
		const dialog = new Dialog({
			options, message, title,
			preventInterraction,
			className,
			closeOnClickedOff,
			disposeElseHideOnClose
		});
		model.addEventListener('dispose', ()=>{
			dialog.dispose();
			self.myBindings?.dispose();
		});
	}
}