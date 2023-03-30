# mysignal的实现和test

mysignal: mysignal.o
	g++ mysignal.o -o mysignal
mysignal.o: mysignal.cc mysignal.hpp
	g++ -std=c++11 -c mysignal.cc -o mysignal.o

.PHONY: clean
clean:
	rm -rf mysignal mysignal.o

