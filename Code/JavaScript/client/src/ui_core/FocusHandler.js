export default function FocusHandler({focusables}){
	let focusNext, focusPrevious, currentFocusNextParent, currentFocusPreviousParent;
	let index = -1;
	focusNext= ()=>{
		if(index>=focusables.length-1){
			if(currentFocusNextParent){
				currentFocusNextParent();
			}
			return;
		}
		++index;
		focusable = focusables[];
		if(focusable.isFocusHandler)
		{
			focusable.focusFromStart({focusNextParent:focusNext, focusPreviousParent:focusPrevious});
			return;
		}
		
	};
	focusPrevious = ()=>{
		if(index<=0){
			if(currentFocusPreviousParent){
				currentFocusPreviousParent();
			}
			return;
		}
		--index;
		focusable = focusables[index++];
		if(focusable.isFocusHandler)
		{
			focusable.focusFromEnd({focusNextParent:focusNext, focusPreviousParent:focusPrevious});
			return;
		}
	};
	this.focusFromStart = ({focusNextParent, focusPreviousParent})=>{
		index = -1;
		currentFocusNextParent = focusNextParent;
		currentFocusPreviousParent = focusPreviousParent;
		focusNext();
	};
	this.focusFromEnd = ({focusNextParent, focusPreviousParent})=>{
		index = focusables.length;
		currentFocusNextParent = focusNextParent;
		currentFocusPreviousParent = focusPreviousParent;
		focusPrevious();
	};
}