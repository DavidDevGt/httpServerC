let testRunning = false;

function log(message, type = 'info') {
    const output = document.getElementById('output');
    const timestamp = new Date().toLocaleTimeString();
    let color = '#f0f6fc';
    let icon = '•';

    switch (type) {
        case 'success':
            color = '#22c55e';
            icon = '✓';
            break;
        case 'error':
            color = '#ef4444';
            icon = '✗';
            break;
        case 'warning':
            color = '#f59e0b';
            icon = '⚠';
            break;
        case 'info':
            color = '#58a6ff';
            icon = 'ℹ';
            break;
        case 'request':
            color = '#a855f7';
            icon = '→';
            break;
        case 'response':
            color = '#06b6d4';
            icon = '←';
            break;
    }

    output.innerHTML += `<span style="color: #6b7280;">[${timestamp}]</span> <span style="color: ${color};">${icon} ${message}</span>\n`;
    output.scrollTop = output.scrollHeight;
}

function formatHeaders(headers) {
    let formatted = '';
    for (let [key, value] of Object.entries(headers)) {
        formatted += `    ${key}: ${value}\n`;
    }
    return formatted;
}

async function testKeepAlive() {
    if (testRunning) return;
    testRunning = true;

    log('=== KEEP-ALIVE CONNECTION TEST ===', 'info');
    log('Testing persistent connection functionality...', 'info');

    try {
        const start = performance.now();
        log('→ GET / HTTP/1.1', 'request');

        const response = await fetch('/', {
            method: 'GET',
            headers: {
                'Connection': 'keep-alive'
            }
        });

        const end = performance.now();
        const headers = {};
        for (let [key, value] of response.headers.entries()) {
            headers[key] = value;
        }

        log(`← ${response.status} ${response.statusText} (${Math.round(end - start)}ms)`, 'response');
        log('Response Headers:', 'info');
        log(formatHeaders(headers));

        if (headers.connection && headers.connection.toLowerCase().includes('keep-alive')) {
            log('✓ Keep-Alive connection established successfully!', 'success');
        } else {
            log('⚠ Connection header not indicating keep-alive', 'warning');
        }

        if (headers['keep-alive']) {
            log(`✓ Keep-Alive parameters: ${headers['keep-alive']}`, 'success');
        }

    } catch (error) {
        log(`✗ Connection failed: ${error.message}`, 'error');
    }

    testRunning = false;
}

async function testMultipleRequests() {
    if (testRunning) return;
    testRunning = true;

    log('=== MULTIPLE REQUESTS TEST ===', 'info');
    log('Testing connection reuse with multiple sequential requests...', 'info');

    const requests = 5;
    const start = performance.now();
    let successCount = 0;

    for (let i = 1; i <= requests; i++) {
        try {
            log(`→ Request ${i}/5`, 'request');
            const reqStart = performance.now();

            const response = await fetch('/', {
                method: 'GET',
                headers: {
                    'Connection': 'keep-alive'
                }
            });

            const reqTime = performance.now() - reqStart;

            if (response.ok) {
                log(`← ${response.status} (${Math.round(reqTime)}ms)`, 'response');
                successCount++;
            } else {
                log(`← ${response.status} ${response.statusText}`, 'error');
            }

            // Small delay between requests
            await new Promise(resolve => setTimeout(resolve, 100));

        } catch (error) {
            log(`✗ Request ${i} failed: ${error.message}`, 'error');
        }
    }

    const totalTime = performance.now() - start;
    log(`\n📊 Test Results:`, 'info');
    log(`   • Successful requests: ${successCount}/${requests}`, 'success');
    log(`   • Total time: ${Math.round(totalTime)}ms`, 'info');
    log(`   • Average per request: ${Math.round(totalTime / requests)}ms`, 'info');
    log(`   • Success rate: ${Math.round((successCount / requests) * 100)}%`, successCount === requests ? 'success' : 'warning');

    testRunning = false;
}

