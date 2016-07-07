TARGETS= f_pointer pthread_test pthread_test2
FLAGS= -lpthread
all: $(TARGETS)

f_pointer: f_pointer.c
	gcc f_pointer.c -o f_pointer
pthread_test: pthread_test.c
	gcc pthread_test.c -o pthread_test -lpthread
pthread_test2: pthread_test2.c
	gcc pthread_test2.c -o pthread_test2 -lpthread

clean:
	rm -f *.o $(TARGETS)
