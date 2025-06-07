#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#define BUF_SIZE 4096

// HTTP handling functions
int read_line(int fd, char *buf, int max);
int handle_client(int fd);
const char *get_mime(const char *path);
void serve_file(int fd, const char *url_path, int keep_alive);
void send_404(int fd, int keep_alive);
void send_400(int fd, int keep_alive);

// HTTP parsing functions
int parse_http_version(const char *request_line);
int parse_connection_header(const char *headers, int http_version);

#endif // HTTP_HANDLER_H
