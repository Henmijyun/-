# 利用管道控制共享内存 

.PHONY:all

all:shmClient shmServer

shmClient: shmClient.cc
	g++ -std=c++11 shmClient.cc -o shmClient 
shmServer: shmServer.cc
	g++ -std=c++11 shmServer.cc -o shmServer 

.PHONY:clean
clean:
	rm -rf shmClient shmServer 

