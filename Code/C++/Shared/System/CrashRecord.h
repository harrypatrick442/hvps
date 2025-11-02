#pragma once
#include "esp_system.h"
#include "esp_err.h"
#include <vector>
#include <string>
#include <sstream>

struct CrashRecord {
    esp_reset_reason_t reason;     // Reset cause
    std::vector<uint8_t> dump;     // Raw core dump bytes
    std::string message;           // Description or error message

    std::string toJSONString() const {
        std::ostringstream oss;

        oss << "{";

        // reason
        oss << "\"reason\":" << static_cast<int>(reason) << ",";

        // message (with minimal escaping)
        oss << "\"message\":\"";
        for (char c : message) {
            switch (c) {
                case '"':  oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\n': oss << "\\n";  break;
                case '\r': oss << "\\r";  break;
                case '\t': oss << "\\t";  break;
                default:   oss << c; break;
            }
        }
        oss << "\",";

        // dump as JSON array of bytes
        oss << "\"dump\":[";
        for (size_t i = 0; i < dump.size(); ++i) {
            oss << static_cast<unsigned int>(dump[i]);
            if (i + 1 < dump.size()) oss << ",";
        }
        oss << "]";

        oss << "}";

        return oss.str();
    }

};