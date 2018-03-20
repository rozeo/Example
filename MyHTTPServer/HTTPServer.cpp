#include "HTTPServer.h"
#include <string.h>

#define RECV_RETRY_COUNT 10

std::vector<std::string> str_split(std::string str, std::string spliter) {
	std::vector<std::string> v;
	std::string s = "";
	for (int i = 0; i < str.length();) {
		if (str.substr(i, spliter.length()) == spliter) {
			v.push_back(s);
			s = "";
			i += spliter.length();
		}
		else {
			s += str[i];
			i++;
		}
	}
	if (s.length() > 0) {
		v.push_back(s);
	}
	return v;
}

bool CheckNoPacket() {
#ifdef _MSC_VER
	if (WSAGetLastError() == WSAEWOULDBLOCK) return true;
	return false;
#else
	if (errno == EWOULDBLOCK) return true;
	return false;
#endif
}

HTTPServer::HTTPServer(char *host, int port, int (*callback)(CONTENT, CONTENT *)) {

	this->host = host;
	if (port < 1 || port >= 65535) {
		this->err = true;
		return;
	}

	this->server = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server == INVALID_SOCK) {
		this->err = true;
		return;
	}

	this->addr.sin_family = AF_INET;
	this->addr.sin_port = htons(port);

	unsigned long h = inet_addr(host);
	if (h == INADDR_NONE) {
		h = **(unsigned int **)(gethostbyname(host)->h_addr_list);
	}
	if (h == INADDR_NONE) {
		this->err = true;
		return;
	}

	#ifdef _MSC_VER
		this->addr.sin_addr.S_ADDR = h;
		int yes = TRUE;

		if (setsockopt(this->server, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) == SOCKET_ERROR) {
			this->err = true;
		}
	#else
		this->addr.sin_addr.S_ADDR = h;
		bool yes = true;

		if (setsockopt(this->server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
			this->err = true;
		}
	#endif

	// ソケット切断後の即時再使用許可


	this->action = callback;
	this->kill_flg = false;
	this->blocking_mode = 1;
	this->err = false;

	runnning_threads = 0;
	for (int i = 0; i < MAX_THREADS; i++) runnable_thread.push_back(i);
}

HTTPServer::~HTTPServer(){
	if (!(this->kill_flg)) this->kill();
	delete tWait_connection, tFinish_thread;
}

bool HTTPServer::GetError() {
	return this->err;
}

int HTTPServer::start(int listen_max) {
	if (bind(this->server, (sockaddr *)&(this->addr), sizeof(this->addr)) != 0) {
		return -1;
	}

	if (listen(this->server, listen_max) != 0) {
		return -2;
	}
	ioctrl(this->server, FIONBIO, &(this->blocking_mode));
	this->tWait_connection = new std::thread(&HTTPServer::WaitingConnection, this);
	return 1;
}

void HTTPServer::kill() {
	if(this->err) return;
	printf("server stopping ... ");
	this->kill_flg = true;
	this->tWait_connection->join();
	this->tFinish_thread->join();
	printf("success server stopped\n");
}

void HTTPServer::WaitingConnection() {
	printf("start listening\n");
	this->tFinish_thread = new std::thread(&HTTPServer::ThreadFinishProcess, this);

	sockaddr_in client_addr;
	SOCK_LEN addrlen = sizeof(client_addr);
	SOCK_TYPE client;
	std::thread *th;

	while (!kill_flg) {
		client = accept(this->server, (sockaddr *)&client_addr, &addrlen);
		if (client == INVALID_SOCK){
			slp(0.02);
			continue;
		}
		while (runnable_thread.empty()) slp(0.01);
		int thread_id = runnable_thread.front();
		runnable_thread.pop_front();
		this->process_thread[thread_id] = new std::thread(&HTTPServer::process, this, client, thread_id);
		this->runnning_threads++;
		
	}
	closesock(this->server);
	printf("stop listening\n");
}

void HTTPServer::ThreadFinishProcess() {
	while (!kill_flg || runnning_threads > 0) {
		if (!this->finished_thread.empty()) {
			int thread_id = this->finished_thread.front();
			this->process_thread[thread_id]->join();
			delete this->process_thread[thread_id];
			this->finished_thread.pop_front();
			this->runnable_thread.push_back(thread_id);
			this->runnning_threads--;
		}
		else {
			slp(0.01);
		}
	}
	printf("stop\n");
}

