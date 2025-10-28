
class Flash{
    public:
        static void initialize();
        static bool getIsInitialized();
		static bool getDouble(
			const char* namespaceName, const char* key, double &outValue);
		static bool setDouble(
			const char* namespaceName, const char* key, double value);
		static bool setString(
			const char* namespaceName,
			const char* key,
			const std::string& value,
			size_t maxLength = 256);

		static bool Flash::getString(const char* namespaceName, const char* key,
			std::string& outValue, size_t maxLength  = 0, bool allowTruncate = true);
		static bool setString(
			const char* namespaceName,
			const char* key,
			const char* value,
			size_t maxLength = 256);
		static bool getString(
			const char* namespaceName,
			const char* key,
			char* outValue,
			size_t maxLength);
    private:
		static inline constexpr const char* TAG = "Flash";
		static inline constexpr const char* STRING_VALUE_NOT_FOUND_FOR_KEY 
					= "String value not found for key '%s'",
			NVS_NOT_INITIALIZED = "NVS not initialized",
			FAILED_OPEN_NAMESPACE = "Failed to open NVS namespace: %s";
        static bool _isInitialized;
};