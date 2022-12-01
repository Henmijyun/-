#实现myMap mySet的make

#obj = main.o

.PHONYL:all
all: testMap testSet

testMap:mainMap.o
	g++ mainMap.o -o testMap 
mainMap.o:mainMap.cpp myMap.h RedBlackTree.h
	g++ -std=c++11 -c mainMap.cpp -o mainMap.o


testSet:mainSet.o
	g++ mainSet.o -o testSet 
mainSet.o:mainSet.cpp mySet.h RedBlackTree.h
	g++ -std=c++11 -c mainSet.cpp -o mainSet.o


.PHONYL:git gdbMap gdbSet clean
git:
	git add mainMap.cpp mainSet.cpp myMap.h mySet.h RedBlackTree.h Makefile 

gdbMap:
	g++ -std=c++11 mainMap.cpp -o mainMap_debug -g

gdbSet:
	g++ -std=c++11 mainSet.cpp -o mainSet_debug -g
	
clean:
	rm -f *.o test* *_debug 




