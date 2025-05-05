#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

class TcpSocket {
public:
    TcpSocket();
    explicit TcpSocket(int fd);
    ~TcpSocket();

    int fd() const { return fd_; }
    
    ssize_t send(const void* buf, size_t len, int flags = 0) const;
    ssize_t recv(void* buf, size_t len, int flags = 0) const;

    // Disable copying
    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;

    // Enable moving
    TcpSocket(TcpSocket&& other) noexcept;
    TcpSocket& operator=(TcpSocket&& other) noexcept;

private:
    int fd_{-1};
};
