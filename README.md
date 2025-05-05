# C++ HTTP Server

A lightweight HTTP/1.1 server implemented in modern C++ with thread-per-connection architecture.

## Features

- **HTTP Methods**: GET, POST
- **Endpoints**:
    - `GET /` - Basic health check
    - `GET /echo/<message>` - Returns the message back
    - `GET /user-agent` - Returns client's User-Agent
    - `GET /files/<filename>` - Downloads files
    - `POST /files/<filename>` - Uploads files
- **Threading**: Handles multiple clients concurrently
- **File Operations**: Secure file upload/download

## Quick Start

### Build & Run

```bash
# Clone and build
git clone https://github.com/Hassan-ach/cpp-http-server.git
cd cpp-http-server
mkdir build && cd build
cmake .. && make

# Run server (with optional directory for files)
./server --dir /path/to/files
```

### Example Usage

```bash
# Upload a file
curl -X POST --data-binary @localfile.txt \
  http://localhost:4221/files/remotefile.txt

# Download a file
curl http://localhost:4221/files/remotefile.txt -o downloaded.txt

# Echo test
curl http://localhost:4221/echo/hello
```

## API Reference

### GET /files/{filename}

- **Success**: `200 OK` with file contents
- **Failure**: `404 Not Found`

### POST /files/{filename}

- **Success**: `201 Created` (file saved)
- **Failure**:
    - `403 Forbidden` (invalid filename)
    - `505 Server Error` (file exists/other errors)

### GET /echo/{message}

- Always returns `200 OK` with the message

## Configuration

| Flag    | Description                   |
| ------- | ----------------------------- |
| `--dir` | Directory for file operations |

## Project Structure

```
/server/
├── CMakeLists.txt           # Build configuration
├── main.cpp                 # Entry point
├── include/                 # All public headers
│   ├── HttpServer.hpp       # Server core interface
│   ├── TcpSocket.hpp        # Socket wrapper interface
│   ├── HttpRequest.hpp      # Request parsing interface
│   ├── HttpResponse.hpp     # Response building interface
│   └── Utils.hpp            # Helper functions interface
└── src/                     # Implementation files
    ├── HttpServer.cpp       # Server core implementation
    ├── TcpSocket.cpp        # Socket wrapper implementation
    ├── HttpRequest.cpp      # Request parsing implementation
    ├── HttpResponse.cpp     # Response building implementation
    └── Utils.cpp            # Helper functions implementation
```

## Error Handling

- All file operations include existence checks
- Path sanitization prevents directory traversal
- Comprehensive exception handling

## License

MIT
