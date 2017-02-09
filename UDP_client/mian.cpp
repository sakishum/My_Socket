/**
 * @file mian.cpp
 * @Synopsis  UDP Client example. (单播)
 *		Usage: ./test localhost 
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2015-11-20
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>	// sockaddr_in
#include <netdb.h>		// gethostbyname
#include <unistd.h>		// close

#define SERVER_PORT 8888
#define MAX_BUF_SIZE 1024

// 如果使用了 connect 就可以使用 send, recv 而不是使用 sendto, recvfrom 接口， 但还是还是利用传输层的UDP协议

void udps_requ(int sockfd, const struct sockaddr_in *addr, int len) {
	char buffer[MAX_BUF_SIZE];
	int size = 0;
	while (1) {
		printf("Please input char:\n");
		fgets(buffer, MAX_BUF_SIZE+1, stdin);
		// 阻塞
		size = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)addr, len);
		if (size < 0) {
			fprintf(stderr, "\n\rsend error.\r\n");
		}
		bzero(buffer, MAX_BUF_SIZE);
	}
}

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in addr;
	struct hostent *host;
	if (argc != 2) {
		fprintf(stderr, "Usage:%s server_ip\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// 返回对应于给定主机名的包含主机名字和地址信息的hostent结构指针。
	// 返回：非空指针——成功，空指针——出错，同时设置 h_errno
	if ((host = gethostbyname(argv[1])) == NULL) {
		fprintf(stderr, "Gethostname error.\n");
		herror("gethostbyname");
		exit(EXIT_FAILURE);
	}

	// socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket < 0) {
		fprintf(stderr, "Socket Error.\n");
		exit(EXIT_FAILURE);
	}

	// IP:PORT
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr = *((struct in_addr*)host->h_addr);

	udps_requ(sockfd, &addr, sizeof(struct sockaddr_in));

	close(sockfd);

	exit(EXIT_SUCCESS);
}

