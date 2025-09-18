import enableAcceptBindings from './enableAcceptBindings';
import enableHash from '../core/enableHash';
import isNullOrUndefined from '../core/isNullOrUndefined';
import capitalizeFirstLetter from '../core/capitalizeFirstLetter';
import PropertyBinding from './PropertyBinding';
import PropertyHelper from './PropertyHelper';
import PropertyBindingCarriedOver from './PropertyBindingCarriedOver';
import PropertyBindingShell from './PropertyBindingShell';
export default class PropertyBindingFactory{
	
	//meOrParams, model, nameParam, onChange, converter, viewModelName, onSetByView
	/*params {
		S.ME
		S.MODEL
		S.NAME or S_NAMES
		S.VIEW_MODEL_NAME (optional)
		S.ON_CHANGE(optional) Not intended if more than one name supplied.
		S.CONVERTER (optional)
		S.ON_SET_BY_VIEW (optional)
	}*/
	static carryOver({me, model, name, viewModelName, onChange, converter, onSetByView}){
		if(!viewModelName){
			viewModelName = name;
		}
		return /*new*/ PropertyBindingFactory._carryOver(me, model, name, onChange, converter, onSetByView, viewModelName);
	}
	
	
	static carryOverModelAsViewModel(me, model, name, a,b,c){
		PropertyBindingFactory.carryOverChildModelAsChildViewModel(me, model, name, a,b,c);
	}
	//me, model, name, viewModelClass, onViewModelSet
	//me, model, name, viewModelName, viewModelClass, onViewModelSet
	static carryOverChildModelAsChildViewModel(me, model/*the model being carried over is a property of this model*/, name, a,b,c){
		var createViewModel, onViewModelSet,viewModelName;
		if(typeof(a)==='string')
		{
			viewModelName=a;
			createViewModel = b;
			onViewModelSet = c;
		}else{
			viewModelName=name;
			createViewModel=a;
			onViewModelSet=b;
		}		
		var bindingsHandler = enableAcceptBindings(me);
		var viewModel;
		var currentModel;
		const _change=(value, oldValue)=>{
			bindingsHandler.changed(viewModelName, value);
			onViewModelSet&&onViewModelSet(value, oldValue);
		};
		const change=(value)=>{
			var oldViewModel = viewModel;
			if(!value)
			{
				if(!currentModel)return;
				viewModel = null;
				_change(viewModel, oldViewModel);
				currentModel= null;
				return;
			}
			if(value==currentModel)
			{
				return;
			}
			currentModel = value;
			viewModel = createViewModel(value);
			_change(viewModel, oldViewModel);
		};
		var capitalizedName = capitalizeFirstLetter(viewModelName);
		var getterName = PropertyHelper.getGetterName(capitalizedName);
		//if(!model[getterName])throw new Error('Model of type '+model.constructor.name+' does not contain a getter with name '+getterName);
		me[getterName]=function(){return viewModel;};
		var propertyBinding = new PropertyBinding(me, model, name, change);
		change(propertyBinding.get());
		return propertyBinding;
	}
	static carryOverViewModelAsView(me, viewModel, name, createView, onViewSet){
		PropertyBindingFactory.carryOverChildViewModelAsChildView(me, viewModel, name,
			createView, onViewSet);
	}
	static carryOverChildViewModelAsChildView(me, viewModel,// this is actually very similar to the above but for now keep as seperate to avoid confusion.
		name, createView, onViewSet){
		var bindingsHandler = enableAcceptBindings(me);
		var view;
		var currentViewModel;
		function change(value){
			if(!value)
			{
				if(!currentViewModel)return;
				var oldView = view;
				disposeCurrentView();
				_change(null, oldView);
				currentViewModel= null;
				return;
			}
			if(value==currentViewModel)
			{
				return;
			}
			var oldView = view;
			disposeCurrentView();
			currentViewModel = value;
			view = createView(value);
			_change(view, oldView);
		};
		function disposeCurrentView(){
			if(!view)return;
			view.dispose();//force implementing dispose for good practise so dont forget cleanup.
			view = null;
		}
		function _change(view, oldView){
			bindingsHandler.changed(name, view);
			onViewSet&&onViewSet(view, oldView);
		}
		var capitalizedName = capitalizeFirstLetter(name);
		var getterName = PropertyHelper.getGetterName(capitalizedName);
		var getter = viewModel[getterName];
		if(!getter)throw new Error('Model of type '+viewModel.constructor.name+' does not contain a get method for property named '+name);
		change(getter());
		me[getterName]=function(){return view;};
		var propertyBinding = new PropertyBinding(me, viewModel, name, change);
		return propertyBinding;
	}
	static arrayConversion(me, model, name, createConvertedItem, removedItem, changedCallback, postpone/* called any time the child changes*/){
		return PropertyBindingFactory._arrayConversion(me, model, name, createConvertedItem, removedItem, changedCallback, postpone, function(item){
			return enableHash(item);
		});
	}
	static arrayConversionFromPrimitives(me, model, name, createConvertedItem, removedItem, changedCallback/* called any time the child changes*/, postpone){
		return PropertyBindingFactory._arrayConversion(me, model, name, createConvertedItem, removedItem, changedCallback, postpone, function(item){
			return String(item);
		});
	}
	static _arrayConversion(me, model, name, createConvertedItem, removedItem, changedCallback/* called any time the child changes*/, postpone, getHashFromItem){
		var mapHashToConvertedItem={};
		var currentConvertedItems=[];
		var propertyBinding = PropertyBindingFactory.standard(me, model, name, changed, true);
		function changed(items){
			var hashesSeen = [];
			for(var i=0; i<items.length; i++){
				var item = items[i];
				var hash = getHashFromItem(item);
				hashesSeen.push(hash);
				var convertedItem = mapHashToConvertedItem[hash];
				if(!convertedItem)
				{
					convertedItem = createConvertedItem(item);
					mapHashToConvertedItem[hash]=convertedItem;
					currentConvertedItems.splice(i, 0, convertedItem);
				}
				else//reposition
				currentConvertedItems.splice(i, 0, currentConvertedItems.splice(currentConvertedItems.indexOf(convertedItem), 1)[0]);
			}
			for(var hash in mapHashToConvertedItem){
				if(hashesSeen.indexOf(hash)<0)
				{
					var convertedItem = mapHashToConvertedItem[hash];
					delete mapHashToConvertedItem[hash];
					currentConvertedItems.splice(currentConvertedItems.indexOf(convertedItem), 1);
					removedItem&&removedItem(convertedItem);
				}
			}
			changedCallback&&changedCallback(currentConvertedItems);
		}
		function getMethod (){return currentConvertedItems;}
		function setMethod (){throw new Error('Cannot set a converted array. Not currently supported. Try setting the array being converted.');}
		var items = propertyBinding.get();
		if(!items)throw new Error( name +' had no get method on object of type '+model.constructor.name);
		if(!postpone)
			changed(items);
		return new PropertyBindingCarriedOver(propertyBinding, new PropertyBindingShell(getMethod, setMethod));
	}
	//me, model, name, createViewModel
    //me, model, name, viewModelsName, createViewModel
	static carryOverModelArrayAsViewModelArray(me, model, name, a, b, c){
        var createViewModel, viewModelsName, callbackChanged;
        if (typeof (a) === 'string') {
            viewModelsName = a;
            createViewModel = b;
			callbackChanged=c;
        } else {
            viewModelsName = name;
            createViewModel = a;
			callbackChanged=b;
        }
		var bindingsHandler = enableAcceptBindings(me);
        var capitalizedName = capitalizeFirstLetter(viewModelsName);
        var getterName = PropertyHelper.getGetterName(capitalizedName);
		var propertyBinding;
        me[getterName] = function () { return propertyBinding.getCarriedOver().get(); };
        function _change(value) {
            bindingsHandler.changed(viewModelsName, value);
			callbackChanged&&callbackChanged(value);
        }
		function removed(viewModel){
			viewModel.disposed();
		}
		propertyBinding = PropertyBindingFactory.arrayConversion(me, model, name, createViewModel, removed, _change);
		return propertyBinding;
	}	
	
