TARGET = Onegin

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): fileUtilities.o main.o
	gcc -o $(TARGET) fileUtilities.o main.o

main.o: main.c fileUtilities.o
	gcc -c -o main.o main.c

fileUtilities.o: fileUtilities.c includes/fileUtilities.h
	gcc -c -o fileUtilities.o fileUtilities.c
