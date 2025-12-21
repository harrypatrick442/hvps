#pragma once
#include <string>
#include "../Core/CleanupBucket.hpp"
class Flash{
    public:
        static void initialize();
        static bool getIsInitialized();
		static bool getFloat(
			const char* namespaceName, const char* key, float &outValue);
		static bool setFloat(
			const char* namespaceName, const char* key, float value);
			
		template<typename T>
		static bool setNumber(const char* namespaceName, const char* key, T value);

		template<typename T>
		static bool getNumber(const char* namespaceName, 
			const char* key, T& outValue);
		
		static bool getString(const char* namespaceName, const char* key,
			std::string& outValue, size_t maxLength  = 0, bool allowTruncate = true);
		
		static bool setString(const char* namespaceName, const char* key,
                      const std::string& value,
                      bool allowEmptyErase = true);
					  
		static bool getCharStringOnHeap(
			const char* namespaceName,
			const char* key,
			char*& outStr,
			CleanupBucket& cleanupBucket,
			size_t maxLength = 0,
			bool allowTruncate = true);
					
		template <typename T>
		static bool getArray(const char* namespaceName, const char* key,
							 T*& outArray, size_t& outCount, 
							 CleanupBucket& cleanupBucket);
							 
		template <typename T>
		static bool setArray(const char* namespaceName, const char* key,
			const T* array, size_t count);
		
		static bool erase(const char* namespaceName, const char* key);
		static bool hasKey(const char* namespaceName, const char* key);

    private:
		static inline constexpr const char* STRING_VALUE_NOT_FOUND_FOR_KEY 
					= "String value not found for key '%s'";
		static inline constexpr const char* NVS_NOT_INITIALIZED = "NVS not initialized";
		static inline constexpr const char*	FAILED_OPEN_NAMESPACE 
			= "Failed to open NVS namespace: %s";
        static bool _isInitialized;
};
#include "Flash.tpp"