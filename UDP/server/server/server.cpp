#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main()
{
	// Startup Winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &data);
	if (wsOk != 0)
	{
		cerr << "Can`t start Winsock!" << endl;
		return 1;
	}

	// Bind socket to ip address and port
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in hint;
	hint.sin_addr.S_un.S_addr = ADDR_ANY;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); // Convert from little to big endian

	if (bind(in, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		cerr << "Can`t bind socket!" << endl;
		return 1;
	}

	sockaddr_in client; // Client information structure
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);
	
	char buf[1024]; // buffer to receive message

	// Enter a loop
	while (true)
	{
		ZeroMemory(buf, 1024);

		// Wait for message
		int bytesReceived = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error receiving from client!" << endl;
			return 1;
		}

		// Display message and client info
		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256); // Convert IP address bytes form to string form

		cout << "Message recv from " << clientIp << " : " << buf << endl;
	}

	// Close socket
	closesocket(in);

	// Shutdown Winsock
	WSACleanup();
	return 0;
}