<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1" />
  <title>Network File Sharing System (TLS) — Readme</title>
  <style>
    body { font-family: system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial; margin: 24px; color:#111; background:#f7f8fb; }
    .container { max-width:980px; margin:0 auto; background:#fff; padding:28px; border-radius:8px; box-shadow:0 6px 22px rgba(22,27,34,0.06); }
    h1 { margin-top:0; }
    .meta { color:#5b6470; font-size:0.95rem; margin-bottom:8px; }
    table { border-collapse:collapse; width:100%; margin:12px 0; }
    th, td { text-align:left; padding:8px; border-bottom:1px solid #eef1f6; }
    .code { background:#0b1220; color:#dbeafe; padding:12px; border-radius:6px; font-family: monospace; overflow:auto; font-size:0.95rem; }
    pre.code { margin:8px 0; }
    .grid { display:flex; gap:12px; flex-wrap:wrap; }
    .card { background:#f3f6fb; padding:12px; border-radius:6px; flex:1 1 260px; }
    ul { margin:8px 0 16px 20px; }
    .badge { display:inline-block; background:#eef2ff; color:#3730a3; padding:4px 8px; border-radius:999px; font-size:0.85rem; margin-right:6px; }
    .footer { color:#6b7280; font-size:0.9rem; margin-top:18px; border-top:1px solid #eef1f6; padding-top:12px; }
    .mono { font-family: monospace; background:#f8fafc; padding:4px 6px; border-radius:4px; }
  </style>
</head>
<body>
  <div class="container">
    <h1>🔐 Network File Sharing System with TLS Encryption</h1>
    <div class="meta">C++17 • TCP sockets • OpenSSL (TLS) • Multi-client • Cross-platform</div>

    <p>A secure, multi-client file sharing system implemented in C++ using TCP sockets and OpenSSL for TLS. Supports authentication, file upload/download, directory operations and audit logs.</p>

    <h2>Features</h2>
    <table>
      <tr><th>Category</th><th>Features</th></tr>
      <tr><td>Security</td><td>TLS encrypted communication (OpenSSL)</td></tr>
      <tr><td>Authentication</td><td>Username/password from <span class="mono">users.txt</span></td></tr>
      <tr><td>File Ops</td><td>LIST, GET, PUT, MKDIR, RM</td></tr>
      <tr><td>Logging</td><td>Logs every action in <span class="mono">logs/server.log</span></td></tr>
      <tr><td>Concurrency</td><td>Multi-threaded server</td></tr>
      <tr><td>Protocol</td><td>Custom text command protocol over TLS</td></tr>
      <tr><td>OS Support</td><td>Linux, Windows (MinGW/MSYS2)</td></tr>
    </table>

    <h2>Project Structure</h2>
    <pre class="code">project/
├─ server_tls.cpp
├─ client_tls.cpp
├─ server.crt
├─ server.key
├─ users.txt
├─ server_files/
├─ downloads/
└─ logs/</pre>

    <h2>Credentials (edit users.txt)</h2>
    <pre class="code">admin:1234:admin
user:pass:user</pre>

    <h2>Install Dependencies</h2>
    <div class="grid">
      <div class="card">
        <strong>Debian / Ubuntu / WSL</strong>
        <pre class="code">sudo apt update
sudo apt install -y build-essential pkg-config libssl-dev</pre>
      </div>
      <div class="card">
        <strong>Fedora / RHEL</strong>
        <pre class="code">sudo dnf install openssl-devel gcc-c++ pkgconf
# or
sudo yum install openssl-devel gcc-c++ pkgconfig</pre>
      </div>
      <div class="card">
        <strong>Windows (MSYS2 / MinGW64)</strong>
        <pre class="code">pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-openssl</pre>
      </div>
    </div>

    <h2>Generate TLS Certificate</h2>
    <pre class="code">openssl req -x509 -nodes -newkey rsa:2048 \
  -keyout server.key -out server.crt -days 365 \
  -subj "/CN=localhost"</pre>
    <p class="meta">Copy <span class="mono">server.crt</span> to the client folder so the client can validate the server certificate.</p>

    <h2>Build & Run</h2>
    <h3>Linux / WSL</h3>
    <pre class="code"># Server
g++ -std=c++17 server_tls.cpp -o server_tls -lssl -lcrypto -lpthread
./server_tls

# Client
g++ -std=c++17 client_tls.cpp -o client_tls -lssl -lcrypto
./client_tls</pre>

    <h3>Windows (MSYS2 MinGW64)</h3>
    <pre class="code"># Server
g++ -std=c++17 server_tls.cpp -o server_tls.exe -lssl -lcrypto -lws2_32 -lpthread
./server_tls.exe

# Client
g++ -std=c++17 client_tls.cpp -o client_tls.exe -lssl -lcrypto -lws2_32
./client_tls.exe</pre>

    <h2>Client Commands</h2>
    <table>
      <tr><th>Menu</th><th>Action</th></tr>
      <tr><td>1</td><td>LIST files on server</td></tr>
      <tr><td>2</td><td>GET (download file)</td></tr>
      <tr><td>3</td><td>PUT (upload file)</td></tr>
      <tr><td>4</td><td>MKDIR (create folder)</td></tr>
      <tr><td>5</td><td>RM (delete file/folder)</td></tr>
      <tr><td>6</td><td>EXIT connection</td></tr>
    </table>

    <h2>Security Notes</h2>
    <ul>
      <li>TLS handshake secures the channel.</li>
      <li>Certificate validated by client (self-signed for dev). For production, use a real CA and revocation.</li>
      <li>Consider per-file encryption (AES) and hardened auth for production.</li>
    </ul>

    <h2>Testing Checklist</h2>
    <pre class="code">Connect multiple clients ✅
Login auth ✅
Upload / download files ✅
View server logs ✅
TLS encrypted channel ✅</pre>

    <h2>Future Enhancements</h2>
    <ul>
      <li>GUI Client (Qt / Python Tkinter)</li>
      <li>Resume broken downloads</li>
      <li>SQLite for user DB</li>
      <li>AES-256 per-file encryption</li>
      <li>JWT authentication</li>
      <li>Cloud deployment (AWS EC2)</li>
    </ul>

    <div class="footer">
      <strong>Author</strong><br/>
      Anirban Sarangi — Capstone Project: Network & System Programming
    </div>
  </div>
</body>
</html>