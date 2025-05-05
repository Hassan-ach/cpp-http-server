#pragma once

#include <string>
#include <unordered_map>
#include <vector>

enum class HttpMethod { GET, POST, UNKNOWN };

struct RequestLine {
    HttpMethod method;
    std::string path;
    std::string httpVersion;
};

class HttpRequest {
  public:
    static HttpRequest parse(const std::string &request, int fd);

    const RequestLine &line() const { return line_; }
    const std::string &body() const { return body_; }
    const std::unordered_map<std::string, std::string> &headers() const {
        return headers_;
    }
    int clientFd() const { return clientFd_; }

  private:
    HttpRequest(RequestLine line, int fd)
        : line_(std::move(line)), clientFd_(fd) {}

    RequestLine line_;
    std::string body_;
    std::unordered_map<std::string, std::string> headers_;
    int clientFd_;
};
