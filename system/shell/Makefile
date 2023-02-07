# 调用系统的进程替换，实现myShell
.PHONY:all
all: myShell myTest


myShell:myshell.c
	gcc myshell.c -o myShell 
	
myTest:mytest.c
	gcc mytest.c -o myTest 


.PHONY:clean
clean:
	rm -f *.o myShell myTest  
