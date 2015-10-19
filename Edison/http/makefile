objects = main.o

main : $(objects)
	gcc -o main $(objects) -lcurl

main.o : http.h

clean:
	rm -rf main $(objects)
