
	// --------------------------
	// Add a C-style array
	// --------------------------
	template<typename T>
	void JHelper::addArray(cJSON* parent, const char* key, const T* values, size_t count)
	{
		static_assert(std::is_arithmetic<T>::value, "T must be a numeric type");
		if (!parent || !key || !values) return;

		cJSON* arr = cJSON_CreateArray();
		if (!arr) return;

		for (size_t i = 0; i < count; ++i)
		{
			cJSON_AddItemToArray(arr, cJSON_CreateNumber(values[i]));
		}

		cJSON_AddItemToObject(parent, key, arr);
	}
	template <typename T>
	constexpr double JHelper::toDoubleSafe(T value) noexcept {
		static_assert(std::is_arithmetic_v<T>, "T must be numeric");

		if constexpr (std::is_floating_point_v<T>) {
			// already float/double/long double
			return static_cast<double>(value);
		} else if constexpr (std::is_unsigned_v<T>) {
			// promote to 64-bit first to avoid overflow on small doubles
			return static_cast<double>(static_cast<uint64_t>(value));
		} else if constexpr (std::is_signed_v<T>) {
			// promote to signed 64-bit before converting
			return static_cast<double>(static_cast<int64_t>(value));
		} else {
			// shouldn't ever happen, fallback
			return static_cast<double>(value);
		}
	}

	// --------------------------
	// Add an std::vector
	// --------------------------
	template<typename T>
	void JHelper::addVector(cJSON* parent, const char* key, const std::vector<T>& vec)
	{
		static_assert(std::is_arithmetic<T>::value, "T must be a numeric type");
		if (!parent || !key) return;

		cJSON* arr = cJSON_CreateArray();
		if (!arr) return;

		for (const auto& val : vec)
		{
			cJSON_AddItemToArray(arr, cJSON_CreateNumber(val));
		}

		cJSON_AddItemToObject(parent, key, arr);
	}
	
	template<typename T>
	T* JHelper::getArray(const cJSON* parent, const char* key, size_t& outLength)
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T* outBuffer = nullptr;
		outLength = 0;

		if (!parent || !key)
			return outBuffer;

		const cJSON* arr = cJSON_GetObjectItemCaseSensitive(parent, key);
		if (!cJSON_IsArray(arr))
			return outBuffer;

		// Count number of numeric elements
		size_t count = 0;
		const cJSON* item = nullptr;
		cJSON_ArrayForEach(item, arr)
		{
			if (cJSON_IsNumber(item))
				++count;
		}

		if (count == 0)
			return outBuffer; // empty array, still success

		// Allocate the buffer
		outBuffer = new (std::nothrow) T[count];
		if (!outBuffer)
			return outBuffer; // allocation failed

		// Fill it
		size_t index = 0;
		cJSON_ArrayForEach(item, arr)
		{
			if (!cJSON_IsNumber(item)) continue;
			outBuffer[index++] = static_cast<T>(item->valuedouble);
		}

		outLength = index;
		return outBuffer;
	}
	
	// --------------------------
	// Parse JSON array → std::vector<T>
	// --------------------------
	template<typename T>
	bool JHelper::getVector(const cJSON* parent, const char* key, std::vector<T>& outVec)
	{
		static_assert(std::is_arithmetic<T>::value, "T must be a numeric type");
		if (!parent || !key) return false;

		const cJSON* arr = cJSON_GetObjectItemCaseSensitive(parent, key);
		if (!cJSON_IsArray(arr)) return false;

		outVec.clear();
		const cJSON* item = nullptr;
		cJSON_ArrayForEach(item, arr)
		{
			if (!cJSON_IsNumber(item)) return false;
			outVec.push_back(static_cast<T>(item->valuedouble));
		}

		return true;
	}
	