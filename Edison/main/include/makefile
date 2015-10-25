CC=gcc
ifeq ($(shell uname), Darwin)
	JSON_C_DIR=/usr/local
	CFLAGS += -I$(JSON_C_DIR)/include
	LDFLAGS += -L$(JSON_C_DIR)/lib -ljson-c
endif

ifeq ($(shell uname), Linux)
	JSON_C_DIR=/usr/local
	CFLAGS += -I$(JSON_C_DIR)/include
	LDFLAGS += -ljson-c -lmraa
endif

objects = drone_include.o ./thread/thpool.o

drone_include : $(objects)
	$(CC) $(objects) -pthread -lcurl -lm $(LDFLAGS) $(CFLAGS) -o drone_include

clean:
	rm -rf drone_include $(objects)
