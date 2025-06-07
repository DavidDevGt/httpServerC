# API Documentation

## HTTP Server C - API Reference

This document describes the internal API of the HTTP Server C implementation.

## Core API

### Server Management

#### `int server_init(int port)`
Initializes the server on the specified port.

**Parameters:**
- `port`: Port number to bind the server to

**Returns:**
- `0` on success
- `-1` on failure

**Example:**
```c
if (server_init(6090) != 0) {
    fprintf(stderr, "Failed to initialize server\n");
    exit(1);
}
```

#### `void server_cleanup()`
Performs cleanup of server resources.

**Usage:**
```c
server_cleanup();
```

### Event Loop

#### `int event_loop_run(int server_fd)`
Runs the main event loop for handling connections.

**Parameters:**
- `server_fd`: Server socket file descriptor

**Returns:**
- `0` on normal termination
- `-1` on error

### Client Management

#### `int client_accept(int server_fd)`
Accepts a new client connection.

**Parameters:**
- `server_fd`: Server socket file descriptor

**Returns:**
- Client socket file descriptor on success
- `-1` on failure

#### `void client_cleanup(int client_fd)`
Cleans up client connection resources.

**Parameters:**
- `client_fd`: Client socket file descriptor

#### `int client_should_keep_alive(ClientConnection *conn)`
Determines if a connection should be kept alive.

**Parameters:**
- `conn`: Pointer to client connection structure

**Returns:**
- `1` if connection should be kept alive
- `0` if connection should be closed

### HTTP Handling

#### `int http_parse_request(const char *request, HttpRequest *parsed)`
Parses an HTTP request string.

**Parameters:**
- `request`: Raw HTTP request string
- `parsed`: Pointer to HttpRequest structure to fill

**Returns:**
- `0` on successful parsing
- `-1` on parse error

#### `int http_generate_response(const HttpRequest *request, char *response_buffer, size_t buffer_size)`
Generates an HTTP response for the given request.

**Parameters:**
- `request`: Parsed HTTP request
- `response_buffer`: Buffer to write response to
- `buffer_size`: Size of response buffer

**Returns:**
- Number of bytes written to buffer
- `-1` on error

#### `const char* http_get_mime_type(const char *file_extension)`
Gets MIME type for a file extension.

**Parameters:**
- `file_extension`: File extension (e.g., ".html")

**Returns:**
- MIME type string
- "application/octet-stream" for unknown types

## Data Structures

### ClientConnection
```c
typedef struct {
    int socket_fd;              // Socket file descriptor
    time_t last_activity;       // Last activity timestamp
    int request_count;          // Number of requests on this connection
    int keep_alive;             // Keep-alive flag
    char buffer[BUFFER_SIZE];   // Request buffer
    size_t buffer_used;         // Bytes used in buffer
} ClientConnection;
```

### HttpRequest
```c
typedef struct {
    char method[16];            // HTTP method (GET, POST, etc.)
    char path[512];             // Request path
    char version[16];           // HTTP version
    char headers[MAX_HEADERS][2][256]; // Headers as key-value pairs
    int header_count;           // Number of headers
    int keep_alive;             // Keep-alive flag from headers
} HttpRequest;
```

### HttpResponse
```c
typedef struct {
    int status_code;            // HTTP status code
    char status_text[64];       // Status text
    char content_type[64];      // Content-Type header
    char *body;                 // Response body
    size_t body_length;         // Body length
    int keep_alive;             // Keep-alive flag
} HttpResponse;
```

## Constants

### Configuration Constants
```c
#define MAX_CONNECTIONS 100
#define KEEP_ALIVE_TIMEOUT 30          // seconds
#define MAX_REQUESTS_PER_CONNECTION 100
#define BUFFER_SIZE 4096
#define PORT 6090
#define DOCUMENT_ROOT "./www"
```

### HTTP Status Codes
```c
#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_ERROR 500
#define HTTP_BAD_REQUEST 400
```

## Error Codes

### Return Values
- `0`: Success
- `-1`: General error
- `-2`: Memory allocation error
- `-3`: Socket error
- `-4`: File system error
- `-5`: Protocol error

### Error Handling Example
```c
int result = http_parse_request(request_str, &parsed_request);
switch (result) {
    case 0:
        // Success - process request
        break;
    case -1:
        // Parse error - send 400 Bad Request
        break;
    case -2:
        // Memory error - send 500 Internal Server Error
        break;
    default:
        // Unknown error
        break;
}
```

## Usage Examples

### Basic Server Setup
```c
#include "server.h"
#include "event_loop.h"

int main() {
    // Initialize server
    int server_fd = server_init(6090);
    if (server_fd < 0) {
        return 1;
    }
    
    // Run event loop
    event_loop_run(server_fd);
    
    // Cleanup
    server_cleanup();
    return 0;
}
```

### Processing HTTP Requests
```c
void handle_client_request(int client_fd) {
    char buffer[BUFFER_SIZE];
    HttpRequest request;
    
    // Read request
    int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        return;
    }
    buffer[bytes_read] = '\0';
    
    // Parse request
    if (http_parse_request(buffer, &request) != 0) {
        // Send error response
        return;
    }
    
    // Generate response
    char response[BUFFER_SIZE];
    int response_size = http_generate_response(&request, response, sizeof(response));
    
    // Send response
    send(client_fd, response, response_size, 0);
}
```

### Keep-Alive Management
```c
void manage_connection(ClientConnection *conn) {
    // Update activity timestamp
    conn->last_activity = time(NULL);
    conn->request_count++;
    
    // Check if connection should be kept alive
    if (client_should_keep_alive(conn)) {
        // Keep connection open
        conn->keep_alive = 1;
    } else {
        // Close connection
        conn->keep_alive = 0;
        client_cleanup(conn->socket_fd);
    }
}
```

## Thread Safety

**Note:** This implementation is single-threaded and uses select() for I/O multiplexing. Functions are not thread-safe and should not be called from multiple threads simultaneously.

## Memory Management

- All dynamically allocated memory must be freed by the caller
- Connection structures are managed by the client manager
- File content is read into temporary buffers and freed after use
- No global memory leaks in normal operation

## Performance Considerations

- Keep response generation fast to maintain connection throughput
- Use efficient string operations for header parsing
- Minimize memory allocations in request processing
- Implement proper timeout handling to prevent resource leaks
