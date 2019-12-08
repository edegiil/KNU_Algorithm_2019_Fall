all:
	gcc -c maximumPoints.c
	gcc -o exec maximumPoints.o
	./exec || ./exec.exe

clear:
	rm -rf exec *.o *.exe

help:
	@echo all : compile and execute
	@echo clear : clean build file