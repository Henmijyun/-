# 动静态库的生成
.PHONY:all
all: libhello.a libhello.so 

# 动态库
libhello.so: mymath_d.o myprint_d.o 
	gcc -shared mymath_d.o myprint_d.o -o libhello.so 
mymath_d.o: mymath.c 
	gcc -c -fPIC mymath.c -o mymath_d.o 
myprint_d.o: myprint.c
	gcc -c -fPIC myprint.c -o myprint_d.o 


# 静态库
libhello.a: mymath.o myprint.o
	ar -rc libhello.a mymath.o myprint.o
mymath.o: mymath.c
	gcc -c mymath.c -o mymath.o
myprint.o: myprint.c
	gcc -c myprint.c -o myprint.o


# 归纳，便于打包发布
.PHONY: output
output:
	mkdir -p output/lib 
	mkdir -p output/include 
	cp -rf *.h output/include 
	cp -rf *.a output/lib 
	cp -rf *.so output/lib 



.PHONY: clean
clean:
	rm -rf *.o *.a *.so output

