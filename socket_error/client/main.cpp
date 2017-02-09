#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

int main(void) {
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(1234);
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_addr.s_addr = INADDR_ANY;       // 服务器IP地址--允许连接到所有本地地址上

	int connfd = socket(AF_INET, SOCK_STREAM, 0);

	// 建立与 TCP 服务器的连接
	// 第 2，3 个参数分别是一个指向套接字地质结构的指针和该结构的大小。  
	int connResult = connect(connfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (connResult < 0) {
		// Connection refused	61
		// Operation timed out	60
		perror("Connect fail.\n");
		close(connfd);
	} else {
		printf("Connect success.\n");
	}
	close(connfd);
	exit(EXIT_SUCCESS);
}
