TARGET = equationSolver
EQUATION = ./Equation

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): equation.o main.o test
	gcc -o $(TARGET) equation.o main.o
	./equationTest.exe

main.o: main.c equation.o
	gcc -c -o main.o main.c

equation.o: Equation/equation.c Equation/equation.h
	gcc -c -o equation.o Equation/equation.c

test: equation.o
	gcc -o equationTest EquationTest/equation_test.c equation.o