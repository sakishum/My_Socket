#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 6000

int main(int, char**) {
	setvbuf(stdout, NULL, _IONBF, 0);
	fflush(stdout);

	// bind the address
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);
	addrto.sin_port = htons(PORT);

	// broadcast address
	struct sockaddr_in from;
	bzero(&from, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	from.sin_port = htons(PORT);

	int sockfd = -1;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "socket error\n");
		exit(EXIT_FAILURE);
	}

	const int opt = -1;
	// set the socket type brocast
	int nb = 0;
	nb = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	if (nb == -1) {
		fprintf(stderr, "set socket error\n");
		exit(EXIT_FAILURE);
	}

	if (bind(sockfd, (struct sockaddr *)(&addrto), sizeof(struct sockaddr))) {
		fprintf(stderr, "bind error\n");
		exit(EXIT_FAILURE);
	}

	unsigned int len = sizeof(struct sockaddr_in);
	char msg[100] = {0};

	for (;;) {
		// recv msg from brocast addr
		int ret = recvfrom(sockfd, msg, 100, 0, (struct sockaddr*)(&from), (socklen_t*)(&len));
		if (ret <= 0) {
			fprintf(stderr, "read error\n");
		} else {
			printf("%s\n", msg);
		}
		sleep(1);
	}

	exit(EXIT_SUCCESS);
}
