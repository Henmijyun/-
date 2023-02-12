# IO mytest mycat myfile
.PHONYL:all 
all: mytest mycat myfile

mytest:mytest.c
	gcc mytest.c -o mytest 

mycat:mycat.c
	gcc mycat.c -o mycat 

myfile:myfile.c
	gcc myfile.c -o myfile 

.PHONY:clean
clean:
	rm -f mytest mycat myfile


