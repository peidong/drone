JSON_C_DIR=/usr/local
CFLAGS += -I$(JSON_C_DIR)/include
LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c

objects = main.o

main : $(objects)
	gcc -o main $(objects) -lcurl $(LDFLAGS) $(CFLAGS)

main.o : http.h

clean:
	rm -rf main $(objects)
