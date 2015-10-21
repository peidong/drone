CC=gcc
ifeq ($(shell uname), Darwin)
	JSON_C_DIR=/usr/local
	CFLAGS += -I$(JSON_C_DIR)/include
	LDFLAGS += -L$(JSON_C_DIR)/lib -ljson-c
endif

ifeq ($(shell uname), Linux)
	JSON_C_DIR=/usr/local
	CFLAGS += -I$(JSON_C_DIR)/include
	LDFLAGS += -ljson-c
endif

objects = main.o

main : $(objects)
	$(CC) -o main $(objects) -lcurl $(LDFLAGS) $(CFLAGS)

main.o : http.h

clean:
	rm -rf main $(objects)
