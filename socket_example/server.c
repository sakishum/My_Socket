#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// 打开—读/写—关闭 模式
// SBLARSC
// socket -> bind -> listen -> accept -> recv -> send -> close
// 1) 创建 socket
// 2）绑定 socket 和 端口号, bind
// 3）监听该端口, listen
// 4）接收来自客户端的连接请求, accept
// 5）从 socket 中读取数据, recv
// 6）向 socket 中发送数据, send
// 7）关闭 socket, close

#define PORT 12000
#define BUFFER_SIZE 40

/*
 * 用于保存 socket 信息的结构体
 * struct sockaddr {
 * 		unsigned short sa_family;			// 2 , 地址族, AF_xxx
 *  	char sa_data[14];					// 14, 字节的协议地址
 * };
 *
 * 比 sockaddr 更方便使用的结构:
 * 16 Byte:
 * struct   sockaddr_in   {  
 *		short int  			sin_family;     // 2, 地址族
 * 		unsigned short int  sin_port;     	// 2, 端口号（2^16, 1~1024 为保留端口，选择 1025~65535 之间的端口号）
 * 		struct in_addr   	sin_addr;     	// 4, 32 位 IP 地址
 * 		unsigned char   	sin_zero[8];    // 8, 以 0 填充以保持与 struct sockaddr 同样大小
 * };
 *
 * // 包含该 socket 的 IP地址和端口号
 * struct   in_addr   {  
 * 		union {
 *   			struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
 *   			struct { u_short s_w1,s_w2; } S_un_w;
 *   			u_long S_addr; 
 *   	} S_un;
 *   	#define s_addr  S_un.S_addr
 *  }; 
 * */

// 两个网络程序之间的一个网络连接包含 5 种信息：
// 通信协议、本地协议地址、本地主机端口、远端协议地址和远端协议端口

void err_sys(const char *str) {
	perror(str);
}

// wrapper function
int Socket(int family, int type, int protocol) {
	int n = 0;
	if ((n = socket(family, type, protocol)) < 0) {
		err_sys("Socket error.");
	}
	return n;
}

int main(void) {
    char buf[BUFFER_SIZE];
    int server_sockfd, client_sockfd;
    size_t sin_size = sizeof(struct sockaddr_in);
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    memset(&server_address, 0, sizeof(server_address));

	// 无论是TCP还是UDP，socket 都需要与一个本地的IP与端口相对应，称之为 源地址 和 源端口。
	// 当只有单一IP的时候，就可以用INADDR_ANY代替那个单一的IP，因为内核分配时只能选择这一个IP。
	// INADDR_ANY 所有本机 IP
    server_address.sin_family = AF_INET;			// 指代协议族, IPv4
    server_address.sin_addr.s_addr = INADDR_ANY;	// 用于多 IP 机器上(多网卡), 自动填入本机地址, 值为 0(会由内核自由选择分配IP)
    //server_address.sin_port = 0;					// 0 表示系统随机选择一个未被占用的端口号
    server_address.sin_port = htons(PORT);			// 将主机的无符号短整形数转换成网络字节顺序, host to net short int 16位
	// 网络端一般是大端，大端高字节在地址低位，低字节在地址高位
	printf("%X\n", 12000);			// 2EE0 小端
	printf("%X\n", htons(12000));	// E02E 大端
	// htonl(): 把32位值从主机字节序转换成网络字节序
	// htons(): 把16位值从主机字节序转换成网络字节序
	// ntohl(): 把32位值从网络字节序转换成主机字节序
	// ntohs(): 把16位值从网络字节序转换成主机字节序

    // 建立服务器端 socket (TCP)
    if ((server_sockfd = Socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server_sockfd creation failed");
        exit(EXIT_FAILURE);
    }

    // 将套接字绑定到服务器的网络地址上(IP:Port) bind，0:成功, <0:失败
    if ((bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(struct sockaddr))) < 0) {
        perror("server socket bind failed");
        exit(EXIT_FAILURE);
    }

    // 建立监听队列, 设置对大连接数量为 5（大多数系统默认缺省值为 20）
    // listen 使得 socket 处于被动的监听模式，并为 socket 建立一个输入数据队列，将到达的服务请求保存在次队列中，直到程序处理他们
    listen(server_sockfd, 5);

    // 死循环，同 while(1) {}
	for (; ; ) {
		// 等待客户端连接请求到达(阻塞)
		// 同一时间 Server 只能处理一个 Client 请求：在使用当前连接的 socket 和 client 进行交互的时候，不能够 accept 新的连接请求。
		printf("Start accept client.\n");
		// listening socket, connected socket
		// 让服务器接收客户端的连接请求。
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&sin_size);
		if (client_sockfd < 0) {
			perror("accept client socket failed");
			exit(EXIT_FAILURE);
		}
		printf("accept done.\n");
		// inet_ntoa, 将网络地址转换成“.”点隔的字符串格式。
		printf("Client port: %d\n", client_address.sin_port);				// 客户端端口号
		printf("Client IP  : %s\n", inet_ntoa(client_address.sin_addr));	// 客户端 IP 地址

		int ret = 0;
		// 接收客户端数据
		// int recv(int sockfd, void *buf, int len, int flags)
		/*if ((ret = recv(client_sockfd, buf, BUFFER_SIZE, 0)) < 0) {*/
		if ((ret = read(client_sockfd, buf, BUFFER_SIZE)) < 0) {
			perror("recv client data failed");
			exit(EXIT_FAILURE);
		}
		printf("ret: %d, strlen(buf): %tu\n", ret, strlen(buf));

		printf("receive from client:%s\n",buf);
		// 发送数据到客户端
		/*if ((ret = send(client_sockfd, buf, strlen(buf), 0)) < 0) {*/
		// 这里如果使用 BUFFER_SIZE， 会发送多余的数据
		if ((ret = write(client_sockfd, buf, strlen(buf))) < 0) {
			perror("server send failed");
			exit(EXIT_FAILURE);
		}

		printf("Len: %d\n", ret);
		close(client_sockfd);				// 关闭连接
		//shutdown(int sockfd, int how);	// how 为 shutdown 操作的方式
		// how: 0:不允许继续接收数据,1:不允许继续发送数据,2:不允许继续发送和发送数据,均不允许则调用 close()
	}
    close(server_sockfd);

    exit(EXIT_SUCCESS);
}
