# Contributing to HTTP Server C

Thank you for your interest in contributing to this project! This document provides guidelines and information for contributors.

## 🚀 Getting Started

### Prerequisites

- GCC compiler (version 7.0 or higher recommended)
- Make build system
- Basic knowledge of C programming
- Understanding of HTTP protocol basics
- Linux/Unix development environment

### Setting up the Development Environment

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/httpServerC.git
   cd httpServerC
   ```
3. **Build the project**:
   ```bash
   make clean && make
   ```
4. **Test the build**:
   ```bash
   ./build/httpserver
   ```

## 📋 Development Guidelines

### Code Style

- **Indentation**: 4 spaces (no tabs)
- **Line Length**: Maximum 80 characters
- **Naming Convention**: 
  - Functions: `snake_case`
  - Variables: `snake_case`
  - Constants: `UPPER_CASE`
  - Structs: `PascalCase`
- **Comments**: Use `//` for single-line, `/* */` for multi-line
- **Header Guards**: Use `#ifndef FILENAME_H` format

### Project Structure

```
src/
├── main.c              # Entry point - minimal, delegates to core
├── core/               # Core server functionality
│   ├── server.c/.h     # Main server setup, socket management
│   └── event_loop.c/.h # I/O multiplexing, event handling
├── http/               # HTTP protocol implementation
│   └── http_handler.c/.h # Request parsing, response generation
└── client/             # Client connection management
    └── client_manager.c/.h # Connection lifecycle, keep-alive
```

### Coding Standards

1. **Error Handling**: Always check return values and handle errors gracefully
2. **Memory Management**: Clean up all allocated memory, check for leaks
3. **Thread Safety**: Document any thread-unsafe functions
4. **Documentation**: Comment complex algorithms and business logic
5. **Testing**: Add tests for new functionality

### Example Code Style

```c
/**
 * Parse HTTP request line
 * @param request_line: Raw HTTP request line
 * @param method: Output buffer for HTTP method
 * @param path: Output buffer for request path
 * @param version: Output buffer for HTTP version
 * @return: 0 on success, -1 on error
 */
int parse_request_line(const char *request_line, char *method, 
                      char *path, char *version) {
    if (!request_line || !method || !path || !version) {
        return -1;
    }
    
    // Implementation here...
    return 0;
}
```

## 🔧 Types of Contributions

### Bug Fixes
- Fix compilation errors or warnings
- Address memory leaks or segmentation faults
- Correct HTTP protocol violations
- Improve error handling

### Features
- Add new HTTP methods support
- Implement additional MIME types
- Enhance logging capabilities
- Add configuration file support
- Improve performance optimizations

### Documentation
- Improve code comments
- Add usage examples
- Create technical documentation
- Update README with new features

### Testing
- Add unit tests
- Create integration tests
- Performance benchmarking
- Security testing

## 🐛 Reporting Issues

When reporting bugs, please include:

1. **Environment**: OS, compiler version, architecture
2. **Reproduction Steps**: Clear steps to reproduce the issue
3. **Expected Behavior**: What should happen
4. **Actual Behavior**: What actually happens
5. **Logs**: Relevant error messages or server logs
6. **Configuration**: Any custom settings or modifications

## 📝 Pull Request Process

1. **Create a Feature Branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make Changes**: Follow the coding standards above

3. **Test Your Changes**:
   ```bash
   make clean && make
   ./build/httpserver
   # Test functionality manually and with the web console
   ```

4. **Commit Changes**:
   ```bash
   git add .
   git commit -m "feat: add support for HTTP POST method"
   ```

5. **Push to Your Fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create Pull Request**: 
   - Provide clear description of changes
   - Reference any related issues
   - Include testing instructions

### Commit Message Convention

Use conventional commits format:

- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation changes
- `style:` Code style changes (formatting, etc.)
- `refactor:` Code refactoring
- `test:` Adding or updating tests
- `chore:` Maintenance tasks

Examples:
```
feat: add HTTP POST method support
fix: resolve memory leak in client_manager
docs: update API documentation
refactor: simplify request parsing logic
```

## 🧪 Testing

### Manual Testing

1. **Basic Functionality**:
   ```bash
   curl -v http://localhost:6090/
   ```

2. **Keep-Alive Testing**:
   ```bash
   curl -v -H "Connection: keep-alive" http://localhost:6090/
   ```

3. **Web Console**: Use the built-in testing interface at `http://localhost:6090`

### Adding Tests

When adding new features, consider:
- Unit tests for individual functions
- Integration tests for complete workflows
- Performance tests for optimization changes
- Security tests for input validation

## 🏆 Recognition

Contributors will be recognized in:
- README.md acknowledgments
- Git commit history
- Release notes for significant contributions

## 📞 Getting Help

- **Issues**: Use GitHub Issues for bugs and feature requests
- **Discussions**: Use GitHub Discussions for questions and ideas
- **Code Review**: Maintainers will review all pull requests

## 📚 Learning Resources

- [HTTP/1.1 RFC 7230-7237](https://tools.ietf.org/html/rfc7230)
- [POSIX Socket Programming](https://pubs.opengroup.org/onlinepubs/9699919799/functions/socket.html)
- [C Programming Best Practices](https://www.kernel.org/doc/html/v4.10/process/coding-style.html)
- [Select System Call](https://man7.org/linux/man-pages/man2/select.2.html)

Thank you for contributing! 🎉
