#这是关于myString的make
obj = main.o

mytest:$(obj)
	g++ $(obj) -o mytest 
main.o:main.cpp myString.h
	g++ -std=c++11 -c main.cpp -o main.o

.PHONY:git1 git2 git3 gdb clean 

git1:
	git add main.cpp myString.h Makefile 
git2:
	git commit -m "更新myString" 
git3:
	git push

gdb:
	g++ -std=c++11  main.cpp -o main_debug -g 

clean:
	rm -f mytest $(obj) *_debug


