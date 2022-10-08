#这是关于TestString的make
obj = main.o

mytest:$(obj)
	g++ $(obj) -o mytest 
main.o:main.cpp MyString.h
	g++ -std=c++11 -c main.cpp -o main.o

.PHONY:git1 git2 git3 clean 

git1:
	git add main.cpp MyString.h Makefile 
git2:
	git commit -m "更新MyString" 
git3:
	git push

clean:
	rm -f mytest $(obj)


