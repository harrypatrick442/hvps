import E from '../ui_core/E';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import _addElseRemoveClass from '../ui_core/_addElseRemoveClass';
import './NoEntries.css';
export default function createNoEntries(params){
	params = params||{};
	const {me, relative, text, propertyNameHasEntries, propertyNameLoadedSuccessfully, model} = params;
	if(!me)throw new Error('me');
	if(!propertyNameHasEntries)throw new Error('propertyNameHasEntries');
	if(!model)throw new Error('model');
	const noEntriesElement = E.div('no-entries');
	if(relative)
		noEntriesElement.classList.add('relative');
	noEntriesElement.textContent = text?text:'None';
	PropertyBindingFactory.standard(me, model, propertyNameHasEntries, (hasEntries)=>{
		_addElseRemoveClass(!hasEntries, 'has-no-entries', me.element);
	});
	if(propertyNameLoadedSuccessfully){
		noEntriesElement.classList.add('only-on-loaded-successfully');
		PropertyBindingFactory.standard(me, model, propertyNameLoadedSuccessfully, (loadedSuccessfully)=>{
			_addElseRemoveClass(loadedSuccessfully, 'loaded-successfully', noEntriesElement);
		});
	}
	return noEntriesElement;
};