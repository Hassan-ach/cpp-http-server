#pragma once

#include <string>
#include <unordered_map>

class HttpResponse {
public:
    HttpResponse() = default;

    void setStatus(const std::string& status) { status_ = status; }
    void setHeader(const std::string& key, const std::string& value) { headers_[key] = value; }
    void setBody(const std::string& body) { body_ = body; }

    std::string toString() const;

private:
    std::string status_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};
