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

objects = pwm.o ../include/thread/thpool.o
pwm : $(objects)
	$(CC) -o pwm $(objects) -lcurl $(LDFLAGS) $(CFLAGS) -pthread

clean:
	rm -rf pwm $(objects)
