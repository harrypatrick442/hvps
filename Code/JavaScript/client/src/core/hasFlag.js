export default function hasFlag(flag, value){
	return (value&flag)===flag;
}