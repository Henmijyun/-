#调用系统函数接口

mytest:main.o
	gcc main.o -o mytest 
main.o:main.c
	gcc -c main.c -o main.o

.PHONY:read clean
read:
	while :; do ps ajx | head -1 && ps ajx | grep mytest | grep -v grep; sleep 1; echo "-------------------------"; done
clean:
	rm -f *.o mytest 
