export default class ClipboardHelper{
	static copyString(str){
		///TODO native implementation required?
		if('clipboard' in navigator){
			navigator.clipboard.writeText(str);
			return;
		}
		const textArea = document.createElement('textarea');
		textArea.value = str;
		textArea.style.opacity = 0;
		document.body.appendChild(textArea);
		try{
			textArea.focus();
			textArea.select();
			try {
			  const success = document.execCommand('copy');
			  if(success)return;
			} catch (err) {
			  console.error(err);
			}
		}
		catch(err){
			console.error(err);
		}
		finally{
			document.body.removeChild(textArea);
		}
		console.error('No supported methods of copying to clipboard found');
	}
}