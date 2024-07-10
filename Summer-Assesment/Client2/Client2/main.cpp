#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>

using namespace std;

#pragma comment(lib ,"ws2_32.lib")


/*
     Initialize Winsock

     Creat Socket

     Connect to the server

     Send/Recieve

     close the socket
*/

bool Initialize() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void SendMsg(SOCKET s) {
    cout << "Enter Your Chat Name : " << endl;
    string name, message;
    getline(cin, name);

    while (1) {
        getline(cin, message);
        string msg = name + " : " + message;
        int bytesend = send(s, msg.c_str(), msg.length(), 0);
        if (bytesend == SOCKET_ERROR) {
            cout << "Error while sending message" << endl;
            break;
        }
        if (message == "quit") {
            cout << "Stopping the program" << endl;
            break;
        }
    }
    closesocket(s);
    WSACleanup();
}

void ReceiveMsg(SOCKET s) {
    char buffer[4096];
    int recvlength;
    string msg = "";

    while (1) {
        recvlength = recv(s, buffer, sizeof(buffer) - 1, 0);
        if (recvlength <=  0){
            cout << "Error While recieving message . Error is : " << WSAGetLastError() << endl;
            break;
        }
        else {
            buffer[recvlength] = '\0';
            msg = string(buffer, recvlength);
            cout << msg << endl;
        }

    }
    closesocket(s);
    WSACleanup();
}


int main() {
    if (!Initialize()) {
        cout << "Initialize Winsock Failed" << endl;
        return 1;
    }

    SOCKET s;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Invalid Socked Created" << endl;
        return 1;
    }

    int port = 12345;
    string serveraddress = "127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

    //  Connecting...
    if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        cout << "Not able to connect to server" << endl;
        cout << ": " << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout << "Successfully connected to server" << endl;

    thread sendThread(SendMsg, s);
    thread recieverThread(ReceiveMsg, s);

    sendThread.join();
    recieverThread.join();
    return 0;
}