#实现myVector的make

obj = main.o

mytest:$(obj)
	g++ $(obj) -o mytest 
main.o:main.cpp myVector.h reverseIterator.h
	g++ -std=c++11 -c main.cpp -o main.o


.PHONY:git1 gdb clean 
git:
	git add main.cpp myVector.h reverseIterator.h Makefile

gdb:
	g++ -std=c++11  main.cpp -o main_debug -g

clean:
	rm -f $(obj) mytest *_debug 


