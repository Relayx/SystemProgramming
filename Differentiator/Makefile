TARGET = Differentiator

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): treeDump.o main.o treeCalculate.o Tree2Latex.o
	gcc -o $(TARGET) treeDump.o main.o treeCalculate.o Tree2Latex.o

main.o: main.c
	gcc -c -o main.o main.c

treeDump.o: lib/treeDump.c
	gcc -c -o treeDump.o lib/treeDump.c

treeCalculate.o: lib/treeCalculate.c
	gcc -c -o treeCalculate.o lib/treeCalculate.c

Tree2Latex.o: lib/Tree2Latex.c
	gcc -c -o Tree2Latex.o lib/Tree2Latex.c