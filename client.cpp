// Compile: g++ -std=c++17 client.cpp -o client
// Run: ./client

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
using namespace std;
namespace fs = std::filesystem;

#define BUFSZ 4096

// Send full message to server
bool sendAll(int sock, const char* d, size_t l) {
    size_t s=0; while(s<l){ ssize_t n=send(sock,d+s,l-s,0); if(n<=0)return false; s+=n;} return true;
}

// Read line response
bool recvLine(int sock,string& o){o.clear();char c;while(true){ssize_t n=recv(sock,&c,1,0);if(n<=0)return false;if(c=='\n')break;o+=c;}return true;}

int main() {

    // ----------------- CONNECT TO SERVER -----------------
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in s{};
    s.sin_family = AF_INET; 
    s.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &s.sin_addr);

    if(connect(sock,(sockaddr*)&s,sizeof(s))<0){
        cout<<"❌ Server not available\n";
        return 0;
    }

    // ----------------- LOGIN -----------------
    cout<<"Login (username:password): ";
    string cred; getline(cin,cred);
    cred+="\n";
    sendAll(sock,cred.c_str(),cred.size());

    string line;
    if(!recvLine(sock,line) || line!="AUTH_OK"){
        cout<<"❌ Login failed\n";
        return 0;
    }
    cout<<"✅ Login Success\n";

    fs::create_directories("downloads"); // Make downloads folder

    // ----------------- MENU LOOP -----------------
    while(true) {
        cout<<"\n========== MENU ==========\n"
            <<"1) LIST\n2) GET\n3) PUT\n4) MKDIR\n5) RM\n6) EXIT\n>> ";

        string ch; getline(cin,ch);

        // LIST
        if(ch=="1"){
            sendAll(sock,"LIST\n",5);
            recvLine(sock,line);
            if(line!="OK") continue;

            cout<<"📂 Server Files:\n";
            while(recvLine(sock,line) && line!=".") cout<<line<<endl;
        }

        // GET file
        else if(ch=="2"){
            cout<<"File to download: ";
            string fn; getline(cin,fn);

            string cmd="GET "+fn+"\n"; 
            sendAll(sock,cmd.c_str(),cmd.size());

            recvLine(sock,line);
            if(line.rfind("OK ",0)!=0){
                cout<<line<<endl;
                continue;
            }

            long long sz=stoll(line.substr(3));
            ofstream f("downloads/"+fn,ios::binary);

            long long left=sz;
            char buf[BUFSZ];
            while(left>0){
                int c=recv(sock,buf,min((long long)BUFSZ,left),0);
                f.write(buf,c);
                left-=c;
            }
            cout<<"✅ Downloaded "<<fn<<endl;
        }

        // PUT file
        else if(ch=="3"){
            cout<<"Local file path: ";
            string p; getline(cin,p);
            ifstream f(p,ios::binary);
            if(!f){cout<<"❌ File not found\n";continue;}

            string fn=p.substr(p.find_last_of("/\\")+1);

            f.seekg(0,ios::end); long long sz=f.tellg(); f.seekg(0);
            string cmd="PUT "+fn+" "+to_string(sz)+"\n";
            sendAll(sock,cmd.c_str(),cmd.size());

            recvLine(sock,line);
            if(line!="OK READY"){cout<<line;continue;}

            char buf[BUFSZ];
            while(f){f.read(buf,BUFSZ); sendAll(sock,buf,f.gcount());}

            recvLine(sock,line);
            cout<<line<<endl;
        }

        // Create folder on server
        else if(ch=="4"){
            cout<<"Folder name: "; string d; getline(cin,d);
            string cmd="MKDIR "+d+"\n";
            sendAll(sock,cmd.c_str(),cmd.size());
            recvLine(sock,line); cout<<line<<endl;
        }

        // Delete file on server
        else if(ch=="5"){
            cout<<"File/Folder to delete: "; string d; getline(cin,d);
            string cmd="RM "+d+"\n";
            sendAll(sock,cmd.c_str(),cmd.size());
            recvLine(sock,line); cout<<line<<endl;
        }

        // EXIT
        else if(ch=="6"){
            sendAll(sock,"EXIT\n",5);
            break;
        }
    }

    close(sock);
    return 0;
}
