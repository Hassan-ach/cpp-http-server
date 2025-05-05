#pragma once

#include <string>
#include <vector>

namespace Utils {
    std::vector<std::string> split(const std::string& s, const std::string& delimiter = " ");
    std::string getMimeType(const std::string& filePath);
}
