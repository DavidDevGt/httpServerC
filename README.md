# HTTP Server in C

A high-performance HTTP server implementation in C featuring persistent connections, keep-alive functionality, and real-time monitoring.

## 🚀 Features

- **HTTP/1.1 Support** with HTTP/1.0 compatibility
- **Keep-Alive Connections** with configurable timeouts
- **Select-based I/O Multiplexing** for efficient connection handling
- **Real-time Connection Monitoring** and statistics
- **MIME Type Support** for HTML, CSS, JS, PNG, JPG files
- **Path Traversal Protection** using realpath()
- **Professional Web Interface** for testing and monitoring

## 🏗️ Architecture

```
httpServerC/
├── src/
│   ├── main.c                 # Entry point
│   ├── core/                  # Core server functionality
│   │   ├── server.c/.h        # Main server implementation
│   │   └── event_loop.c/.h    # Event loop and I/O multiplexing
│   ├── http/                  # HTTP protocol handling
│   │   └── http_handler.c/.h  # Request/response processing
│   └── client/                # Client connection management
│       └── client_manager.c/.h # Connection lifecycle management
├── www/                       # Web assets
│   └── index.html             # Testing console interface
├── build/                     # Compiled binaries
└── docs/                      # Documentation
```

## 🛠️ Building

### Prerequisites

- GCC compiler
- Make build system
- Linux/Unix environment

### Compilation

```bash
# Clone the repository
git clone https://github.com/DavidDevGt/httpServerC.git
cd httpServerC

# Build the server
make

# Clean build artifacts
make clean
```

### Running

```bash
# Start the server (default port: 6090)
./build/httpserver

# Access the web interface
# Open http://localhost:6090 in your browser
```

## 📊 Configuration

The server is configured with the following defaults:

- **Port**: 6090
- **Max Connections**: 100
- **Keep-Alive Timeout**: 30 seconds
- **Max Requests per Connection**: 100
- **Document Root**: `./www/`

## 🧪 Testing

The project includes a comprehensive web-based testing console accessible at `http://localhost:6090` that provides:

- **Keep-Alive Testing**: Verify persistent connection functionality
- **Multiple Requests**: Test connection reuse with sequential requests
- **Performance Benchmarks**: Concurrent request testing
- **Header Inspection**: Detailed HTTP header analysis
- **Real-time Monitoring**: Connection statistics and server status

### Manual Testing

```bash
# Test basic functionality
curl -v http://localhost:6090/

# Test keep-alive
curl -v -H "Connection: keep-alive" http://localhost:6090/

# Multiple requests on same connection
curl -v --keepalive-time 30 http://localhost:6090/ http://localhost:6090/
```

## 🔧 Technical Details

### Keep-Alive Implementation

- Automatic timeout handling (30s inactivity)
- Request counting per connection (max 100)
- Graceful connection cleanup
- HTTP/1.0 and HTTP/1.1 support

### I/O Multiplexing

- `select()` system call for efficient connection handling
- Non-blocking socket operations
- Concurrent connection support up to system limits

### Security Features

- Path traversal protection using `realpath()`
- Input validation and sanitization
- Safe file serving with MIME type detection

## 📈 Performance

- **Concurrent Connections**: Up to 100 simultaneous connections
- **Request Throughput**: Optimized for high-frequency requests
- **Memory Efficiency**: Minimal memory footprint per connection
- **CPU Optimization**: Event-driven architecture reduces CPU usage

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 👨‍💻 Author

**David Vargas** - [@DavidDevGt](https://github.com/DavidDevGt)

## 🙏 Acknowledgments

- HTTP/1.1 RFC 7230-7237 specifications
- POSIX socket programming standards
- Modern C development best practices

---

*Built with ❤️ using C*