async function testPerformance() {
    if (testRunning) return;
    testRunning = true;

    log('=== PERFORMANCE BENCHMARK ===', 'info');
    log('Running performance test with concurrent requests...', 'info');

    const concurrentRequests = 10;
    const start = performance.now();

    try {
        const promises = Array.from({ length: concurrentRequests }, (_, i) =>
            fetch('/', {
                method: 'GET',
                headers: {
                    'Connection': 'keep-alive',
                    'X-Request-ID': `test-${i + 1}`
                }
            }).then(response => ({
                id: i + 1,
                status: response.status,
                ok: response.ok,
                time: performance.now()
            })).catch(error => ({
                id: i + 1,
                error: error.message,
                time: performance.now()
            }))
        );

        log(`→ Launching ${concurrentRequests} concurrent requests...`, 'request');
        const results = await Promise.all(promises);
        const totalTime = performance.now() - start;

        const successful = results.filter(r => r.ok).length;
        const failed = results.filter(r => r.error).length;

        log(`← All requests completed in ${Math.round(totalTime)}ms`, 'response');
        log(`\n📊 Performance Results:`, 'info');
        log(`   • Concurrent requests: ${concurrentRequests}`, 'info');
        log(`   • Successful: ${successful}`, 'success');
        log(`   • Failed: ${failed}`, failed > 0 ? 'error' : 'success');
        log(`   • Total time: ${Math.round(totalTime)}ms`, 'info');
        log(`   • Requests/second: ${Math.round((concurrentRequests / totalTime) * 1000)}`, 'info');
        log(`   • Average latency: ${Math.round(totalTime / concurrentRequests)}ms`, 'info');

    } catch (error) {
        log(`✗ Performance test failed: ${error.message}`, 'error');
    }

    testRunning = false;
}

async function testHeaders() {
    if (testRunning) return;
    testRunning = true;

    log('=== HEADER INSPECTION TEST ===', 'info');
    log('Inspecting all response headers in detail...', 'info');

    try {
        log('→ GET / HTTP/1.1 (detailed headers)', 'request');

        const response = await fetch('/', {
            method: 'GET',
            headers: {
                'User-Agent': 'HTTP-Server-C-Tester/1.0',
                'Accept': 'text/html,application/xhtml+xml',
                'Connection': 'keep-alive',
                'Cache-Control': 'no-cache'
            }
        });

        log(`← ${response.status} ${response.statusText}`, 'response');
        log('\n📋 Complete Response Headers:', 'info');

        const headers = {};
        for (let [key, value] of response.headers.entries()) {
            headers[key] = value;
            log(`   ${key}: ${value}`, 'info');
        }

        log('\n🔍 Header Analysis:', 'info');

        // Analyze specific headers
        if (headers['content-type']) {
            log(`✓ Content-Type: ${headers['content-type']}`, 'success');
        }

        if (headers['content-length']) {
            log(`✓ Content-Length: ${headers['content-length']} bytes`, 'success');
        }

        if (headers['connection']) {
            const isKeepAlive = headers['connection'].toLowerCase().includes('keep-alive');
            log(`${isKeepAlive ? '✓' : '⚠'} Connection: ${headers['connection']}`, isKeepAlive ? 'success' : 'warning');
        }

        if (headers['server']) {
            log(`✓ Server: ${headers['server']}`, 'success');
        } else {
            log(`⚠ No Server header present`, 'warning');
        }

        if (headers['date']) {
            log(`✓ Date: ${headers['date']}`, 'success');
        }

    } catch (error) {
        log(`✗ Header inspection failed: ${error.message}`, 'error');
    }

    testRunning = false;
}

function clearOutput() {
    const output = document.getElementById('output');
    output.innerHTML = `<span style="color: #58a6ff;"># HTTP Server C - Testing Console</span>
<span style="color: #7c3aed;">## Console cleared - Ready for new tests</span>
<span style="color: #22c55e;">✓ Server running on port 6090</span>
<span style="color: #f59e0b;">⚡ Click any test button to start</span>`;
}

// Auto-run initial test on page load
window.addEventListener('load', () => {
    setTimeout(() => {
        log('🚀 Auto-running initial keep-alive test...', 'info');
        testKeepAlive();
    }, 1500);
});

// Add some visual feedback for running tests
function setButtonState(running) {
    const buttons = document.querySelectorAll('.btn-test');
    buttons.forEach(btn => {
        if (running) {
            btn.disabled = true;
            btn.style.opacity = '0.6';
        } else {
            btn.disabled = false;
            btn.style.opacity = '1';
        }
    });
}

// Update the test functions to show loading state
const originalTestRunning = testRunning;
Object.defineProperty(window, 'testRunning', {
    get: () => originalTestRunning,
    set: (value) => {
        originalTestRunning = value;
        setButtonState(value);
    }
});