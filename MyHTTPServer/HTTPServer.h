#pragma once
#include <string>
#include <map>
#include <thread>
#include <vector>
#include <sstream>
#include <iostream>
#include <time.h>
#include "def.h"
#include "fcgi.h"
#include <deque>

/*
	create HTTPServer instance
		HTTPServer *server = new HTTPServer(host, port, callback_function);
	callback function's prottype
		CONTENT name(CONTENT content);

	call HTTPServer->start(), if return <0, exit program

*/

#define RECEIVE_BUFFER_SIZE 512
#define MAX_THREADS 128
#define LISTEN_MAX 64
#define LLD long long int

// http_status codes
static std::map<int, std::string> http_status_message{
	{200, "OK"},
	{301, "Moved Permanently"},
	{302, "Found"},
	{400, "Bad Request"},
	{401, "Unauthorized"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{406, "Not Acceptable"},
	{408, "Request Timeout"},
	{411, "Length Required"},
	{418, "I'm a teapot"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"},
	{502, "Bad Gateway"},
	{503, "Service Unavailable"},
	{504, "Gateway Timeout"}
};

#define DEFAULT_MIME "text/html"
static std::map<std::string, std::string> content_type{
	{"png", "image/png"},
	{"js", "text/javascript"},
	{"css", "text/css"},
	{"html", "text/html"}
};

// small alphabet -> big alphabet
#define sa2ba(c) ((c >= 'a' && c <= 'z')?c-('a' - 'A'):c)

// hex values map
static std::map<char, int> hex_map{
	{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
	{'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
	{'8', 8}, {'9', 9}, {'A', 10}, {'B', 11},
	{'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
};

class HTTPServer {
private:
	SOCK_TYPE server;
	sockaddr_in addr;
	u_long blocking_mode;

	std::string host;

	bool kill_flg;
	bool err;

	std::thread *tWait_connection;
	int runnning_threads;
	std::thread *process_thread[MAX_THREADS];
	std::deque<int> runnable_thread;
	std::deque<int> finished_thread;

	// スレッド終了監視
	std::thread *tFinish_thread;

	int (*action)(CONTENT content, CONTENT *response);

	void WaitingConnection();
	void ThreadFinishProcess();
	void process(SOCK_TYPE client, int thread_id);
	void finish_process_thread(int id);

public:
	HTTPServer(char *host, int port, int (*callback)(CONTENT, CONTENT *));
	~HTTPServer();

	bool GetError();

	int start(int listen_max = LISTEN_MAX);
	void kill();
};

 // Decode Percent Encoding string(for params)
std::string PercentDecode(std::string base);