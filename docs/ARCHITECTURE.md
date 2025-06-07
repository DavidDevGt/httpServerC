# Architecture Documentation

## Overview

This HTTP server is designed with a modular architecture that separates concerns across different components. The design emphasizes performance, maintainability, and HTTP standard compliance.

## Core Components

### 1. Main Entry Point (`main.c`)
- Initializes the server
- Sets up signal handlers
- Starts the main event loop
- Handles graceful shutdown

### 2. Core Module (`core/`)

#### Server (`server.c/.h`)
- Socket creation and binding
- Listen socket configuration
- Basic server lifecycle management
- Server configuration and constants

#### Event Loop (`event_loop.c/.h`)
- I/O multiplexing using `select()`
- Connection event handling
- Timeout management
- Main server loop coordination

### 3. HTTP Module (`http/`)

#### HTTP Handler (`http_handler.c/.h`)
- HTTP request parsing
- Response generation
- MIME type detection
- Static file serving
- HTTP protocol compliance

### 4. Client Module (`client/`)

#### Client Manager (`client_manager.c/.h`)
- Connection lifecycle management
- Keep-alive functionality
- Connection pool management
- Resource cleanup

## Data Flow

```
1. New Connection Request
   ↓
2. Event Loop (select())
   ↓
3. Client Manager (accept/manage)
   ↓
4. HTTP Handler (parse request)
   ↓
5. File System (serve content)
   ↓
6. HTTP Handler (generate response)
   ↓
7. Client Manager (send response)
   ↓
8. Keep-Alive or Close Connection
```

## Keep-Alive Implementation

### Connection Management
- Each connection tracks request count
- Timeout tracking for idle connections
- Automatic cleanup of expired connections

### HTTP Headers
- `Connection: keep-alive` support
- `Keep-Alive: timeout=30, max=100` parameters
- HTTP/1.0 and HTTP/1.1 compatibility

### Performance Optimizations
- Connection reuse reduces overhead
- Non-blocking I/O operations
- Efficient memory management

## Security Considerations

### Path Traversal Protection
- Uses `realpath()` to resolve symbolic links
- Validates requested paths against document root
- Prevents access to system files

### Input Validation
- Bounds checking on all input
- Safe string handling
- Buffer overflow protection

### Resource Limits
- Maximum connection limits
- Request size limitations
- Timeout enforcement

## Configuration

### Compile-time Constants
```c
#define MAX_CONNECTIONS 100
#define KEEP_ALIVE_TIMEOUT 30
#define MAX_REQUESTS_PER_CONNECTION 100
#define BUFFER_SIZE 4096
#define PORT 6090
```

### Runtime Configuration
- Document root: `./www/`
- Server identification header
- Error response templates

## Error Handling

### Connection Errors
- Graceful connection termination
- Client disconnect detection
- Resource cleanup on errors

### HTTP Errors
- Standard HTTP error codes
- Custom error pages
- Detailed error logging

### System Errors
- Socket operation failures
- File system errors
- Memory allocation failures

## Performance Characteristics

### Memory Usage
- Fixed-size connection pool
- Per-connection buffers
- Minimal dynamic allocation

### CPU Efficiency
- Event-driven architecture
- No busy waiting
- Efficient I/O multiplexing

### Scalability
- Handles up to 100 concurrent connections
- Linear performance scaling
- Minimal per-connection overhead

## Testing Strategy

### Unit Testing
- Individual function testing
- Component isolation
- Mock dependencies

### Integration Testing
- Full request/response cycles
- Keep-alive functionality
- Error condition handling

### Performance Testing
- Concurrent connection limits
- Request throughput measurement
- Memory usage profiling

## Future Enhancements

### Potential Improvements
- HTTPS/TLS support
- HTTP/2 protocol support
- Dynamic content generation
- Caching mechanisms
- Load balancing support
- Configuration file support

### Code Organization
- Plugin architecture for handlers
- Modular MIME type system
- Configurable routing
- Enhanced logging framework