void HTTPServer::process(SOCK_TYPE client, int thread_id) {
	CONTENT *content = new CONTENT();
	CONTENT *response = new CONTENT();

	std::string request_str = "";
	char *buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
	if (buffer == NULL) {
		closesock(client);
		finish_process_thread(thread_id);
		return;
	}
	int receive_size;

	if (ioctrl(client, FIONBIO, &(this->blocking_mode)) < 0) {
		closesock(client);
		finish_process_thread(thread_id);
		return;
	}

	int retry_count = 0;
	while(true){
		receive_size = recv(client, buffer, RECEIVE_BUFFER_SIZE, 0);
		if ((receive_size == 0 && !CheckNoPacket()) || (!CheckNoPacket() && receive_size == -1)) {
			closesock(client);
			return;
		}
		request_str += buffer;

		if (receive_size < RECEIVE_BUFFER_SIZE) break;
	}

	std::vector<std::string> request = str_split(request_str, "\r\n");

	std::string params = "";
	std::istringstream top(request[0]);
	std::string uri;
	top >> content->method_type >> uri  >> content->http_ver;
	if (content->method_type == "POST") {
		content->request_uri = uri;
	}
	else {
		std::vector<std::string> uri_sp = str_split(uri, "?");
		content->request_uri = uri_sp[0];
		if (uri_sp.size() > 1) {
			params = uri_sp[1];
		}
	}

	int i;
	for (i = 1; request[i] != ""; i++) {
		char header[1024];
		sscanf(request[i].c_str(), "%[^:]", header);
		content->SetHeader(header, request[i].substr(strlen(header) + 2, request[i].length() - strlen(header) + 2));
	}
	if (content->method_type == "POST") {
		for (++i; i < request.size(); i++) {
			params += request[i];
		}
	}

	// GET, POSTパラメータの読み込み
	if (params != "") {
		std::vector<std::string> prms = str_split(params, "&");
		for (std::vector<std::string>::iterator it = prms.begin(); it != prms.end(); it++) {
			char name[1024];
			sscanf((*it).c_str(), "%[^=]", name);
			content->SetParam(name, (*it).substr(strlen(name) + 1, (*it).length() - strlen(name) + 1));
		}
	}

	this->action(*content, response);
	if (response->GetHeader("Host") == "") response->SetHeader("Host", this->host);

	if (response->status_code <= 0) response->status_code = 200;

	response->SetHeader("Content-Length", std::to_string(response->content.length()));

	//

	std::ostringstream res_str;
	char http_ver[5];
	sprintf(http_ver, "%.1f", content->http_ver);
	res_str << "HTTP/" << http_ver << " "
			<< response->status_code << " "
			<< http_status_message[response->status_code] << "\r\n";

	for (auto it = response->header->begin(); it != response->header->end(); it++) {
		res_str << (*it).first << ": " << (*it).second << "\r\n";
	}
	res_str << "\r\n";
	res_str << response->content << "\r\n";

	LLD send_size = 0;
	while (send_size < res_str.str().length()) {
		LLD sz = send(client, res_str.str().c_str(), res_str.str().length(), 0);
		if (sz < 0) break;
		send_size += sz;
	}
	printf("[%lld] %s %d\n", time(NULL), content->request_uri.c_str(), response->status_code);
	closesock(client);

	delete content;
	delete response;
	finish_process_thread(thread_id);
}

void HTTPServer::finish_process_thread(int id) {
	this->finished_thread.push_back(id);
}

std::string PercentDecode(std::string base) {
	std::string conv_to = "";
	for (int i = 0; i < base.length(); i++) {
		if (base[i] == '%') {
			if (i + 2 < base.length()) {
				if (base[i + 1] == '%') {
					conv_to += '%';
					i++;
				}
				else {
					int u = hex_map[sa2ba(base[i + 1])];
					int l = hex_map[sa2ba(base[i + 2])];
					conv_to += (char)(u * 16 + l);
					i += 2;
				}
				continue;
			}
		}
		conv_to += base[i];
	}
	return conv_to;
}