#实现myList的make

obj = main.o

mytest:$(obj)
	g++ $(obj) -o mytest 
main.o:main.cpp myList.h reverseIterator.h
	g++ -std=c++11 -c main.cpp -o main.o


.PHONY:git clean
git:
	git add main.cpp myList.h reverseIterator.h Makefile

clean:
	rm -f $(obj) mytest 


