# 基于TCP协议,实现客户端client与服务器端server的实现

.PHONY:all
all:tcp_client tcp_server

tcp_client:tcp_client.cc
	g++ -std=c++11 tcp_client.cc -o tcp_client -lpthread

tcp_server:tcp_server.cc #./ThreadPool/%.cc
	g++ -std=c++11 tcp_server.cc -o tcp_server -lpthread

.PHONY:clean clientAB
clean:
	rm -rf tcp_server tcp_client  #clientA clientB

clientAB:
	mkfifo clientA clientB
