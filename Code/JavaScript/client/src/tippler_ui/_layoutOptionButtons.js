import i from '../ui_core/i';
import E from '../ui_core/E';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import LayoutOptions from '../enums/LayoutOptions';
import comp from '../components/_layoutOptionButtons';
export default function _layoutOptionButtons(self, model, layoutOptions, propertyNameActive, disposes){
	let binding;
	const [element, setActive] = comp(layoutOptions, (value)=>binding.set(value), disposes);
	binding = PropertyBindingFactory.standard(self, model, propertyNameActive, setActive);
	disposes.push(binding.dispose);
	return element;
}