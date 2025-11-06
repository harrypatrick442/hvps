#include "Base64.hpp"
size_t Base64::getEncodeLength(const std::vector<uint8_t>& data){
	return 4* ((data.size() + 2) / 3);
}
char* Base64::encode(const std::vector<uint8_t>& data, CleanupBucket& cleanupBucket) {
	size_t length = getEncodeLength(data);
	char* charArray = new char[length + 1];
	cleanupBucket.addDeleteArray(charArray);
	encodeToCharArray(0, charArray, data);
	charArray[length] = '\0';
	return charArray;
}
void Base64::encodeToCharArray(
	size_t indexFromInclusive,
	char* charArray,
	const std::vector<uint8_t>& data) {
	static constexpr char base64_table[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static constexpr int mod_table[] = {0, 2, 1};
	
	size_t input_length = data.size();
	size_t output_length = getEncodeLength(data);

    for (int i = 0, j = indexFromInclusive; i < input_length;) {

        uint32_t octet_a = i < input_length ? (uint8_t)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (uint8_t)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (uint8_t)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        charArray[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
        charArray[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
        charArray[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
        charArray[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        charArray[output_length - 1 - i] = '=';
}
