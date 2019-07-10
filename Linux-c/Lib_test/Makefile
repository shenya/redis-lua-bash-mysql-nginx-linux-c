all: libmy_math.a libmy_math.so
	gcc -o a.out main.c -L. -lmy_math -Wl,-rpath=.

libmy_math.a: my_math.o
	ar rcs libmy_math.a  my_math.o

libmy_math.so: my_math.o
	gcc -shared -o libmy_math.so my_math.o

my_math.o: my_math.c
	gcc -fPIC -c my_math.c -g


clean:
	rm *.o
	rm *.a
	rm *.so*
	rm a.out
