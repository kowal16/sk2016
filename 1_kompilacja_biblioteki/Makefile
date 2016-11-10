all: compilation_cycle

compilation_cycle: preprocesor compilator assembler linker

preprocesor: hello.c
	cpp hello.c > hello.i

compilator: hello.i
	gcc -Wall -S hello.i

assembler:	hello.s
	as hello.s -o hello.o

linker: hello.o
	gcc -v hello.o -o hello.dyn
	gcc -v hello.o -static -o hello.static

info:
	file hello.dyn
	file hello.static
	ldd hello.dyn
	strings hello.dyn
	strings hello.static

strip:
	strip hello.dyn
	strip hello.static

symbols:
	nm hello.dyn
	nm hello.static

shared_fib: fib.c
	gcc -fPIC -g -c -Wall fib.c -o fib.o
	gcc -shared -Wl,-soname,libfib.so -o libfib.so.1.0.1 fib.o 
	ldconfig -n .
	gcc -Wall -g  test_libfib.c -o test_shared_libfib.o -L. -lfib

static_fib: fib.c
	gcc -c -Wall fib.c -o fib.o
	ar rcs libfib.a fib.o
	gcc -static test_libfib.c -L. -lfib -o test_static_fib.o

clean:
	- rm *.i *.o *.dyn *.static *.so.* *.so *.s


