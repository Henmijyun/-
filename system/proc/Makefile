# 调用系统进程接口，实现多进程分支运行

.PHONY:all
all: myProcC myProcCpp 


myProcC:main.c
	gcc main.c -o myProcC 
myProcCpp:main.cc 
	g++ -std=c++11 main.cc -o myProcCpp 
	


.PHONY:read clean
read:
	while :; do ps ajx | head -1 && ps ajx | grep mytest | grep -v grep; sleep 1; echo "-------------------------"; done
clean:
	rm -f *.o myProc* 
