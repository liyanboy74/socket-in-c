TARGET=test
CC=gcc

SRC= *.c

default:clean campile run
campile:
	$(CC) $(SRC) -o $(TARGET)
clean:
	rm *.exe
run:
	./$(TARGET)