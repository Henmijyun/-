mytest:main.o
	g++ main.o -o mytest 
main.o:main.cpp
	g++ -std=c++11 -c main.cpp -o main.o

.PHONY:clean
clean:
	rm -f *.o mytest


