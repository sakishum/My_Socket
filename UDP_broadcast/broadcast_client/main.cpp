#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 6000

int main(int, char**) {
	setvbuf(stdout, NULL, _IONBF, 0);
	fflush(stdout);

	int sockfd = -1;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))  == -1) {
		fprintf(stderr, "socket error.\n");
		exit(EXIT_FAILURE);
	}

	const int opt = -1;
	// set socket type broadcast 
	int nb = 0;
	nb = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(int));
	if (nb == -1) {
		fprintf(stderr, "set socket error.\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	addrto.sin_port = htons(PORT);
	int nlen = sizeof(sockaddr_in);
	
	for (;;) {
		sleep(1);
		// send msg to brocast addr
		char msg[] = {"abcdefg"};
		int ret = sendto(sockfd, msg, strlen(msg), 0, (sockaddr*)(&addrto), nlen);
		if (ret < 0) {
			fprintf(stderr, "send error.\n");
		} else {
			printf("ok\n");
		}
	}
	exit(EXIT_SUCCESS);
}
