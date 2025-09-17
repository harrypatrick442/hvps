#ifndef Flash_hpp
#define Flash_hpp
class Flash{
	private:
		static const char* TAG;
    public:
        static void initialize();
        static bool getIsInitialized();
		static bool getDouble(
			const char* namespaceName, const char* key, double &outValue);
		static bool setDouble(
			const char* namespaceName, const char* key, double value);
    private:
        static bool _isInitialized;
};
#endif // Flash_hpp