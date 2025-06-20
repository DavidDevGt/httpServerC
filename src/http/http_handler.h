/**
 * @file http_handler.h
 * @brief HTTP Server - HTTP Protocol Handler Interface
 * @version 1.0.0
 * @date 2025-06-07
 * @author David Dev (@DavidDevGt)
 * 
 * @description
 * HTTP protocol handling interface providing request parsing, response generation,
 * and file serving capabilities. Supports HTTP/1.1 with keep-alive connections
 * and proper MIME type handling.
 * 
 * Features:
 * - HTTP/1.1 and HTTP/1.0 support
 * - Keep-alive connection management
 * - MIME type detection
 * - Path traversal protection
 * - Error response handling
 * 
 * @license MIT License
 */

#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#define BUF_SIZE 4096

int read_line(int fd, char *buf, int max);
int handle_client(int fd);
const char *get_mime(const char *path);
void serve_file(int fd, const char *url_path, int keep_alive);
void send_404(int fd, int keep_alive);
void send_400(int fd, int keep_alive);
int parse_http_version(const char *request_line);
int parse_connection_header(const char *headers, int http_version);

#endif // HTTP_HANDLER_H
