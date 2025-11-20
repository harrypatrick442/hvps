class LedStrip{
	private:
		ProgrammableRGBLedPixel* _leds;
		atomic<bool> _changed;
	public:
		LedStrip();
		void refresh();
		IRGBPixel* getPixels(size_t& length);
};