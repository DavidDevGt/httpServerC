CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Isrc
TARGET = build/httpserver
SRCDIR = src
BUILDDIR = build

SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/core/server.c \
          $(SRCDIR)/core/event_loop.c \
          $(SRCDIR)/client/client_manager.c \
          $(SRCDIR)/http/http_handler.c

OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR) $(BUILDDIR)/core $(BUILDDIR)/client $(BUILDDIR)/http

clean:
	rm -rf $(BUILDDIR)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@echo "Starting HTTP server with keep-alive support..."
	@echo "Server will be available at http://localhost:6090"
	@echo "Press Ctrl+C to stop the server"
	./$(TARGET)

test-keepalive: 
	@echo "Testing keep-alive functionality..."
	@echo "=== Single request ==="
	curl -v -H "Connection: keep-alive" http://localhost:6090/ 2>&1 | grep -E "(Connection|Keep-Alive|HTTP)"
	@echo "\n=== Multiple requests on same connection ==="
	curl -v --http1.1 -H "Connection: keep-alive" \
		http://localhost:6090/ \
		http://localhost:6090/ \
		http://localhost:6090/ 2>&1 | grep -E "(Connection|Keep-Alive|HTTP|Re-using)"

help:
	@echo "Available targets:"
	@echo "  all           - Compile the server"
	@echo "  clean         - Remove compiled files"
	@echo "  run           - Compile and run the server"
	@echo "  test          - Same as run"
	@echo "  test-keepalive - Test keep-alive with curl (server must be running)"
	@echo "  help          - Show this help"
