// http_demo 从读取请求到响应请求

#include <iostream>
#include <memory>
#include <cassert>
#include <vector>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "HttpServer.hpp"
#include "Usage.hpp"
#include "Util.hpp"

// 一般http都要有自己的web根目录，如果只请求/，返回默认首页
#define ROOT "./wwwroot"
#define HOMEPAGE "index.html"


void HandlerHttpRequest(int sockfd)
{
	// 1.读取请求 for test
	char buffer[1024];
	ssize_t s = recv(sockfd, buffer, sizeof(buffer)-1, 0);
	if (s > 0)
	{
		buffer[s] = 0;
		// std::cout << buffer << "---------------------\n" << std::endl;
	}

	// 2.状态行内容截取
	std::vector<std::string> vline;
	Util::cutString(buffer, "\r\n", &vline);  // 每一行截取出来

	std::vector<std::string> vblock;
	Util::cutString(vline[0], " ", &vblock);  // 第一行3个子串分别截取, 方法 url路径 版本

	// std::cout << "####### start ########" << std::endl;
	// for (auto& iter : vblock)
	// {
	// 	std::cout << "---" << iter << "\n" << std::endl;
	// }
	// std::cout << "######## end #######" << std::endl;

	// 3.首页路径设置
	std::string file = vblock[1];
	std::string target = ROOT; 

	if (file == "/")
	{
		file = "/index.html";
	}
	target += file;  // ./wwwroot/index.html
	std::cout << target << std::endl; 

	// 4.打开文件流--读取html文件 （C/C++）
	// int fd = open(target.c_str(), O_RDONLY);
	// if (fd < 0)
	// 	// 打开失败，不存在
	// else
	// 	// 打开成功

	std::string content;
	std::ifstream in(target);  // 打开C++文件流
	if (!in.is_open())
	{
		// 打开失败，不存在
		std::cout << "打开文件流失败" << std::endl;
	}
	else
	{
		// 打开成功，读取
		std::string line;
		while (std::getline(in, line))  // 从流中读取到line
		{
			content += line;
		}
		in.close();  // 关闭文件流
	}
	
	// 5.构建一个http的响应
	std::string HttpResponse;
	if (content.empty())
	{
		// HttpResponse = "HTTP/1.1 404 NotFound\r\n";  // 状态行
		HttpResponse = "HTTP/1.1 302 Found\r\n";   // 302临时跳转
		HttpResponse += "Location: http://52.68.195.117:8080/a/404.html\r\n";  // 跳转的网站
	}
	else
	{
		HttpResponse = "HTTP/1.1 200 OK\r\n";  // 状态行
		HttpResponse += "Content-Type: text/html\r\n";   // 请求数据类型 （参考对照表）
		HttpResponse += ("Content-Length: " + std::to_string(content.size()) + "\r\n");  // 正文长度的属性
		HttpResponse += "Set-Cookie: 这是一个cookie\r\n";  // cookie	
	}
	// 现代的浏览器很强，会给缺失的报头属性进行识别补充
	// 只是为了test可以不写
	HttpResponse += "\r\n";  // 空行
	HttpResponse += content;  // 正文
	// HttpResponse += "<html><h3>hello skk!</h3></html>";  // 正文
	send(sockfd, HttpResponse.c_str(), HttpResponse.size(), 0);   // 返回数据

}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		Usage(argv[0]);
		exit(0);		
	}

	std::unique_ptr<HttpServer> httpserver(
		new HttpServer(atoi(argv[1]), HandlerHttpRequest)
		);
	httpserver->Start();

	return 0;
}
