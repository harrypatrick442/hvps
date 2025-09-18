export default function getDateStringFromDate(date){
	const dayOfMonth = date.getDay();
	const month = date.getMonth()+1;
	const year = date.getYear()+1900;
	return `${dayOfMonth}${month}${year}`;
}