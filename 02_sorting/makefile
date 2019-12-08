arraysort:
	gcc -c arraysort.c
	gcc -o array_sort arraysort.o
	./array_sort || ./array_sort.exe

listsort:
	gcc -c listsort.c
	gcc -o list_sort listsort.o
	./list_sort || ./list_sort.exe

heavyarraysort:
	gcc -c heavy_arraysort.c
	gcc -o heavy_arraysort heavy_arraysort.o
	./heavy_arraysort || ./heavy_arraysort.exe

heavylistsort:
	gcc -c heavy_listsort.c
	gcc -o heavy_listsort heavy_listsort.o
	./heavy_listsort || ./heavy_listsort.exe

clear:
	rm -rf heavy_listsort heavy_arraysort list_sort array_sort *.o *.exe output/*

help:
	@echo arraysort : execute sort with array
	@echo listsort : execute sort linked list
	@echo heavyarraysort : execute sort with array and count time
	@echo heavyarraysort : execute sort linked list and count time
	@echo clear : clean build file