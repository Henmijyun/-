mytest:MyString.o main.o
	g++ main.o MyString.o -o mytest 
MyString.o:MyString.cpp
	g++ -c MyString.cpp -o MyString.o
main.o:mian.cpp 
	g++ -c main.cpp -o main.o

.PHONY:clean
clean:
	rm -f *.o mytest 

	

