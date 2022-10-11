#实现MyVector的make

obj = main.o

mytest:$(obj)
	g++ $(obj) -o mytest 
main.o:main.cpp MyVector.h
	g++ -std=c++11 -c main.cpp -o main.o


.PHONY:git1 clean
git:
	git add main.cpp MyVector.h Makefile

clean:
	rm -f $(obj) mytest 


