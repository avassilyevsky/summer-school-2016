all: file1 file2

file1: file1.o bin_sem.o
	gcc file1.o bin_sem.o -o file1

file2: file2.o bin_sem.o
	gcc file2.o bin_sem.o -o file2

file1.o: File1.c
	gcc -c File1.c -o file1.o

file2.o: File2.c
	gcc -c File2.c -o file2.o

bin_sem.o: bin_sem.c
	gcc -c bin_sem.c

clean:
	rm -f *.o file1 file2
