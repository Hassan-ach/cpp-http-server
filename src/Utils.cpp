#include "Utils.hpp"
#include <filesystem>
#include <unordered_map>

namespace Utils {
std::vector<std::string> split(const std::string &s,
                               const std::string &delimiter) {
    std::vector<std::string> result;
    if (s.empty())
        return result;

    size_t start = 0;
    size_t end = 0;

    // Skip leading delimiter
    if (!delimiter.empty() && s.starts_with(delimiter)) {
        start = delimiter.length();
    }

    while ((end = s.find(delimiter, start)) != std::string::npos) {
        if (end > start) {
            result.push_back(s.substr(start, end - start));
        }
        start = end + delimiter.length();

        if (start >= s.length())
            break;
    }

    if (start < s.length()) {
        result.push_back(s.substr(start));
    }

    return result;
}

std::string getMimeType(const std::string &filePath) {
    static const std::unordered_map<std::string, std::string> mimeTypes = {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".txt", "text/plain"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".pdf", "application/pdf"},
    };

    std::filesystem::path path(filePath);
    std::string extension = path.extension().string();

    auto it = mimeTypes.find(extension);
    if (it != mimeTypes.end()) {
        return it->second;
    }

    return "application/octet-stream";
}
} // namespace Utils
