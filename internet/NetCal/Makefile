# 自定义协议: 计算器服务

.PHONY:all
all:client server

client:CalClient.cc 
	g++ -std=c++11 CalClient.cc -o CalClient #-lpthread

server:CalServer.cc 
	g++ -std=c++11 CalServer.cc -o CalServer -lpthread

.PHONY:clean
clean:
	rm -rf CalClient CalServer

	