#pragma once
class Display{
	
	private:
		private size_t _width;
		private size_t _height;
		IRGBPixel** _hardwarePixelsFlattenedArray;
		//Populate this by calling getPixels on ILedStrip and then iterating over them and placing pointers in this array.
		
		
	protected:
		Display(size_t width, size_t height);
		void addHardwareRowMajor(ILedStrip ledStrip, size_t offset);
		void refresh();
		//addHardwareColumnMajor(ILedStrip ledStrip, size_t offset);
	private:
		void checkHardwareComplete();
		
};