TARGET = Differentiator

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): treeDump.o main.o treeCalculate.o tree2Latex.o utilities.o differentiator.o treeParse.o optimizer.o
	gcc -o $(TARGET) treeDump.o main.o treeCalculate.o tree2Latex.o utilities.o differentiator.o treeParse.o optimizer.o

main.o: main.c
	gcc -c -o main.o main.c

treeDump.o: lib/treeDump.c
	gcc -c -o treeDump.o lib/treeDump.c

treeCalculate.o: lib/treeCalculate.c
	gcc -c -o treeCalculate.o lib/treeCalculate.c

tree2Latex.o: lib/tree2Latex.c
	gcc -c -o tree2Latex.o lib/tree2Latex.c

utilities.o: lib/utilities.c
	gcc -c -o utilities.o lib/utilities.c

differentiator.o: lib/differentiator.c
	gcc -c -o differentiator.o lib/differentiator.c

treeParse.o: lib/treeParse.c
	gcc -c -o treeParse.o lib/treeParse.c

optimizer.o: lib/optimizer.c
	gcc -c -o optimizer.o lib/optimizer.c