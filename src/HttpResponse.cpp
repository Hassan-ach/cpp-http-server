#include "HttpResponse.hpp"

std::string HttpResponse::toString() const {
    std::string result = "HTTP/1.1 " + status_ + "\r\n";

    for (const auto &[key, value] : headers_) {
        result += key + ": " + value + "\r\n";
    }

    if (!body_.empty()) {
        result += "Content-Length: " + std::to_string(body_.size()) + "\r\n";
    }

    result += "\r\n" + body_;
    return result;
}
