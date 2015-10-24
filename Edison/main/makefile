CC=gcc
ifeq ($(shell uname), Darwin)
	JSON_C_DIR=/usr/local
	CFLAGS += -I$(JSON_C_DIR)/include
	LDFLAGS += -L$(JSON_C_DIR)/lib -ljson-c
endif

ifeq ($(shell uname), Linux)
	JSON_C_DIR=/usr/local
	CFLAGS += -I$(JSON_C_DIR)/include
	LDFLAGS += -ljson-c -lmraa -lm
endif

objects = drone_main.o ./include/thread/thpool.o

drone_main : $(objects)
	$(CC) $(objects) -pthread -lcurl $(LDFLAGS) $(CFLAGS) -o drone_main

clean:
	rm -rf drone_main $(objects)
