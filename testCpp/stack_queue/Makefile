#实现myStack myQueue myPriorityQueu的make

obj = main.o

mytest:$(obj)
	g++ $(obj) -o mytest 
main.o:main.cpp myStack.h myQueue.h myPriorityQueue.h
	g++ -std=c++11 -c main.cpp -o main.o


.PHONY:git clean
git:
	git add main.cpp myStack.h myQueue.h myPriorityQueue.h Makefile

clean:
	rm -f $(obj) mytest 


