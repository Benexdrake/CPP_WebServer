#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <winsock2.h>
#include "Request.cpp"

using namespace std;

#pragma comment(lib,"ws2_32.lib")

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main()
{
	WSAData wsaData;
	Request req = Request();

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "Fehler beim Initialisieren von Winsock" << endl;
		return 1;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		cerr << "Fehler beim Erstellen des Sockets" << endl;
		return 1;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "Fehler beim Binden des Sockets" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cerr << "Fehler beim Wechsel in den Empfangsmodus" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	cout << "Server gestartet. Warte auf Verbindungen..." << endl;

	while (true)
	{
		sockaddr_in clientAddr;
		int clientAddrLen = sizeof(clientAddr);
		SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			cerr << "Fehler beim Akzeptieren der Verbindung" << endl;
			continue;
		}

		char buffer[BUFFER_SIZE] = { 0 };
		int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
		if (bytesRead == SOCKET_ERROR)
		{
			cerr << "Fehler beim Lesen der Anfrage" << endl;
			closesocket(clientSocket);
			continue;
		}

		string request(buffer);

		req.handleRequest(clientSocket, request);

		closesocket(clientSocket);
	}

	closesocket(serverSocket);
	WSACleanup();

	return 0;
}

