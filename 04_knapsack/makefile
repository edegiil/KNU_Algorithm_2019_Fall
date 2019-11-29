all:
	gcc -c knapsack.c
	gcc -o exec knapsack.o
	./exec || ./exec.exe

clear:
	rm -rf exec *.o *.exe

help:
	@echo all : compile and execute
	@echo clear : clean build file