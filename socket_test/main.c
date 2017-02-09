/* Copyright(C)
 * For free
 * All right reserved
 */
/**
 * @file main.c
 * @Synopsis  Example for socket client in C.
 *		input:	./test 127.0.0.1 8888
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2013-09-04
 */

#include <netinet/in.h>
// ----------- Socket -------------
#include <sys/types.h>
#include <sys/socket.h>
// ----------- Socket -------------
#include <arpa/inet.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("connect address prot\n");
		exit(EXIT_FAILURE);
	}
	//          domain:IPv4  type:TCP	protocol:当protocol为0时，会自动选择type类型对应的默认协议
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in my_addr;

	my_addr.sin_addr.s_addr = inet_addr(argv[1]);
	my_addr.sin_family = AF_INET;	
	my_addr.sin_port = htons(strtol(argv[2], NULL, 10));

	int ret = connect(fd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in));

	if (-1 == ret) {
		perror("Connect failed:");
		exit(EXIT_FAILURE);
	}

	close(fd);
	exit(EXIT_SUCCESS);
}

