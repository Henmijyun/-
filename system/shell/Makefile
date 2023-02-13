# 调用系统的进程替换，实现myShell
.PHONY:all
all: myShell myTest myShellIO 


myShell:myshell.c
	gcc myshell.c -o myShell 
	
myTest:mytest.c
	gcc mytest.c -o myTest 

myShellIO:myshell_io.c
	gcc myshell_io.c -o myShellIO

.PHONY:clean
clean:
	rm -f *.o *.txt myShell myTest myShellIO 
