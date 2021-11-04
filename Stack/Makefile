TARGET = Stack

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): main.o
	gcc -o $(TARGET) main.o

main.o: main.c
	gcc -c -o main.o main.c