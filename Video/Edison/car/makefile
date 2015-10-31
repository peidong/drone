JSON_C_DIR=/usr/local
CFLAGS += -I$(JSON_C_DIR)/include
LDFLAGS += -ljson-c -lmraa -lm

car :
	gcc car_control.c -o car_control -pthread -lcurl $(LDFLAGS) $(CFLAGS)

clean :
	rm -rf car_control
