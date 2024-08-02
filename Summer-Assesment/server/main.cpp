#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <thread>
#include <vector>

using namespace std;

#pragma comment(lib , "ws2_32.lib")

/*
   // Initialize Winsock library

   // Creat the socket

   // Get the IP and Port

   // bind the IP/Port with socket

   // listen to the socket

   // Accept

   // Recieve and Send

   // Close the Socket

   // Cleanup the Winsock
*/

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void InteractWithClient(SOCKET clientSocket , vector<SOCKET>& clients) {
	// Send & Recv
	cout << "client connected" << endl;
	char buffer[4096];
	while (1) {
		int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesrecvd == 0) {
			cout << "Client Disconnected" << endl;
		}
		string message(buffer, bytesrecvd);
		cout << "Message from clint : " << message << endl;
		for (auto client : clients) {
			if (client != clientSocket) {
				// all other clients connected will recieve except sender 
				send(client, message.c_str(), message.length(), 0);
			}
		}
	}
	auto it = find(clients.begin(), clients.end(), clientSocket);
	if (it != clients.end()) {
		clients.erase(it);
	}
	closesocket(clientSocket);
}

int main() {
	if (!Initialize()) {
		cout << " Winsock Inialaization Failed " << endl;
	}
	
	cout << "Server repo processing..............." << endl;

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSocket == INVALID_SOCKET) {
		cout << "Socket Cration Failed " << endl;
		return 1;
	}

	// Creat Address Structure
	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port); 

	// Convert the ipaddrees (0.0.0.0) put it inside the sin_family in binary format. 
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
		cout << "seeting address structure failed" << endl ;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// bind
	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr))== SOCKET_ERROR) {
		cout << "Bind Failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// listen to the socket
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Listen Failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	
	cout << "Server Has Started Listeing on Port : " << port << endl;	
	vector<SOCKET> clients;

	while (1) {
		// Accept
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			cout << "invalid clind socket" << endl;
		}
		clients.push_back(clientSocket);

		thread t1(InteractWithClient, clientSocket , std::ref(clients));
		t1.detach();
	}

	closesocket(listenSocket);

	WSACleanup();
	return 0;
}