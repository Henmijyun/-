#实现myMap mySet的make

#obj = main.o

#.PHONYL:all
#all: testMap testSet


mytest:main.o
	g++ main.o -o mytest 
main.o:main.cpp myMap.h mySet.h RedBlackTree.h 
	g++ -std=c++11 -c main.cpp -o main.o


#testMap:mainMap.o
#	g++ mainMap.o -o testMap 
#mainMap.o:mainMap.cpp myMap.h RedBlackTree.h
#	g++ -std=c++11 -c mainMap.cpp -o mainMap.o

#testSet:mainSet.o
#	g++ mainSet.o -o testSet 
#mainSet.o:mainSet.cpp mySet.h RedBlackTree.h
#	g++ -std=c++11 -c mainSet.cpp -o mainSet.o


.PHONYL:git gdb clean
git:
	git add main.cpp myMap.h mySet.h RedBlackTree.h Makefile 

gdb:
	g++ -std=c++11 main.cpp -o main_debug -g

	
clean:
	rm -f *.o mytest *_debug 




