#pragma once
#include <iostream>
#include <string>
#include <map>
#include <winsock2.h>

using namespace std;

class Request
{
public:
	Request();
	map<string, string> parseQueryString(const string& queryString);
	void requestHandler(SOCKET clientSocket, const string& request);
};
