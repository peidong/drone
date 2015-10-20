objects = main.o

main : $(objects)
	gcc -o main $(objects) -lcurl -ljson-c 

main.o : http.h

clean:
	rm -rf main $(objects)
