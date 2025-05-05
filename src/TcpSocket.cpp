#include "TcpSocket.hpp"
#include <stdexcept>

TcpSocket::TcpSocket() {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        throw std::runtime_error("Failed to create socket");
    }
}

TcpSocket::TcpSocket(int fd) : fd_(fd) {
    if (fd_ < 0) {
        throw std::runtime_error("Invalid file descriptor");
    }
}

TcpSocket::~TcpSocket() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

TcpSocket::TcpSocket(TcpSocket &&other) noexcept : fd_(other.fd_) {
    other.fd_ = -1;
}

TcpSocket &TcpSocket::operator=(TcpSocket &&other) noexcept {
    if (this != &other) {
        if (fd_ >= 0) {
            close(fd_);
        }
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

ssize_t TcpSocket::send(const void *buf, size_t len, int flags) const {
    return ::send(fd_, buf, len, flags);
}

ssize_t TcpSocket::recv(void *buf, size_t len, int flags) const {
    return ::recv(fd_, buf, len, flags);
}
