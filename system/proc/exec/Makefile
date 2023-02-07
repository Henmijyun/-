# 进程等待 + 进程替换

.PHONY:all
all:exec mycmd

mycmd:mycmd.c
	gcc -o $@ $^
exec:exec.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f exec mycmd 




