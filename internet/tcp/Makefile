# 基于TCP协议,实现客户端client与服务器端server的实现

.PHONY:all
all:tcp_client tcp_server
tcp_client:tcp_client.cc
	g++ -std=c++11 tcp_client.cc -o tcp_client -lpthread

tcp_server:tcp_server.cc
	g++ -std=c++11 tcp_server.cc -o tcp_server -lpthread

.PHONY:clean clientAB netstat 
clean:
	rm -rf tcp_server tcp_client #clientA clientB

# 创建命名管道
clientAB:
	mkfifo clientA clientB

# 查询TCP链接情况
netstat:
	netstat -lntp 



