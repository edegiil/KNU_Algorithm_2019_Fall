all:
	gcc -c bst.c
	gcc -o exec bst.o
	./exec || ./exec.exe

clear:
	rm -rf exec *.o *.exe

help:
	@echo all : compile and execute
	@echo clear : clean build file