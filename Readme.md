
# 🚀 Network File Sharing System — C++ (Client ↔ Server)

A lightweight, multi-client TCP file sharing system in C++ (POSIX sockets).  
Features authentication, upload/download, directory management and logging — a minimal FTP-like server for learning and prototyping.

---

## Table of Contents
- Features
- Architecture
- Requirements
- Folder structure
- Build & Run
- Configuration (users)
- Client commands / Protocol
- Examples
- Troubleshooting
- Future enhancements
- Author

---

## Features
- Authentication via users.txt (username:password:role)
- LIST / GET / PUT / MKDIR / RM commands
- Threaded: handles multiple clients
- File I/O and chunked transfers
- Logging to `logs/server.log` and console
- Cross-platform: Linux / macOS / WSL / MinGW (Windows)

---

## Architecture
Client ↔ TCP ↔ Server

Server responsibilities:
- Authenticate users
- Serve directory listings
- Send/receive files (binary-safe)
- Create/remove directories/files
- Log events

---

## Requirements
- g++ (C++17) or equivalent
- pthreads (Linux/WSL/macOS)
- On Windows with MinGW: link ws2_32 and adapt sockets if needed
- C++ standard library filesystem support

---

## Folder structure
project/
- server.cpp
- client.cpp
- users.txt
- server_files/    (served files)
- downloads/       (client default)
- logs/            (server.log)

---

## Build & Run

Linux / macOS / WSL
```sh
# Server
g++ -std=c++17 server.cpp -o server -lpthread
./server

# Client
g++ -std=c++17 client.cpp -o client
./client
```

Windows (MinGW)
```ps1
# Server (may require winsock init in client/server)
g++ -std=c++17 server.cpp -o server.exe -lws2_32 -lpthread
.\server.exe

g++ -std=c++17 client.cpp -o client.exe -lws2_32
.\client.exe
```

Notes:
- Use WSL for POSIX sockets to avoid Windows API changes.
- Server listens on port 8080 by default (see `PORT` in server.cpp).

---

## Configuration — users.txt
Format per line:
```
username:password:role
```
Example (workspace)
```
admin:1234:admin
omm:12343:user
abhi:56787:user
```
Clients send `username:password` on connect for authentication.

---

## Client Commands / Protocol (line-based)
- AUTH: client sends `user:pass\n` → server replies `AUTH_OK\n` or `AUTH_FAIL\n`
- LIST: `LIST\n` → `OK\n` + file list ending with `.\n`
- GET: `GET filename\n` → `OK <size>\n` then <size> bytes (binary)
- PUT: `PUT filename size\n` → server `OK READY\n` then client sends <size> bytes → server responds `OK SAVED\n`
- MKDIR: `MKDIR name\n` → `OK MKDIR\n`
- RM: `RM path\n` → `OK RM\n`
- EXIT: `EXIT\n` → disconnect

Error responses: prefixed with `ERR` (for example `ERR NOT_FOUND\n`).

---

## Examples

1) List files
Client: `LIST\n`  
Server: `OK\nwelcome.txt\nimages/\n.\n`

2) Download file
Client: `GET welcome.txt\n`  
Server: `OK 1234\n` followed by 1234 raw bytes

3) Upload file
Client: `PUT report.pdf 204800\n`  
Server: `OK READY\n` then client streams 204800 bytes

---

## Troubleshooting
- "Permission denied" — ensure `server_files/` exists and server has write permissions.
- Partial transfers — make sure client/server read/write loop handles returned byte counts.
- Windows socket errors — prefer WSL or adapt code for Winsock (WSAStartup, closesocket, etc.).

---

## Future enhancements
- TLS/SSL for encrypted transport
- Hashed passwords / SQLite user DB
- Resume interrupted transfers
- CLI improvements and progress reporting
- GUI or web-based client

---

## Author
Anirban Sarangi — Capstone Project (2025)  
Contact: your email

---
```<!-- filepath: d:\networkFileSharing\Readme.md -->
