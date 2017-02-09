/**
 * @file main.cpp
 * @Synopsis  UDP Server example.（单播）
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2015-11-20
 */

#include <cstdio>
#include <cstdlib>
#include <string.h>			// strlen
#include <sys/types.h>		
#include <sys/socket.h>		// socket
#include <errno.h>			// strerror
#include <unistd.h>			// close
#include <arpa/inet.h>		// sockaddr_in

#define SERVER_PORT 12000
#define MAX_MSG_SIZE 1024

// 如果使用了 connect 就可以使用 send, recv 而不是使用 sendto, recvfrom 接口， 但还是还是利用传输层的UDP协议

void udps_respon(int sockfd) {
	struct sockaddr_in addr;
	int n;
	socklen_t addrlen;
	char msg[MAX_MSG_SIZE];
	while (1) {
		bzero(msg, sizeof(msg));
		addrlen = sizeof(sockaddr);
		// ssize_t recvfrom(int, void*, size_t, int, sockaddr*, socklen_t*)
		// 阻塞
		n = recvfrom(sockfd, static_cast<void*>(msg), MAX_MSG_SIZE, 0, (struct sockaddr*)&addr, &addrlen);
		msg[n] = '\0';
		fprintf(stdout, "Server have recvived: %s", msg);
	}
}

int main(void) {
	int sockfd;
	struct sockaddr_in addr;
	// Socket
	// AF_INET	   IPv4
	// SOCK_DGRAM  UDP
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("Socket Error.\n");
		exit(EXIT_FAILURE);
	}

	// IP:Port
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 设置本地主机 IP 地址
	addr.sin_port = htons(SERVER_PORT);			// 设置端口号

	// bind
	// sockaddr_in ==转==> sockaddr
	if (bind(sockfd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(struct sockaddr)) < 0) {
		perror("Bind Error.\n");
		exit(EXIT_FAILURE);
	}

	udps_respon(sockfd);
	close(sockfd);

	return EXIT_SUCCESS;
}