	static standard(me, model, name, valueChanged, postpone){
		var propertyBinding = new PropertyBinding(me, model, name, valueChanged);
		if(!postpone)
			valueChanged(propertyBinding.get());
		return propertyBinding;
	}
	static _carryOver(me, model, name, onChange, converter, onSetByView, viewModelName){
		var bindingsHandler = enableAcceptBindings(me);
		let change;
		const capitalizedName = capitalizeFirstLetter(name);
		const setterName = PropertyHelper.getSetterName(capitalizedName);
		const getterName = PropertyHelper.getGetterName(capitalizedName);
		let setterNameViewModel, getterNameViewModel;
		if(viewModelName){
			 const capitalizedNameViewModel = capitalizeFirstLetter(viewModelName);
			 setterNameViewModel = PropertyHelper.getSetterName(capitalizedNameViewModel);
			 getterNameViewModel = PropertyHelper.getGetterName(capitalizedNameViewModel);
		}
		else{
			 setterNameViewModel = setterName;
			 getterNameViewModel = getterName;
			 viewModelName = name;
		}
		if(model[getterName]===undefined)
			throw new Error(getterName+' is not defined on model type '+model.constructor.name);
		/*if(model[setterName]===undefined)
			throw new Error(setterName+' is not defined on model type '+model.constructor.name);*/
		var s;
		var changed = bindingsHandler.changed;
		if(converter){
			change = 
				onChange
				?function(value){
					var convertedValue=converter.from(value);
					changed(viewModelName, convertedValue);
					onChange(convertedValue);
				}
				:function(value){
					changed(viewModelName, converter.from(value));
				};
			me[getterNameViewModel]=function(){
					return converter.from(model[getterName]());
				};
				
			s  =function(value){ model[setterName](converter.to(value));};
		}
		else{
			change = 
			onChange
			?function(value){
				changed(viewModelName, value);
				onChange(value);
			}
			:function(value){
				changed(viewModelName, value);
			};
			me[getterNameViewModel]=function(){
				return model[getterName]();
			};
			s = function(value){ model[setterName](value);};
		}
		if(onSetByView)
			s = (function(main){ return function(value){main(value);onSetByView(value);};})(s);
		me[setterNameViewModel]=s;
		return new PropertyBinding(me, model, name, change);
	}
	static lazyLoadViewForSingletonModel(me, ModelClass, ViewClass, parentElement){
		let existingModel;
		let view;
		PropertyBindingFactory.standard(me, ModelClass, 'instance', (value)=>{
			if(isNullOrUndefined(value)){
				if(view){
					if(view.dispose){
						view.dispose();
					}
					parentElement?.removeChild(view.element);
				}
				view = null;
				existingModel= null;
				return;
			}
			if(!isNullOrUndefined(existingModel)){
				if(existingModel===value)
					return;
				if(view.dispose){
					view.dispose();
				}
				parentElement?.removeChild(view.element);
			}
			view = new ViewClass({model:value});
			ViewClass.instance = view;
			existingModel = value;
			parentElement?.appendChild(view.element);
		});
	};
}