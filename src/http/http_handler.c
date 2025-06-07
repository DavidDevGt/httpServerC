#define _GNU_SOURCE  // for dprintf and realpath

#include "http_handler.h"
#include "../client/client_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  // for strncasecmp
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>

#define WWW_ROOT "./www"

int read_line(int fd, char *buf, int max)
{
    int i = 0, n;
    char c;
    while (i < max - 1)
    {
        n = recv(fd, &c, 1, 0); // read one byte
        if (n <= 0)
            return n;
        if (c == '\r')
            continue;
        if (c == '\n')
            break;
        buf[i++] = c; // save byte
    }
    buf[i] = '\0';
    return i; // return number of bytes read
}

int parse_http_version(const char *request_line)
{
    // Check if it's HTTP/1.1 or HTTP/1.0
    if (strstr(request_line, "HTTP/1.1"))
        return 11;
    else if (strstr(request_line, "HTTP/1.0"))
        return 10;
    return 11; // Default to HTTP/1.1
}

int parse_connection_header(const char *headers, int http_version)
{
    // Look for Connection: keep-alive or Connection: close
    const char *connection = strstr(headers, "Connection:");
    if (!connection)
    {
        // Default behavior based on HTTP version
        // HTTP/1.1 defaults to keep-alive, HTTP/1.0 defaults to close
        return (http_version >= 11) ? 1 : 0;
    }
    
    // Move past "Connection:"
    connection += 11;
    
    // Skip whitespace
    while (*connection == ' ' || *connection == '\t')
        connection++;
    
    // Check for keep-alive
    if (strncasecmp(connection, "keep-alive", 10) == 0)
        return 1;
    
    // Check for close
    if (strncasecmp(connection, "close", 5) == 0)
        return 0;
    
    // Default behavior based on HTTP version
    return (http_version >= 11) ? 1 : 0;
}

int handle_client(int fd)
{
    char line[BUF_SIZE];
    char headers[BUF_SIZE] = "";
    
    client_info_t *client = find_client(fd);
    if (!client) 
    {
        printf("Warning: Client fd=%d not found in client list\n", fd);
        return -1;
    }
    
    // Update last activity
    client->last_activity = time(NULL);
    
    int read_result = read_line(fd, line, sizeof(line));
    if (read_result <= 0)
    {
        if (read_result == 0)
            printf("Client fd=%d disconnected\n", fd);
        else
            printf("Error reading from client fd=%d: %s\n", fd, strerror(errno));
        return -1;
    }
        
    // parse HTTP request line
    char method[16], path[256];
    if (sscanf(line, "%15s %255s", method, path) != 2)
    {
        printf("Invalid HTTP request line from fd=%d: %s\n", fd, line);
        send_400(fd, 0);
        return -1;
    }
    
    if (strcmp(method, "GET") != 0)
    {
        // only GET method is supported
        printf("Unsupported method %s from fd=%d\n", method, fd);
        send_400(fd, 0);
        return -1;
    }
    
    // Parse HTTP version from request line
    int http_version = parse_http_version(line);
    
    // Read headers
    int header_len = 0;
    while (read_line(fd, line, sizeof(line)) > 0 && line[0] != '\0')
    {
        // Accumulate headers for parsing
        int line_len = strlen(line);
        if (header_len + line_len + 3 < (int)sizeof(headers)) // +3 for \r\n and null terminator
        {
            strcat(headers, line);
            strcat(headers, "\r\n");
            header_len += line_len + 2;
        }
    }
    
    // Parse connection header based on HTTP version
    int keep_alive = parse_connection_header(headers, http_version);
    
    // Check if we should close due to request limit
    client->request_count++;
    if (client->request_count >= MAX_REQUESTS_PER_CONNECTION)
    {
        printf("Client fd=%d reached max requests (%d), closing connection\n", 
               fd, MAX_REQUESTS_PER_CONNECTION);
        keep_alive = 0;
    }
    
    client->keep_alive = keep_alive;
    
    printf("Serving %s to fd=%d (request #%d, keep_alive=%s)\n", 
           path, fd, client->request_count, keep_alive ? "yes" : "no");
    
    serve_file(fd, path, keep_alive); // serve file or 404
    
    return keep_alive ? 0 : -1; // Return -1 to close connection, 0 to keep alive
}

const char *get_mime(const char *path)
{
    const char *ext = strrchr(path, '.');
    if (!ext)
        return "application/octet-stream";
    if (strcmp(ext, ".html") == 0)
        return "text/html";
    if (strcmp(ext, ".css") == 0)
        return "text/css";
    if (strcmp(ext, ".js") == 0)
        return "application/javascript";
    if (strcmp(ext, ".png") == 0)
        return "image/png";
    if (strcmp(ext, ".jpg") == 0)
        return "image/jpeg";
    return "application/octet-stream";
}

void serve_file(int fd, const char *url_path, int keep_alive)
{
    char requested_path[512];
    snprintf(requested_path, sizeof(requested_path), "%s%s",
             WWW_ROOT, strcmp(url_path, "/") == 0 ? "/index.html" : url_path);

    char resolved_path[512];
    char www_root_resolved[512];
    
    // Get absolute path for both the requested file and www root
    if (!realpath(requested_path, resolved_path) || !realpath(WWW_ROOT, www_root_resolved))
    {
        send_404(fd, keep_alive);
        return;
    }
    
    // Check if the resolved path is within www root directory
    if (strncmp(resolved_path, www_root_resolved, strlen(www_root_resolved)) != 0)
    {
        send_404(fd, keep_alive);
        return;
    }

    int file_fd = open(resolved_path, O_RDONLY);
    if (file_fd < 0)
    {
        send_404(fd, keep_alive);
        return;
    }

    struct stat st;
    fstat(file_fd, &st);
    const char *mime = get_mime(resolved_path);

    // headers
    const char *connection_header = keep_alive ? "keep-alive" : "close";
    dprintf(fd,
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: %s\r\n"
            "Connection: %s\r\n"
            "Keep-Alive: timeout=%d, max=%d\r\n\r\n",
            (size_t)st.st_size, mime, connection_header, 
            KEEP_ALIVE_TIMEOUT, MAX_REQUESTS_PER_CONNECTION);

    // body
    ssize_t r;
    char buf[BUF_SIZE];
    while ((r = read(file_fd, buf, sizeof(buf))) > 0)
    {
        send(fd, buf, r, 0);
    }
    close(file_fd);
}

void send_404(int fd, int keep_alive)
{
    const char *body = "<h1>404 Not Found</h1>";
    const char *connection_header = keep_alive ? "keep-alive" : "close";
    dprintf(fd,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: text/html\r\n"
            "Connection: %s\r\n\r\n"
            "%s",
            strlen(body), connection_header, body);
}

void send_400(int fd, int keep_alive)
{
    const char *body = "<h1>400 Bad Request</h1>";
    const char *connection_header = keep_alive ? "keep-alive" : "close";
    dprintf(fd,
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: text/html\r\n"
            "Connection: %s\r\n\r\n"
            "%s",
            strlen(body), connection_header, body);
}
