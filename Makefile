TARGET=test
CC=gcc

SRC= *.c

LINK= -lws2_32 #-lwsock32

default:campile
campile:
	$(CC) $(SRC) $(LINK) -o $(TARGET)
clean:
	rm *.exe
run:
	./$(TARGET)