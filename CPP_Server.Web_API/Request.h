#pragma once

#include <winsock2.h>

class Request
{
public:
	void handleRequest(SOCKET clientSocket, const string& request);
};

