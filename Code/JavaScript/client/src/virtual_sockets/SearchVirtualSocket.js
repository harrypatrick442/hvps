import eventEnable from '../core/eventEnable';
import VirtualSocketMessage from '../messages/VirtualSocketMessage';
import SnippetsWebServerTypes from '../messages/SnippetsWebServerTypes';
import FoundSnippetMessage from '../messages/FoundSnippetMessage';
import LoadSnippetRequest from '../messages/LoadSnippetRequest';
import SnippetToLoadMessage from '../messages/SnippetToLoadMessage';
import isNullOrUndefined from '../core/isNullOrUndefined';
export default class SearchVirtualSocket
{	
	constructor({ virtualSocket}){
		eventEnable(this);
		this._handleMessage = this._handleMessage.bind(this);
		this.continueSearch = this.continueSearch.bind(this);
		this.dispose = this.dispose.bind(this);
		this.loadSnippet = this.loadSnippet.bind(this);
		this._virtualSocket = virtualSocket;
		this._virtualSocket.addEventListener('message', this._handleMessage);
	}
	_handleMessage({payload, internalType}){
		switch(internalType){
			case SnippetsWebServerTypes.done:
				this.dispatchEvent({type:'done'});
				break;
			case SnippetsWebServerTypes.doneSubsearch:
				this.dispatchEvent({type:'donesubsearch'});
				break;
			case SnippetsWebServerTypes.tooBusy:
				this.dispatchEvent({type:'toobusy'});
				break;
			case SnippetsWebServerTypes.foundSnippet:
				console.log('found snippet');
				this.dispatchEvent({type:'foundsnippet', snippetMessage:FoundSnippetMessage.fromJSON(payload).snippet});
				break;
			case SnippetsWebServerTypes.snippetToLoad:
				this.dispatchEvent({type:'snippettoload', snippetMessage:SnippetToLoadMessage.fromJSON(payload).snippet});
				break;
		}
	}
	continueSearch(){
		this._virtualSocket.send(SnippetsWebServerTypes.continueSearch, {});
	}
	loadSnippet(snippetId){
		this._virtualSocket.send(SnippetsWebServerTypes.loadSnippetForSearch, LoadSnippetRequest.toJSON({snippetId}));
	}
	dispose(){
		this._virtualSocket.removeEventListener('message', this._handleMessage);
		this._virtualSocket.dispose();
	}
}