CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic
OBJFILES = dh_assign_1.o rsa_assign_1.o



TARGET = dh_assign_1 rsa_assign_1

all: $(TARGET)

dh_assign_1: dh_assign_1.o
	$(CC) $(CFLAGS) $(DBG) -o dh_assign_1 dh_assign_1.o -lgmp

rsa_assign_1: rsa_assign_1.o
	$(CC) $(CFLAGS) $(DBG) -o rsa_assign_1 rsa_assign_1.o -lgmp

clean:
	rm -f $(TARGET) *.o
