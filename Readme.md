🚀 Network File Sharing System — C++ (Client-Server)

A secure, multi-client TCP File Sharing System built in C++ using Raw Socket Programming.
Supports authentication, file upload/download, directory management & logging — like a mini FTP server.

📋 Table of Contents

📁 Features

🧠 Architecture

📂 Folder Structure

⚙️ Setup & Compilation

🔑 Login Credentials

🧭 Usage Commands

🖥️ Sample Output

📚 Learning Outcomes

🚀 Future Enhancements

👨‍💻 Author

📁 Features
Feature	Description
🔐 Login authentication	Credentials stored in users.txt
📂 List directory contents	View server files & folders
⬇️ Download files	Client can GET files
⬆️ Upload files	Client can PUT files
📁 Create folders	MKDIR support
🗑 Remove files	RM command
⚙ Multi-client	Threaded server
🧾 Logging	Logs to logs/server.log + console
🛠️ Platform	Works on Linux & Windows
🧠 Architecture
+-----------+         TCP/IP Socket         +-----------+
|   Client  |  <--------------------------> |   Server  |
|-----------|                               |-----------|
| Login     |                               | User Auth |
| Commands  |                               | File Ops  |
| File I/O  |                               | Logging   |
+-----------+                               +-----------+

📂 Folder Structure
project/
 ├── server.cpp
 ├── client.cpp
 ├── users.txt
 ├── server_files/
 ├── downloads/
 └── logs/

⚙️ Setup & Compilation
✅ Linux / macOS / WSL

Server

g++ -std=c++17 server.cpp -o server -lpthread
./server


Client

g++ -std=c++17 client.cpp -o client
./client

🪟 Windows (MinGW / CMD / PowerShell)

Server

g++ -std=c++17 server.cpp -o server.exe -lws2_32 -lpthread
.\server.exe


Client

g++ -std=c++17 client.cpp -o client.exe -lws2_32
.\client.exe

🔑 Login Credentials

users.txt

admin:1234:admin
user:pass:user


Login from client:

admin:1234

🧭 Usage Commands
Action	Client Menu
📂 List files	1
⬇ Download file	2
⬆ Upload file	3
📁 Create directory	4
🗑 Delete file/folder	5
🚪 Exit	6
🖥️ Sample Output
✅ Server Console
✅ Server running on port 8080
LOGIN OK: admin
admin GET welcome.txt
admin PUT report.pdf
DISCONNECT: admin

✅ Client Console
Login (username:password): admin:1234
✅ Login Success

1) LIST
2) GET
3) PUT
4) MKDIR
5) RM
6) EXIT

📂 Files:
welcome.txt
images/
video.mp4

🎯 Learning Outcomes

TCP/IP socket programming

Multi-threaded system design

File I/O operations

Client-Server authentication

Logging and directory management

Real-world networking concepts

🚀 Future Enhancements
Enhancement	Purpose
🔒 TLS/SSL Encryption	Secure file transfer
🧠 SQLite user DB	Replace users.txt
🔁 Resume broken transfers	Like real FTP clients
📦 File Sync Client	Auto sync like Dropbox
🖥 Web or GUI Dashboard	React / Qt / Tkinter frontend
👨‍💻 Author

Anirban Sarangi
📧 your email
📌 Capstone Project — Linux & Socket Programming
🗓️ 2025