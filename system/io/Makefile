# IO mytest mycat myfile
.PHONYL:all 
all: mytest mycat myfile myfilecpp 

mytest:mytest.c
	gcc mytest.c -o mytest 

mycat:mycat.c
	gcc mycat.c -o mycat 

myfile:myfile.c
	gcc myfile.c -o myfile 

#myfile2:myfile2.c
#	gcc myfile2.c -o myfile2 

myfilecpp:myfile.cc 
	g++ -std=c++11 myfile.cc -o myfilecpp  

.PHONY:clean
clean:
	rm -rf *.o *.txt mytest mycat myfile myfilecpp 


