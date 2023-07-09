# 自定义协议: 计算器服务
# 守护进程化

.PHONY:all
all:CalClient CalServer

CalClient:CalClient.cc 
	g++ -std=c++11 CalClient.cc -o CalClient #-lpthread

CalServer:CalServer.cc 
	g++ -std=c++11 CalServer.cc -o CalServer -lpthread

.PHONY:clean
clean:
	rm -rf CalClient CalServer

	