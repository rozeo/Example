#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "HTTPServer.h"
#include <regex>

#define INADDR_LOCALHOST 0x7F000001
#ifdef _MSC_VER
	#define EXEC_ROOT "C:/Users/stpro/Documents/Visual Studio 2017/Projects/socket/socket" 
#else
	#define EXEC_ROOT "/home/roz-dev/cpp/http_server"
#endif
#define DOC_ROOT "doc"
#define INDEX_FILE "index.html";

int callback(CONTENT request, CONTENT *response) {

	response->SetHeader("ServerName", "EasyHTTPServer");
	response->SetHeader("ServerVersion", "v1.0.0");

	if (request.request_uri[request.request_uri.length() - 1] == '/') {
		request.request_uri += INDEX_FILE;
	}

	std::smatch match;
	std::string ext;

	// request_uri�����g���q���o
	std::regex_search(request.request_uri, match, std::regex("\\.([^\\.]*)$"));
	if (match[1].str() == "") {
		response->SetHeader("Content-Type", DEFAULT_MIME);
	}
	else {
		ext = match[1].str();
		response->SetHeader("Content-Type", (content_type[ext] == "")? DEFAULT_MIME: content_type[ext]);
	}

	if (ext == "php") {
		int suc = fastcgi_pass(response, {
			{"REQUEST_METHOD", request.method_type},
			{"SCRIPT_FILENAME", EXEC_ROOT + std::string("/") + DOC_ROOT + request.request_uri},
			{"DOCUMENT_URI", request.request_uri},
			{"SCRIPT_NAME", request.request_uri},
			{"DOCUMENT_ROOT", EXEC_ROOT + std::string("/") + DOC_ROOT},
			{"HTTP_COOKIE", request.GetHeader("Cookie")}
		});
		if (suc != 0) {
			response->status_code = 503;
			response->content = "Internal Server Error";
		}
		return 0;
	}
	else {

		std::ostringstream cont;

		FILE *fp = fopen((DOC_ROOT + request.request_uri).c_str(), "rb");
		if (fp == NULL) {
			response->status_code = 404;
			response->content = "<h1>Not Found.</h1>";
			return 0;
		}

		int size = 0;
		while (true) {
			char buf;
			fread(&buf, 1, 1, fp);

			response->content += buf;
			if (feof(fp) != 0) break;
		}

		fclose(fp);

		response->content += PercentDecode(request.GetParam("value"));
	}
	return 0;
}

int main() {

	#ifdef _MSC_VER
		WSAData wsa;
		WSAStartup(MAKEWORD(2, 0), &wsa);
	#endif

	HTTPServer *server = new HTTPServer("127.0.0.1", 18000, callback);
	if (server->GetError()) {
		return -1;
	}
	int err;
	if ((err = server->start()) < 0) {
		printf("failed %d\n", err);
		#ifdef _MSC_VER
			WSACleanup();
		#endif
		return -2;
	}
	std::string str;
	while (true) {
		std::cin >> str;
		if (str == "exit") break;
	}
	server->kill();

	delete server;
	#ifdef _MSC_VER
		WSACleanup();
	#endif
	return 0;
}
