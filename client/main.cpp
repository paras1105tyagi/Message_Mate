#include<iostream>
#include<WinSock2.h>
#include<thread>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

//initialise winsock


//create socket

//connect to the server

//send/recv

//close the socket

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}





void SendMsg(SOCKET s) {
	cout << "Enter your chat name: " << endl;
	string name;
	getline(cin, name);
	string message;
	
	while (1) {
		getline(cin, message);
		string msg = name + " : " + message;
		int bytesent = send(s, msg.c_str(), msg.length(), 0);
		if (bytesent == SOCKET_ERROR) {
			cout << "error sending message " << endl;
			break;
		}
		if (message == "quit") {
			cout << "Stopping the application " << endl;
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
		recvlength = recv(s, buffer, sizeof(buffer), 0);





		if (recvlength <= 0) {
			cout << "disconnected from server " << endl;
			break;
		}
		else {
			msg = string(buffer, recvlength);
			cout << msg << endl;
		}
	}
	closesocket(s);
	WSACleanup();
}
int main() {

	if (!Initialize()) {
		cout << "initialize winsock failed " << endl;
		return 1;
	}


	SOCKET s;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "invalid sockey created " << endl;
		return 1;
	}
	int port = 12345;
	string serveraddress = "127.0.0.1";
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

	if(connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR){
		cout << "Not able to connect to server" << endl;
		closesocket(s);
		WSACleanup();
		return 1;
	}

	cout << "successfully connected to server " << endl;

	thread senderthread(SendMsg, s);
	thread receiver(ReceiveMsg, s);
	//send/recv
	senderthread.join();
	receiver.join();
	//cout << "client program started " << endl;


	

	return 0;
}