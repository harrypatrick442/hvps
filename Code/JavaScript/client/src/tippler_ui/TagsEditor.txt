import E from '../ui_core/E';
import i from '../ui_core/i';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import isNullOrUndefined from '../core/isNullOrUndefined';
import eventEnable from '../core/eventEnable';
import LoopBreak from '../core/LoopBreak';
import TagsEditorEntry from './TagsEditorEntry';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import OrderedItems from '../tippler_ui/OrderedItems';
import KeyCode from '../enums/KeyCode';
import _addElseRemoveClass from '../ui_core/_addElseRemoveClass';
import './TagsEditor.scss';
const  REG_EXP_ALLOWED_CHARACTERS= new RegExp('[0-9a-zA-Z]');
const ADD_TAG_KEY_CODES = [32,13,9,188,190];
export default function TagsEditor({model, propertyNameTags, restrictCharacters, maxTagLength, maxNTags}){
	if(isNullOrUndefined(restrictCharacters))restrictCharacters = true;
	if(isNullOrUndefined(maxTagLength))maxTagLength = 30;
	if(isNullOrUndefined(maxNTags))maxNTags = 30;
	const self = this;
	eventEnable(self);
	const disposes =[];
	const mapStrToEntry = new Map();
	const element = (self.element = E.div('tags-editor'));
	const entriesElement = E.div('entries');
	const inputElement = E.text('input', 'Type tags. use ",", space or enter to add...');
	const inputsElement = E.div('inputs');
	element.appendChild(inputsElement);
	element.appendChild(entriesElement);
	inputsElement.appendChild(inputElement);
	inputElement.maxLength = maxTagLength;
	const clearButton = _createImageHoverButton(
		i('Delete'), i('DeleteHover'), 'clear-button', ()=>self.clear(), disposes, true);
	inputsElement.appendChild(clearButton);
	
	
	
	const bindingTags = PropertyBindingFactory.standard(this, model, propertyNameTags, (value)=>{
		_addElseRemoveClass(value&&value.length>0, 'has-entries', element);
	});
	self.clear = ()=>{
		bindingTags.set(null);
		dispatchChanged();
	};
	let releaseCurrentHighlightDuplicate;
	const unhighlightDuplicate = ()=>{
		if(!releaseCurrentHighlightDuplicate)return;
		releaseCurrentHighlightDuplicate();
		releaseCurrentHighlightDuplicate = null;
	};
	const dispatchChanged=()=>{
		this.dispatchEvent({type:'changed'});
	};
	const addTag = (str)=>{
		let tags = bindingTags.get();
		if(tags){			
			if(tags.length>=maxTagLength)
				return false;
			if(tags.indexOf(str)>=0)
			{
				const entry = mapStrToEntry.get(str);
				unhighlightDuplicate();
				releaseCurrentHighlightDuplicate = entry.highlightDuplicate();
				return false;
			}
			tags.push(str);
		}
		else
		{
			tags = [str];
		}
		bindingTags.set(tags);
		dispatchChanged();
		return true;
	};
	const handleEntryRemove = (tag)=>{
		let tags = bindingTags.get();
		if(!tags)return;
		const oldLength = tags.length;
		tags = tags.filter(t=>t!==tag);
		if(tags.length===oldLength)return;
		bindingTags.set(tags);
		dispatchChanged();
	};
	const removeEndTag = ()=>{
		const tags = bindingTags.get();
		if(!tags)return;
		if(tags.length<1)return;
		tags.splice(tags.length - 1, 1);
		bindingTags.set(tags);
		dispatchChanged();
	};
	const entryDisposed = (tag)=>{
		mapStrToEntry.delete(tag.str);
	};
	inputElement.addEventListener('keydown', (e)=>{
		const {keyCode, key}=e;
		if(keyCode===KeyCode.ARROW_LEFT||keyCode===KeyCode.ARROW_RIGHT)
			return;
		const value = inputElement.value;
		if(keyCode===KeyCode.BACKSPACE){
			if(!value||value.length<1){
				removeEndTag();
				return;
			}
		}
		if(ADD_TAG_KEY_CODES.indexOf(keyCode)>=0){
			if(value&&value.length>0&&addTag(value)){
				inputElement.value = '';
			}
			return;
		}
		if(restrictCharacters&&!REG_EXP_ALLOWED_CHARACTERS.test(key))
		{
			e.preventDefault();
			return;
		}
		if(maxNTags&&bindingTags.get()?.length>maxNTags){
			e.preventDefault();
			return;
		}
		unhighlightDuplicate();
	});
	const orderedItems = new OrderedItems({
		element:entriesElement, 
		model, 
		propertyNameItems:propertyNameTags, 
		createView:(str)=>{
			const entry = new TagsEditorEntry({str, disposed:entryDisposed, remove:handleEntryRemove});
			mapStrToEntry.set(str, entry);
			return entry;
		},
		itemsArePrimitives:true
	});
	self.dispose = ()=>{
		disposes.forEach(d=>d());
		orderedItems.dispose();
		self.myBindings.dispose();
	};
}