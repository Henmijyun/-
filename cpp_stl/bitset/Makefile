#实现myBitset的make

obj = main.o

mytest:$(obj)
	g++ $(obj) -o mytest 
main.o:main.cpp myBitset.h BloomFilter.h
	g++ -std=c++11 -c main.cpp -o main.o


.PHONY:git gdb clean
git:
	git add main.cpp myBitset.h Makefile BloomFilter.h

gdb:
	g++ -std=c++11  main.cpp -o main_debug -g  

clean:
	rm -f $(obj) mytest *_debug


