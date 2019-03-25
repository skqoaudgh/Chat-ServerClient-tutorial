#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main()
{
	// stdin server information


	// Startup Winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &data);
	if (wsOk != 0)
	{
		cerr << "Can`t start Winsock!" << endl;
		return 1;
	}

	// Create a hint structure for the server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	// Socket Creation
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	// Write out to that server
	while (true)
	{
		string buf;
		getline(cin, buf);

		int sendOk = sendto(out, buf.c_str(), 1024, 0, (sockaddr*)&server, sizeof(server));
		if (sendOk == SOCKET_ERROR)
		{
			cerr << "That didn`t work!" << endl;
			return 1;
		}
	}

	// Close socket
	closesocket(out);

	// Shutdown Winsock
	WSACleanup();
	return 0;
}