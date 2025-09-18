import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import E from '../ui_core/E';
import DynamicIcon from '../ui_core/DynamicIcon';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
export default function Button({
			preventPropagation, className, classNames, classNameToggled,
			model, isToggle, toggled, disabled,
			img, imgHovering, imgDisabled, imgToggled, imgDisabledToggled, imgHoveringToggled, 
			propertyNameDisabled, propertyNameText, propertyNameVisible, propertyNameToggled, 
			click, methodNameClick, text
		}) {
        eventEnable(this);
        const self = this;
		var hovering = false;
        const element = E.button('button');
		var disposed=false;
		var imgElement, textElement, currentImg;
        if (className)
            element.classList.add(className);
        if (classNames)
            classNames.forEach((className)=>{
                element.classList.add(className);
            });
        element.classList.add('button');
        var getToggled;
		var setToggled;
		if(img){
			const imgWrapperElement = E.div('img-wrapper');
			imgElement = E.img();
			element.appendChild(imgWrapperElement);
			imgWrapperElement.appendChild(imgElement);
		}
		var propertyBindingToggled;
        var propertyBindingDisabled;
		var propertyBindingText;
		var propertyBindingVisible;
        if (propertyNameDisabled) {
            propertyBindingDisabled = PropertyBindingFactory.standard(
				this, model, propertyNameDisabled, disabledChanged);
        }
		if(propertyNameVisible){
			propertyBindingVisible = PropertyBindingFactory.standard(
				this, model, propertyNameVisible, visibleChanged);
		}
		if(propertyNameText||text){
			textElement = E.div('text');
			element.appendChild(textElement);
			if(propertyNameText){
				propertyBindingText = PropertyBindingFactory.standard(
					this, model, propertyNameText, textChanged);
			}
			if (text) textChanged(text);
		}
        element.addEventListener('click', handleClick);
        element.addEventListener('mouseenter', handleMouseEnter);
        element.addEventListener('mouseleave', handleMouseLeave);
        element.addEventListener('focus', handleMouseEnter);
        element.addEventListener('blur', handleMouseLeave);
		if(isToggle||propertyNameToggled){
			if(propertyNameToggled){
				propertyBindingToggled = PropertyBindingFactory.standard(this, model, propertyNameToggled, toggledChanged);
				getToggled = function(){return propertyBindingToggled.get();};
				setToggled = function(value){ propertyBindingToggled.set(value);};
			}
			else{
				if(isNullOrUndefined(toggled))
					toggled=false;
				getToggled = function(){return toggled;}
				setToggled=function(value){ 
					toggled = value;
					toggledChanged(value);
				};
				setToggled(toggled);
			}
		}
        this.getElement = ()=> element;
        this.setDisabled = (value) => {
            disabledChanged(value);
        };
		this.dispose=() => {			
			element.removeEventListener('click', handleClick);
			element.removeEventListener('mouseenter', handleMouseEnter);
			element.removeEventListener('mouseleave', handleMouseLeave);
			element.removeEventListener('focus', handleMouseEnter);
			element.removeEventListener('blur', handleMouseLeave);
			self.myBindings?.dispose();
		};
		updateImg();
        function handleClick(e) {
            e = e || window.event;
            if (preventPropagation) {
                e.stopPropagation();
            }
			e.preventDefault();
			if(methodNameClick){
				if(!model)throw new Error('No model supplied');
				const clickMethod = model[methodNameClick];
				if(!clickMethod)throw new Error(`Model ${model.constructor.name} has no ${methodNameClick} method`);
				clickMethod();
			}
			click&&click(e);
            dispatchClick();
            toggle();
        }
		function handleMouseEnter(){
			hovering = true;
			element.classList.add('hovering');
			updateImg();
		}
		function handleMouseLeave(){
			hovering = false;
			element.classList.remove('hovering');
			updateImg();
		}
        function toggle() {
            if (!isToggle) return;
			var toggled = !getToggled();
			setToggled(toggled);
			dispatchToggled(toggled);
        
		}
		function setImg(img){
			if(currentImg instanceof(DynamicIcon)){
				currentImg.removeEventListener('changed', currentImgChanged);
			}
			currentImg = img;
			if(img instanceof(DynamicIcon)){
				imgElement.src = img.src;
				currentImg.addEventListener('changed', currentImgChanged);
				return;
			}
			const type = typeof(img);
			if(type==='string'){
				imgElement.src = img;
				return;
			}
			throw new Error('Not supported for type '+type);
		}
		function currentImgChanged({src}){
			imgElement.src=src;
		}
		function updateImg(){
			if(disabled){
				if(toggled){
					if(imgDisabledToggled)
					{
						setImg(imgDisabledToggled);
						return;
					}
				}
				if(imgDisabled){
					setImg(imgDisabled);
					return;
				}
			}
			if(hovering){
				if(toggled){
					if(imgHoveringToggled)
					{
						setImg(imgHoveringToggled);
						return;
					}
				}
				if(imgHovering){
					setImg(imgHovering);
					return;
				}
			}
			else{
				if(toggled){
					if(imgToggled){
						setImg(imgToggled);
						return;
					}
				}
			}
			setImg(img);
		}
        function dispatchClick() {
            self.dispatchEvent({type:'click'});
        }
        function dispatchToggled(toggled) {
            self.dispatchEvent({type:'toggled', toggled});
        }
        function disabledChanged(value) {
			disabled = value;
			if(value)
				element.classList.add('disabled');
			else
				element.classList.remove('disabled');
            element.disabled = value ? true : false;
			updateImg();
        }
		function visibleChanged(value){
			if(value)element.classList.add('visible');
			else element.classList.remove('visible');
		}
		function textChanged(value){
			if(textElement)
				textElement.textContent=value;
		}
		function toggledChanged(value){
			toggled = value;
            if (value)
                element.classList.remove(classNameToggled);
			else
                element.classList.add(classNameToggled);
			updateImg();
		}
    };