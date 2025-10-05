
class Flash{
    public:
        static void initialize();
        static bool getIsInitialized();
		static bool getDouble(
			const char* namespaceName, const char* key, double &outValue);
		static bool setDouble(
			const char* namespaceName, const char* key, double value);
    private:
		static inline constexpr const char* TAG = "Flash";
        static bool _isInitialized;
};