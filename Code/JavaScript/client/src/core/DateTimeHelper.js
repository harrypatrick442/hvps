import padZerosToStart from './padZerosToStart';
const MILLISECONDS_DAY = 1000*3600*24;
const MILLISECONDS_MONTH_AVERAGE = 1000*3600*24*30.4;
const MILLISECONDS_IN_SHORT_YEAR = 1000*3600*24*365;
const MILLISECONDS_HOUR = 3600000;
const MILLISECONDS_MINUTE = 60000;

export default class DateTimeHelper {
	static getDayOfWeekForFirstDayOfMonth(date) {
		date = new Date(date.getTime());
		date.setDate(1);
		return date.getDay();
	}
	static getDaysInPreviousMonthFromDate(date){
		const newDate = new Date(date);
		newDate.setDate(0);
		return newDate.getDate();
	}
	static daysInMonthFromDate(date){
		return DateTimeHelper.daysInMonth(date.getMonth()+1, date.getYear()+1900);
	}
	static daysInMonth (month, year) {
		return new Date(year, month, 0).getDate();
	}
	static getDayOfWeekString(index, shortString) {
		if(shortString){
			switch(index){
				case 0:return 'Mon';
				case 1:return 'Tue';
				case 2:return 'Wed';
				case 3:return 'Thu';
				case 4:return 'Fri';
				case 5:return 'Sat';
				case 6:return 'Sun';
			}
		}
		switch(index){
			case 0:return 'Monday';
			case 1:return 'Tuesday';
			case 2:return 'Wednesday';
			case 3:return 'Thursday';
			case 4:return 'Friday';
			case 5:return 'Saturday';
			case 6:return 'Sunday';
		}
	}
	static getTimeAgoAbbreviated(millisecondsUTC){
		var millisecondsDifference = new Date().getTime()-millisecondsUTC;
		if(millisecondsDifference<=0)return 'now';
		const years = Math.floor(millisecondsDifference/MILLISECONDS_IN_SHORT_YEAR);
		if(years>0)
			return `${parseInt(years)} yr`;
		const months = Math.floor(millisecondsDifference/MILLISECONDS_MONTH_AVERAGE);
		if(months>0)
			return `${parseInt(months)} mos`;
		const days=Math.floor(millisecondsDifference/MILLISECONDS_DAY);
		if(days>0)
			return `{parseInt(days)} d`;
		const hours = Math.floor(millisecondsDifference/MILLISECONDS_HOUR);
		if(hours>0) 
			return `${parseInt(hours)} hr`;
		const minutes = Math.floor(millisecondsDifference/MILLISECONDS_MINUTE);
		if(minutes>0)
			return `${parseInt(minutes)} min`;
		return 'now';
	}
	static getDateStringWithMonthWordFromDate(date){
		const yearString = String(date.getYear()+1900);
		const monthString = DateTimeHelper.getMonthString(date.getMonth(), true);
		const day = date.getDate();
		return `${day} ${monthString} ${yearString}`;
	}
	static getDateStringWithNumbersFromDate(date){
		const yearString = String(date.getYear()+1900);
		const monthString = String(date.getMonth()+1);
		const day = date.getDate();
		return `${padZerosToStart(String(day), 2)}/${padZerosToStart(monthString, 2)}/${yearString}`;			
	}
	static getTimeStringFromDate(date){
		const hours = date.getHours();
		const minutes = date.getMinutes();
		const seconds = date.getSeconds();
		return `${padZerosToStart(String(hours), 2)}:${padZerosToStart(String(minutes), 2)}:${padZerosToStart(String(seconds), 2)}`;
	}
	static getPrependAndTime(millisecondsUTC){
		const date = new Date(millisecondsUTC);
		return `${DateTimeHelper._getPrependStringFromDate(date)} ${DateTimeHelper.getTimeStringFromDateNoSeconds(date)}`
	}
	static _getPrependStringFromDate(date){
		const now = new Date().getTime();
		const millisecondsUTC = date.getTime();
		const currentDays = Math.floor(now / 8.64e7);
		const messageDays = Math.floor(millisecondsUTC / 8.64e7);
		const differenceDays = currentDays - messageDays;
		if (differenceDays === 0)
			return 'Today at';
		if (differenceDays === 1)
			return 'Yesterday at';
		return DateTimeHelper.getDateStringWithNumbersFromDate(date);
	}
	static getTimeStringFromDateNoSeconds(date){
		const hours = date.getHours();
		const minutes = date.getMinutes();
		return `${padZerosToStart(String(hours), 2)}:${padZerosToStart(String(minutes), 2)}`;
	}
	static isToday(date){
		const today = new Date();
		return date.getDate() == today.getDate() &&
			date.getMonth() == today.getMonth() &&
			date.getFullYear() == today.getFullYear();
	}
	static getMonthString(monthIndex, small){
		if(small===undefined)
			small = false;
		if(small){
			
			switch(monthIndex){
				case 0:
					return 'Jan';
				case 1:
					return 'Feb';
				case 2:
					return 'Mar';
				case 3:
					return 'Apr';
				case 4:
					return 'May';
				case 5:
					return 'Jun';
				case 6:
					return 'Jul';
				case 7:
					return 'Aug';
				case 8:
					return 'Sep';
				case 9:
					return 'Oct';
				case 10:
					return 'Nov';
				case 11:
					return 'Dec';
			}
		}
		switch(monthIndex){
			case 0:
				return 'January';
			case 1:
				return 'Febuary';
			case 2:
				return 'March';
			case 3:
				return 'April';
			case 4:
				return 'May';
			case 5:
				return 'June';
			case 6:
				return 'July';
			case 7:
				return 'August';
			case 8:
				return 'September';
			case 9:
				return 'October';
			case 10:
				return 'November';
			case 11:
				return 'December';
		}
	}
	static getDaysInMonth (monthIndex, year) {
		return new Date(year, monthIndex+1, 0).getDate();
	}
	static  getDayOfMonthSuffix(value){
		if (value > 3 && value < 21) return 'th';
		switch (value % 10) {
			case 1:  return "st";
			case 2:  return "nd";
			case 3:  return "rd";
			default: return "th";
		}
	}
}