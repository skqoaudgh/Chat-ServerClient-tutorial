#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main()
{
	// Initialize Winsock
	WSADATA wsData; // WinSock data-structure
	WORD ver = MAKEWORD(2, 2); // means 2.2 version

	int wsOk = WSAStartup(ver, &wsData); // initializing Winsock lib 
	if (wsOk != 0)
	{
		cerr << "Can`t Initialize Winsock! Quitting" << endl;
		return 1;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); // make socket with IPv4, TCP (SOCK_DGRAM:UDP)
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can`t create a socket! Quitting" << endl;
		return 1;
	}

	// Bind thd socket to an ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); // host byte to network byte (little->big endian)
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientsize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);

	char host[NI_MAXHOST];		// client`s remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST);	// same as memset(host, 0, NI_MAXHOST)
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, clientsize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);

	// while loop: accept and echo message back to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) // SOCKET_ERROR = -1
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		// Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup Winsock
	WSACleanup();
	return 0;
}