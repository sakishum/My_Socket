/**
 * @file main.cpp
 * @Synopsis  获取多个节点IP, 比如google 就有好几个ip.
 *		URL: http://blog.csdn.net/huang_xw/article/details/8502895
 *		output:
 *		[2404:6800:4005:c00::68]:80
 *		173.194.127.83:80
 *		173.194.127.80:80
 *		173.194.127.84:80
 *		173.194.127.81:80
 *		173.194.127.82:80
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2014-05-22
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

int main(void) {
	// 定义IO 服务对象
	//  io_service 就是boost::asio 基于OS的I/O引擎， 其他的功能是建立在它之上的 。
	boost::asio::io_service my_io_service ;
	// 定义解析对象
	// resolver 接管了 DNS， 将开发者的查询（query） 转换为endpoint ( IP：Port) 地址。
	boost::asio::ip::tcp::resolver resolver(my_io_service);
	// 查询socket的相应信息
	// query 保存用户查询， 可以使主机名+服务命， 可以是单独的服务名， 可以是单独的主机名...
	boost::asio::ip::tcp::resolver::query query("www.baidu.com", "http");
	// 进行域名或者服务解释，以便生成SOCKET内部使用的数据格式
	boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end; // End marker.

	while (iter != end) {
		boost::asio::ip::tcp::endpoint endpoint = *iter++;
		std::cout << "Endpoint: " << endpoint << std::endl;
	}	

	return EXIT_SUCCESS;
}
