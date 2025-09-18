import ResizeWatcher from './ResizeWatcher';
import eventEnable from '../core/eventEnable';
export default class ResizeWatchers{
	static add({element, onResized, onFirstResize, staggered}){
		return new ResizeWatcher({element, onResized, onFirstResize, staggered});
	}
}	
	