export default class RoundingHelper {
	static toSignificantFigures(num, n) {
		if (num === 0) return 0;
		const d = Math.ceil(Math.log10(Math.abs(num)));
		const power = n - d;
		const magnitude = Math.pow(10, power);
		return Math.round(num * magnitude) / magnitude;
	}
}