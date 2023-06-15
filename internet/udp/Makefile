.PHONY:all
all:udp_client udp_server

udp_client:udp_client.cc
	g++ -std=c++11 udp_client.cc -o udp_client -lpthread

udp_server:udp_server.cc
	g++ -std=c++11 udp_server.cc -o udp_server -lpthread

.PHONY:clean
clean:
	rm -rf udp_server udp_client
	