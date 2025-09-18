export default function getConvertToType(valueType){
	switch(valueType){
		case 'string':
			return function(value){
				return String(value);
			};
		case 'number':
			return function(value){
				return Number(value);
			};
		case 'int':
			return function(value){
				return parseInt(value);
			};
		case 'float':
			return function(value){
				return parseFloat(value);
			};
		case 'boolean':
			return function(value){
				if(typeof(value)==='string')
				{
					value = value.toLowerCase();
					switch(value){
						case 'false':
						return false;
						case 'true':
						return true;
						default:
							return null;
					}
				}
				return Boolean(value);
			};
		/*case CustomConverterTypes.BooleanOrNull:
			return function(value){
				if(Validation[S.IS_NULL_OR_EMPTY](value))return null;
				if(typeof(value)==='string'){
					var lower = value.toLowerCase();
					if(lower=='false'||value=='0')
					{
						return false;
					}
				}
				return true;
			}*/
		default:
			return function(value){return value;};
			//throw new Error('Type '+valueType+' not recognised');
	}
}