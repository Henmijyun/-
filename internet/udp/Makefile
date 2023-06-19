# 基于UDP协议,实现客户端client与服务器端server的实现

.PHONY:all
all:udp_client udp_server

udp_client:udp_client.cc
	g++ -std=c++11 udp_client.cc -o udp_client -lpthread

udp_server:udp_server.cc
	g++ -std=c++11 udp_server.cc -o udp_server -lpthread

.PHONY:clean clientAB
clean:
	rm -rf udp_server udp_client  clientA clientB

clientAB:
	mkfifo clientA clientB
