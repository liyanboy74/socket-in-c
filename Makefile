TARGET=test
CC=gcc

SRC= *.c

LINK= -lws2_32 #-lwsock32

default:campile run
campile:
	$(CC) $(SRC) $(LINK) -o $(TARGET)
clean:
	rm *.exe
run:
	./$(TARGET)