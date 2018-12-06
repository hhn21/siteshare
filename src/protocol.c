#include "protocol.h"

/* Make new request with NULL data */
Request newRequest() {
	Request request;
	request.length = 0;
	request.data = NULL;
	return request;
}

/* Make new response with NULL data */
Response newResponse() {
	Response response;
	response.length = 0;
	response.data = NULL;
	return response;
}

/* Bind socket to specific port and listen to connections
 * params:
 *      socket: socket file descriptor
 *      port: port number
 * returns:
 *      1 if request succesfully
 *      <= 0 value if has any error
 */
int listenOnPort(int socket, int port){
	struct sockaddr_in servaddr;
	int rs;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);

	// Bind socket to address
	rs = bind(socket, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(rs <= 0) return rs;

	// Listen request from client
	rs = listen(socket, BACKLOG);
	if(rs <= 0) return rs;

	return 1;
}

int _sendBytes(int socket, char* bytes, int length) {
	int bytesSent;
	int nLeft = length, idx = 0;

	while (nLeft > 0){
		bytesSent = send(socket, &(bytes[idx]), nLeft, 0);
		if (bytesSent < 0) return bytesSent;
		nLeft -= bytesSent;
		idx += bytesSent;
	}

	return length;
}

int _recvBytes(int socket, char* des, int length) {
	int bytesRecv;
	int nLeft = length, idx = 0;

	while (nLeft > 0){
		bytesRecv = recv(socket, &(des[idx]), nLeft, 0);
		if (bytesRecv < 0) return bytesRecv;
		nLeft -= bytesRecv;
		idx += bytesRecv;
	}

	return length;
}

/* Send a request to server through socket
 * params:
 *      socket: socket file descriptor
 *      request: request to send
 * returns:
 *      1 if request succesfully
 *      <= 0 value if has any error
 */
int request(int socket, Request request, Response *response){
	int rs;

	/************************* Send request *************************/
	rs = _sendBytes(socket, (char*)&request.opcode, sizeof(RequestOpcode)); 	// send request opcode
	if(rs < 0) return rs;
	rs = _sendBytes(socket, (char*)&request.length, sizeof(RequestLengthType)); // send request length
	if(rs < 0) return rs;
	rs = _sendBytes(socket, (char*)request.data, (int)request.length); 			// send request data
	if(rs < 0) return rs;

	/************************* Receive response *************************/
	rs = _recvBytes(socket, (char*)&response->status, sizeof(ResponseStatus)); 		// receive response length
	if(rs < 0) return rs;
	rs = _recvBytes(socket, (char*)&response->length, sizeof(ResponseLengthType));  // receive response length
	if(rs < 0) return rs;
	response->data = malloc(response->length);
	rs = _recvBytes(socket, (char*)response->data, (int)response->length); 			// receive response data
	if(rs < 0) return rs;

  	return 1;
}

/* Send a response to client through socket
 * params:
 *      socket: socket file descriptor
 *      response: response to send
 * returns:
 *      bytes sent
 *      <= 0 value if has any error
 */
int response(int socket, Response response){
	int rs;

	rs = _sendBytes(socket, (char*)&response.status, sizeof(ResponseStatus)); 	  // send response length
	if(rs < 0) return rs;
	rs = _sendBytes(socket, (char*)&response.length, sizeof(ResponseLengthType)); // send response length
	if(rs < 0) return rs;	
	rs = _sendBytes(socket, (char*)response.data, (int)response.length); 		  // send response data
	if(rs < 0) return rs;

	return response.length;
}

/* Receive a request from client through socket
 * params:
 *      socket: socket file descriptor
 *      request: request pointer to save client request
 * returns:
 *      1 if fetch request succesfully
 *      <= 0 value if has any error
 */
int fetchRequest(int socket, Request *request){
	int rs;

	rs = _recvBytes(socket, (char*)&request->opcode, sizeof(RequestOpcode)); 	 // receive request length
	if(rs < 0) return rs;
	rs = _recvBytes(socket, (char*)&request->length, sizeof(RequestLengthType)); // receive request length
	if(rs < 0) return rs;
	request->data = malloc(request->length);
	rs = _recvBytes(socket, (char*)request->data, (int)request->length); 		 // receive request data
	if(rs < 0) return rs;

	return 1;
}