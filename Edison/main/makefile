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

objects = drone_main.o

drone : $(objects)
	$(CC) $(objects) -pthread -lcurl $(LDFLAGS) $(CFLAGS) -o drone_main

objects_demo = demo_main.o

demo : $(objects_demo)
	$(CC) $(objects_demo) -pthread -lcurl $(LDFLAGS) $(CFLAGS) -o demo_main

objects_bbb = drone_beaglebone.o

bbb: $(objects_bbb)
	$(CC) $(objects_bbb) -pthread -lmraa -lm -o drone_beaglebone

clean:
	rm -rf drone_main demo_main drone_beaglebone $(objects) $(objects_demo) $(objects_bbb)
