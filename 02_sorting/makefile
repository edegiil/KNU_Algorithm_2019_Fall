selection:
	gcc -c selection_sort.c
	gcc -o selection_sort selection_sort.o
	./selection_sort || ./selection_sort.exe

mergearray:
	gcc -c merge_sort_array.c
	gcc -o merge_sort_array merge_sort_array.o
	./merge_sort_array || ./merge_sort_array.exe

merge:
	gcc -c merge_sort.c
	gcc -o merge_sort merge_sort.o
	./merge_sort || ./merge_sort.exe

clear:
	rm -rf exec *.o *.exe

help:
	@echo selection : execute selection sort with linked list (hw1)
	@echo mergearray : execute merge sort with array (hw2)
	@echo merge : execute merge sort with linked list (hw3)
	@echo clear : clean build file