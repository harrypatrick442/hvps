export default function getTimeStringFromDate(date){
	const hour = date.getHours();
	const minutes = date.getMinutes();
	const seconds=date.getSeconds();
	return `${hour}:${minutes}:${seconds}`;
}