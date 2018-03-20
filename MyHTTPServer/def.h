#pragma once
#include <map>
#include <string>
#include <vector>
#include <cstring>

std::vector<std::string> str_split(std::string str, std::string spliter);
bool CheckNoPacket();

#ifdef _MSC_VER
	#include <winsock2.h>
	#define _CRT_SECURE_NO_WARNINGS

	#define SOCK_TYPE SOCKET
	#define SOCK_LEN int
	#define INVALID_SOCK INVALID_SOCKET

	#define ioctrl(s, t, m) ioctlsocket(s, t, m)
	#define closesock(s) closesocket(s)

	#define slp(t) Sleep(t * 1000)
	#define S_ADDR S_un.S_addr

#else
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>

	#define SOCK_TYPE int
	#define SOCK_LEN socklen_t
	#define INVALID_SOCK -1

	#define ioctrl(s, t, m) ioctl(s, t, m)
	#define closesock(s) close(s)

	#define slp(t) usleep(t * 1000) // slp(1) = wait 1000ms
	#define S_ADDR s_addr
#endif

class CONTENT {
public:
	std::string method_type;
	std::string request_uri;
	float http_ver;
	int status_code;

	std::string content;

	std::map<std::string, std::string> *header;
	std::map<std::string, std::string> *params;

	CONTENT() {
		method_type = "";
		request_uri = "";
		http_ver = 1.0;
		status_code = 200;
		content = "";

		header = new std::map<std::string, std::string>();
		params = new std::map<std::string, std::string>();
	}

	CONTENT(CONTENT& src) {
		method_type = src.method_type;
		request_uri = src.request_uri;
		http_ver = src.http_ver;
		status_code = src.status_code;
		content = src.content;

		header = new std::map<std::string, std::string>(*(src.header));
		params = new std::map<std::string, std::string>(*(src.params));
	}

	~CONTENT() {
		delete header, params;
	}

	void SetHeader(std::string key, std::string value) {
		header->insert(std::pair<std::string, std::string>(key, value));
	}
	void SetParam(std::string key, std::string value) {
		params->insert(std::pair<std::string, std::string>(key, value));
	}

	std::string GetHeader(std::string key) {
		std::map<std::string, std::string>::iterator it = header->find(key);
		if (it != header->end() || header->end()->first == key) return it->second;
		else return "";
	}
	std::string GetParam(std::string key) {
		std::map<std::string, std::string>::iterator it = params->find(key);
		if (it != params->end() || params->end()->first == key) return it->second;
		else return "";
	}
};