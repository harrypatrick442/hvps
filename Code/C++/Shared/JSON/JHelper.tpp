
	// --------------------------
	// Add a C-style array
	// --------------------------
	template<typename T>
	void JHelper::addArray(cJSON* parent, const char* key, const T* values, size_t count)
	{
		static_assert(std::is_arithmetic<T>::value, "T must be a numeric type");
		if (
			(parent==nullptr) 
			|| (key==nullptr) 
			|| (values==nullptr)
			||(count<1)
		) return;
		cJSON* arr = cJSON_CreateArray();
		if (!arr) return;
		for (size_t i = 0; i < count; ++i)
		{
			cJSON_AddItemToArray(arr, cJSON_CreateNumber(values[i]));
		}
		cJSON_AddItemToObject(parent, key, arr);
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
	