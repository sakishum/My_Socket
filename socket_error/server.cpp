#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>		// sockaddr_in

auto main(int, char**) -> int32_t {
	struct sockaddr_in serverAddr;
	//struct sockaddr_in clientAddr;

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(1234);

	// htonl: convert values between host and network byte order(字节序)
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* whilcared 通配地址*/ 
	printf("INADDR_ANY = %d\n", INADDR_ANY);		// INADDR_ANY = 0

	//bzero(&clientAddr, sizeof(clientAddr));
	//socklen_t clientAddrLen = 0;

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	int yes = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));

	if (listenfd < 0) {
		perror("Create socket fail.\n");
		close(listenfd);
		exit(EXIT_FAILURE);
	}

	int bindResult = bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (bindResult < 0) {
		close(listenfd);
		perror("Bind socket port fail.\n");
		exit(EXIT_FAILURE);
	} else {
		printf("Bind socket port success.\n");
	}

	// listen 仅供 TCP 服务器调用，它做两件事：
	// 1) 当 socket 函数创建一个套接字时，他被假设为一个主动套接字，也就是说，
	// 他是一个将调用 connect 发起连接的客户套接字。listen 函数把一个未连接的
	// 套接字转换成一个被动套接字，指示内核应接受指向该套接字的连接请求;
	// 2) 函数的第二个参数规定了内核应该为相应套接字排队的最大连接个数。
	// listen(listenfd, 20);	// client will connect success
	
	// 服务器并没有调用 accept，这是因为调用 listen 方法之后，内核为任何一个
	// 给定的监听套接字维护两个队列：未完成连接队列和已完成连接队列；当客户端
	// SYN 到达时，如果队列是满的，TCP 就忽略该分节，但不会发送 RST(重建连接)；
	// 当进程调用 accept 时，已完成队列的队顶项将返回给进程，如果队列为空，则
	// 阻塞(套接字)默认为阻塞。
	//		未完成连接队列(incomplete connection queue)：
	//		已完成连接队列(complete connection queue)：
	// 也就说，只要调用了 listen 方法后，服务器端就打开了三次握手的开关，能够
	// 处理来自客户端的 SYN 分节了，只要三次握手完成，客户端就会 connect 成功,
	// 而跟服务器调不调用 accept 没有任何关系，accept 只是去取已完成连接队列
	// 的队顶项。
	sleep(60*5);

	exit(EXIT_SUCCESS);
}


