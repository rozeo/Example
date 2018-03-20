#include "fcgi.h"
#include <vector>
#include <iostream>

#define PHP_FPM_DST "127.0.0.1"
#define PHP_FPM_PORT 9000

int fastcgi_pass(CONTENT *response, std::map<std::string, std::string> params) {

	// make socket to php-fpm
	SOCK_TYPE sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	std::string request_str = "";
	std::string content = "";
	
	u_long block = 1;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PHP_FPM_PORT);
	addr.sin_addr.S_ADDR = inet_addr(PHP_FPM_DST);

	// declare fpm headers
	FCGI_BeginRequestRecord beginRecord;
	memset(&beginRecord, 0, sizeof(FCGI_BeginRequestRecord));
	beginRecord.header.version = FCGI_VERSION_1;
	beginRecord.header.type = FCGI_BEGIN_REQUEST;
	beginRecord.header.contentLengthB0 = FCGI_HEADER_LEN;
	beginRecord.header.requestIdB0 = 1;

	beginRecord.body.roleB0 = FCGI_RESPONDER;
	beginRecord.body.flags = FCGI_CLOSE_AFTER;

	int content_length = 0;
	for (auto it = params.begin(); it != params.end(); it++) {
		content_length += (*it).first.length() + (*it).second.length() + 2;
	}

	// add params header record
	int paddingLength = 8 - content_length % 8;
	FCGI_Record record;
	memset(&record, 0, sizeof(FCGI_Record));
	record.version = FCGI_VERSION_1;
	record.type = FCGI_PARAMS;
	record.requestIdB0 = 1;
	record.contentLengthB0 = (UC)(content_length % 256);
	record.contentLengthB1 = (UC)(content_length / 256);
	record.paddingLength = (UC)paddingLength;

	// declare fpm end headers
	FCGI_EndRequestRecord endRecord;
	memset(&endRecord, 0, sizeof(FCGI_EndRequestRecord));
	endRecord.header.version = FCGI_VERSION_1;
	endRecord.header.type = FCGI_END_REQUEST;
	endRecord.header.requestIdB0 = 1;

	endRecord.body.protocolStatus = FCGI_REQUEST_COMPLETE;

	char *full_request = (char *)malloc(FCGI_HEADER_LEN * 5 + content_length + paddingLength);
	if (full_request == NULL) return -1;

	int offset = 0;
	memcpy(&full_request[offset], &beginRecord, sizeof(beginRecord));
	offset += sizeof(beginRecord);

	memcpy(&full_request[offset], &record, sizeof(record));
	offset += sizeof(record);

	for (auto it = params.begin(); it != params.end(); it++) {

		std::string f = (*it).first, s = (*it).second;
		full_request[offset] = (UC)f.length();
		full_request[offset + 1] = (UC)s.length();
		memcpy(&full_request[offset + 2], f.c_str(), f.length());
		memcpy(&full_request[offset + 2 + f.length()], s.c_str(), s.length());

		offset += 2 + f.length() + s.length();
	}

	for (int i = 0; i < paddingLength; i++) full_request[offset++] = 0;
	memcpy(&full_request[offset], &endRecord, sizeof(endRecord));
	offset += sizeof(endRecord);

	// connect php-fpm host, and set non-blocking mode
	int err = connect(sock, (sockaddr *)&addr, sizeof(addr));
	if (err != 0) {
		printf("failed connect, php-fpm host\n");
		free(full_request);
		return -1;
	}

	ioctrl(sock, FIONBIO, &block);

	int size = 0;
	while (size < offset) {
		int sz = send(sock, full_request, offset, 0);
		if (sz < 0) {
			closesock(sock);
			return -1;
		}
		size += sz;
	}
	free(full_request);
	
	char buffer[2048];

	/*
		パケなし定義
		VS20xx: size == -1 かつ WSAGetLastError == WSAEWOULDBLOCK
		GCC:    size == 0 かつ errno == EWOULDBLOCK
	*/
	do {
		size = recv(sock, buffer, 2048, 0);
	} while((size == 0 && CheckNoPacket()) || (CheckNoPacket() && size == -1));

	// load record
	FCGI_Record responseRecord;
	memcpy(&responseRecord, buffer, sizeof(FCGI_Record));
	int content_size = (int)(responseRecord.contentLengthB1) * 256 + (int)(responseRecord.contentLengthB0);
	size -= sizeof(FCGI_Record);

	std::string recv_buffer = &buffer[sizeof(FCGI_Record)];
 
	while (size < content_size) {
		int sz = recv(sock, buffer, 2048, 0);

		// パケットサイズが0 もしくはWSAEWOULDBLOCKかつエラーなら継続
		if (sz <= 0) {
			if (CheckNoPacket())
				continue;
			else
				break;
		}
		size += sz;
		recv_buffer += buffer;
	}

	closesock(sock);
	recv_buffer = recv_buffer.substr(0, content_size);

	std::vector<std::string> sp = str_split(recv_buffer, "\r\n");
	std::vector<std::string>::iterator it = sp.begin();
	if (responseRecord.type == FCGI_STDERR) {
		response->content = recv_buffer;
	}
	else {
		bool crlf = false;
		for (; it != sp.end(); it++) {
			if (*it == "") {
				crlf = true;
			}
			else {
				if (crlf) break;
				char header[512];
				sscanf((*it).c_str(), "%[^:]", header);
				response->SetHeader(header, (*it).substr(strlen(header) + 2, (*it).length() - strlen(header) + 2));
			}
		}
	}
	response->content = *it;
	return 0;
}