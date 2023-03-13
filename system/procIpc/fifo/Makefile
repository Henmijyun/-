# 命名管道的模拟实现

.PHONY:all

all:client server

client: client.cxx
	g++ -std=c++11 client.cxx -o client 
server: server.cxx
	g++ -std=c++11 server.cxx -o server 

.PHONY:clean
clean:
	rm -rf client server 

