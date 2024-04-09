#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <winsock2.h>

using namespace std;

class Request
{
private:
	map<string, string> parseQueryString(const string& queryString) {
		map<string, string> queryParams;

		istringstream iss(queryString);
		string pair;
		while (getline(iss, pair, '&')) {
			size_t pos = pair.find('=');
			if (pos != string::npos) {
				string key = pair.substr(0, pos);
				string value = pair.substr(pos + 1);
				queryParams[key] = value;
			}
		}

		return queryParams;
	}

public:
	void handleRequest(SOCKET clientSocket, const string& request) {
		string response;

		if (request.find("/hello") != string::npos) {

			size_t queryStart = request.find('?');
			if (queryStart != string::npos)
			{
				size_t queryEnd = request.find(' ', queryStart);
				string queryString = request.substr(queryStart + 1, queryEnd - queryStart - 1);
				map<string, string> queryParams = parseQueryString(queryString);

				if (queryParams.count("name") > 0)
				{
					string name = queryParams["name"];

					string json = "{\"message\": \"\Hello " + name + " !\"}";

					response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json;
				}
			}
			else
				response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello!";
		}
		else if (request.find("/goodbye") != string::npos) {
			response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGoodbye!";
		}
		else {
			response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello World!";
		}

		send(clientSocket, response.c_str(), response.length(), 0);
	}
};