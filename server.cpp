// Compile Command:  g++ -std=c++17 server.cpp -o server -lpthread
// Run: ./server

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <map>
#include <mutex>
using namespace std;
namespace fs = std::filesystem;

#define PORT 8080
#define BUFSZ 4096

mutex log_lock; // For safe multi-thread logging

//--------------------------------------
// Log message to file + print to console
//--------------------------------------
void log_event(string msg) {
    lock_guard<mutex> guard(log_lock);
    fs::create_directories("logs"); // Ensure log folder exists
    ofstream log("logs/server.log", ios::app);
    log << msg << endl;
    cout << msg << endl;
}

//--------------------------------------
// Read users from users.txt file
// Format: username:password:role
//--------------------------------------
map<string, pair<string,string>> load_users() {
    map<string, pair<string,string>> users;
    ifstream f("users.txt");
    string line;

    while(getline(f,line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        string user, pass, role;
        getline(ss, user, ':');
        getline(ss, pass, ':');
        getline(ss, role, ':');
        users[user] = {pass, role};
    }
    return users;
}

//--------------------------------------
// Send all bytes to client
//--------------------------------------
bool sendAll(int sock, const char* data, size_t len) {
    size_t sent = 0;
    while(sent < len) {
        ssize_t n = send(sock, data + sent, len - sent, 0);
        if(n <= 0) return false;
        sent += n;
    }
    return true;
}

//--------------------------------------
// Receive text line from client
//--------------------------------------
bool recvLine(int sock, string &out) {
    out.clear();
    char c;
    while(true) {
        ssize_t n = recv(sock, &c, 1, 0);
        if(n <= 0) return false;
        if(c == '\n') break;
        out += c;
    }
    return true;
}

//--------------------------------------
// Authenticate client login
//--------------------------------------
bool authenticate(int client, string &username) {
    string creds;
    recvLine(client, creds);  // Receive "user:pass"

    auto pos = creds.find(':');
    username = creds.substr(0, pos);
    string password = creds.substr(pos+1);

    auto users = load_users();
    if(users.count(username) && users[username].first == password) {
        sendAll(client, "AUTH_OK\n", 8);
        log_event("LOGIN OK: " + username);
        return true;
    }
    sendAll(client, "AUTH_FAIL\n", 10);
    log_event("LOGIN FAIL: " + username);
    return false;
}

//--------------------------------------
// List files & folders in server directory
//--------------------------------------
string list_files(string root) {
    stringstream ss;
    for(auto &p : fs::directory_iterator(root)) {
        ss << (p.is_directory() ? "[DIR] " : "      ")
           << p.path().filename().string() << "\n";
    }
    return ss.str();
}

//--------------------------------------
// Send file to client (Download)
//--------------------------------------
void sendFile(int client, string path) {
    ifstream f(path, ios::binary);
    if(!f) {
        sendAll(client, "ERR NOT_FOUND\n", 14);
        return;
    }

    // Calculate file size
    f.seekg(0, ios::end);
    long long size = f.tellg();
    f.seekg(0);

    // Send header "OK filesize"
    string header = "OK " + to_string(size) + "\n";
    sendAll(client, header.c_str(), header.size());

    // Send file content
    char buffer[BUFSZ];
    while(f) {
        f.read(buffer, BUFSZ);
        sendAll(client, buffer, f.gcount());
    }
}

//--------------------------------------
// Receive file from client (Upload)
//--------------------------------------
void recvFile(int client, string path, long long size) {
    ofstream f(path, ios::binary);
    char buffer[BUFSZ];
    long long left = size;

    // Receive file chunk-by-chunk
    while(left > 0) {
        int n = recv(client, buffer, min((long long)BUFSZ, left), 0);
        f.write(buffer, n);
        left -= n;
    }
}

//--------------------------------------
// Handle each connected client
//--------------------------------------
void clientThread(int client) {
    string user;
    if(!authenticate(client, user)) { close(client); return; }

    string root = "server_files/";
    string cmd;

    while(recvLine(client, cmd)) {

        if(cmd == "LIST") {
            string out = "OK\n" + list_files(root) + ".\n";
            sendAll(client, out.c_str(), out.size());
        }

        else if(cmd.rfind("GET ",0)==0) {
            string fn = cmd.substr(4);
            log_event(user + " GET " + fn);
            sendFile(client, root + fn);
        }

        else if(cmd.rfind("PUT ",0)==0) {
            stringstream ss(cmd.substr(4));
            string fn; long long size;
            ss >> fn >> size;

            sendAll(client, "OK READY\n", 9);
            log_event(user + " UPLOAD " + fn);

            recvFile(client, root + fn, size);
            sendAll(client, "OK SAVED\n", 9);
        }

        else if(cmd.rfind("MKDIR ",0)==0) {
            fs::create_directories(root + cmd.substr(6));
            sendAll(client, "OK MKDIR\n", 9);
        }

        else if(cmd.rfind("RM ",0)==0) {
            fs::remove_all(root + cmd.substr(3));
            sendAll(client, "OK RM\n", 6);
        }

        else if(cmd == "EXIT") break;
    }

    close(client);
    log_event("DISCONNECT: " + user);
}

//--------------------------------------
// MAIN SERVER FUNCTION
//--------------------------------------
int main() {
    fs::create_directories("server_files");
    fs::create_directories("logs");

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 5);

    cout << "✅ Server running on port " << PORT << endl;

    while(true) {
        sockaddr_in client{};
        socklen_t len = sizeof(client);
        int c = accept(server, (sockaddr*)&client, &len);

        // Handle client in new thread
        thread(clientThread, c).detach();
    }
}
