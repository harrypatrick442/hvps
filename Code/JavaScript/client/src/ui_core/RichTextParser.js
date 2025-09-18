import EmoticonsManager from '../emoticons/EmoticonsManager';
import MessageV2Content from '../messages/MessageV2Content';
import MessageV2ContentType from '../enums/MessageV2ContentType';
import MentionViewModel from '../view_models/echat/MentionViewModel';
import EmoticonView from '../views/emoticons/Emoticon';
import MentionView from '../views/echat/Mention';
import NodeType from '../enums/NodeType';
import CharHelper from '../core/CharHelper';
import isNullOrUndefined from '../core/isNullOrUndefined';
import HashTagViewModel from '../view_models/echat/HashTagViewModel';
import HashTagView from '../views/echat/HashTag';
import E from './E';
const invisibleSpace = String.fromCharCode(65279);
const ALLOWED_END_MENTION_BY_ID_CHARACTERS = [' ', '\r', '\n']
export default class RichTextParser{
	static fromElementsToContentString({element, version}){
		if(version<2){
			return {content:element.textContent};
		}
		const entries = [];
		let mentionUserIds = [];
		let tags = [];
		RichTextParser._fromElementToContentString(element, entries, mentionUserIds, tags);
		if(entries.length<1){
			return null;
		}
		return {
			content:JSON.stringify(MessageV2Content.toJSON({entries})), 
			mentionUserIds:(mentionUserIds.length>0?mentionUserIds:null),
			tags:(tags.length>0?tags:null)
		};
	}
	static _fromElementToContentString(parentElement, entries, mentionUserIds, tags){
		parentElement.childNodes.forEach(n=>{
			if(n.nodeType===NodeType.Text){
				RichTextParser._extractFromText(n.textContent, entries);
				return;
			}
			if(n.classList.contains('message')){
				RichTextParser._fromElementToContentString(n, entries, mentionUserIds, tags);
				return;
			}
			if(n.isEmoticon){
				const model = n.model;
				const {code}=model;
				if(code){
					entries.push({code, type:MessageV2ContentType.Emoticon});
					return;
				}
			}
			if(n.isMention){
				const model = n.model;
				const {userIdMentioning}=model;
				if(userIdMentioning){
					mentionUserIds.push(userIdMentioning);
					entries.push({userId:userIdMentioning, type:MessageV2ContentType.Mention});
					return;
				}
			}
			if(n.isHashTag){
				console.log('was hash tag');
				const model = n.model;
				const {tag}=model;
				if(tag){
					console.log('tag');
					console.log(tag);
					tags.push(tag);
					entries.push({tag, type:MessageV2ContentType.HashTag});
					return;
				}
			}
			if(n.classList.contains('emoticon')){
				let code = n.getAttribute('code');
				if(code){
					code = parseInt(code);
					if(EmoticonsManager.instance.hasCode(code)){
						entries.push({code, type:MessageV2ContentType.Emoticon});
						return;
					}
				}
				let multimediaToken = n.getAttribute('multimediatoken');
				if(multimediaToken){
					
				}
			}
			if(n.classList.contains('mention')){
				let userId = n.getAttribute('userid');
				if(userId){
					userId = parseInt(userId);
					if(mentionUserIds===null)
						mentionUserIds =[];
					mentionUserIds.push(userId);
					entries.push({userId, type:MessageV2ContentType.Mention});
					return;
				}
			}
			if(n.classList.contains('hash-tag')){
				let tag = n.getAttribute('tag');
				if(tag){
					tags.push(tag);
					entries.push({tag, type:MessageV2ContentType.HashTag});
					return;
				}
			}
			RichTextParser._extractFromText(n.textContent, entries);
		});
	}
	static unpackageContent({content, version}){
		if(isNullOrUndefined(content))
			return null;
		if(version<2)
			return {version, content};
		const obj = JSON.parse(content);
		if(isNullOrUndefined(obj))
			return null;
		return {version, content:MessageV2Content.fromJSON(obj)};
	}
	static fromContentToElements({element, content, disposes, version, setUneditable,
		callbackChanged, callbackClickedMention, callbackClickedHashTag}){
		const unpackagedContent = RichTextParser.unpackageContent({content, version});
		return RichTextParser.fromUnpackagedContentToElements({element, unpackagedContent,
			disposes, setUneditable, callbackChanged, callbackClickedMention,
			callbackClickedHashTag
		});
	}
	static fromUnpackagedContentToElements({element, unpackagedContent,
		disposes, setUneditable, callbackChanged, callbackClickedMention,
		callbackClickedHashTag
	})
	{
		if(isNullOrUndefined(unpackagedContent))
			return;
		if(unpackagedContent.version<2)
		{
			const textNode = E.textNode(unpackagedContent);
			if(!textNode)return;
			element.appendChild(textNode);
		}
		RichTextParser._fromContentEntriesToElements({
			element, 
			contentEntries:unpackagedContent.content.entries,
			setUneditable,
			disposes,
			callbackChanged,
			callbackClickedMention,
			callbackClickedHashTag
		});
	}
	static _fromContentEntriesToElements({element, contentEntries, disposes, setUneditable, 
		callbackChanged, callbackClickedMention, callbackClickedHashTag}){
		const appendChild = !setUneditable
			?(v)=>element.appendChild(v.element)
			:(v)=>{
				v.element.contentEditable = false;
				element.appendChild(v.element);
			};
		contentEntries?.forEach(entry=>{
			const{type} = entry;
			switch(type){
				case MessageV2ContentType.Text:
					const textNode = E.textNode(entry.text);
					element.appendChild(textNode);
					return;
				case MessageV2ContentType.Emoticon:
					let {code}=entry;
					const emoticon = EmoticonsManager.instance.getFromCode(code);
					if(!emoticon){
						const textNode = E.textNode(String.fromCodePoint("0x"+code.toString(16)));
						element.appendChild(textNode);
						return;
					}
					const emoticonView = new EmoticonView({model:emoticon});
					disposes.push(emoticonView.dispose);
					appendChild(emoticonView);
					return;
				case MessageV2ContentType.Mention:
					let {userId}=entry;
					const mention = new MentionViewModel({userIdMentioning:userId, callbackClicked:callbackClickedMention});
					const mentionView = new MentionView({model:mention});
					disposes.push(mention.dispose);
					disposes.push(mentionView.dispose);
					appendChild(mentionView);
					return;
				case MessageV2ContentType.HashTag:
					let {tag}=entry;
					const hashTag = new HashTagViewModel({tag, callbackClicked:callbackClickedHashTag});
					const hashTagView = new HashTagView({model:hashTag});
					disposes.push(hashTag.dispose);
					disposes.push(hashTagView.dispose);
					appendChild(hashTagView);
					return;
			}
		});
	}
	/*
	static fromSingleStringToElements({text, element, disposes, setUneditable, callbackChanged}){
		const entries = [];
		RichTextParser._extractFromText(text, entries);
		console.log('entries was ');
		console.log(entries);
		return RichTextParser._fromContentEntriesToElements({
			element, contentEntries:entries, disposes, setUneditable, callbackChanged
		});
	}*/
	static _extractFromText(text, entries){
		text = text.replaceAll(invisibleSpace, '');
		if(text.length<1)
			return;
		RichTextParser._findMentionsByUserIdWithinString(text).forEach(m=>{
			const {userIdMentioning , str} = m;
			if(!isNullOrUndefined(userIdMentioning)){
				entries.push({userId:userIdMentioning, type:MessageV2ContentType.Mention});
				return;
			}
			EmoticonsManager.instance.parseStringWithShortcuts(str).forEach(find=>{
				const {emoticon, str}=find;
				if(emoticon){					
					entries.push({code:emoticon.code, type:MessageV2ContentType.Emoticon});
					return;
				}
				let currentText='';
				for(var c of str){
					const code = c.codePointAt(0);
					if(EmoticonsManager.instance.hasCode(code)){
						if(currentText.length>0){
							entries.push({text:currentText, type:MessageV2ContentType.Text});
							currentText = '';
						}
						entries.push({code, type:MessageV2ContentType.Emoticon});
						continue;
					}
					currentText+=c;
				}
				if(currentText.length>0)
					entries.push({text:currentText, type:MessageV2ContentType.Text});
			});
		});
	}
	static _findMentionsByUserIdWithinString(str){
		const res =[];
		let i=0;
		let runStartIndex=0;
		const length = str.length;
		const lengthMinusOne = length - 1;
		let c;
		let lastEntry;
		const appendStrEntry = ()=>{			
			const s = str.substr(runStartIndex, i-runStartIndex);
			if(lastEntry&&!isNullOrUndefined(lastEntry.str)){
				lastEntry.str+=s;
			}
			else{
				lastEntry = {str:s};
				res.push(lastEntry);
			}
		};
		while(i<length){
			c = str[i];
			if(c==='@'){
				if(i>runStartIndex){
					appendStrEntry();
					runStartIndex = i;	
				}
				i++;
				const firstNumberIndex = i;
				let hasNumber = false;
				while(true){
					c = str[i];
					const isNumberChar = CharHelper.isNumber(c);
					if(isNumberChar){					
						hasNumber = true;
						i++;
						if(i<length)continue;
					}
					if(hasNumber
						&&(
							(ALLOWED_END_MENTION_BY_ID_CHARACTERS.indexOf(c)>=0)
							||(i>=length-1)
						)
					){
						lastEntry = {userIdMentioning:parseInt(str.substr(firstNumberIndex, i-firstNumberIndex))};
						res.push(lastEntry);
						runStartIndex = i;
					}
					break;
				}
				continue;
			}
			i++;
		}
		if(i>runStartIndex){
			appendStrEntry();
		}
		return res;
	}
	static toSingleStringExpression({content, version}){
		const unpackagedContent = RichTextParser.unpackageContent({content, version});
		const strs = [];
		unpackagedContent.content.entries?.forEach(entry=>{
			const{type} = entry;
			switch(type){
				case MessageV2ContentType.Text:
					strs.push(entry.text);
					return;
				case MessageV2ContentType.Emoticon:
					let {code}=entry;
					const emoticon = EmoticonsManager.instance.getFromCode(code);
					strs.push(String.fromCodePoint("0x"+code.toString(16)));
					return;
				case MessageV2ContentType.Mention:
					let {userId}=entry;
					strs.push('@'+userId);
					return;
			}
		});
		return strs.join('');
	}
}