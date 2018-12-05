#include "protocol.h"

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

/* Send a request to server through socket
 * params:
 *      socket: socket file descriptor
 *      request: request to send
 * returns:
 *      1 if request succesfully
 *      <= 0 value if has any error
 */
int request(int socket, Request request, Response *response){


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
	return 0;
}