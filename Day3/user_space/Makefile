TARGET= sync_test
FLAGS= -lpthread
all: $(TARGET)

sync_test: sync_test.c
	gcc sync_test.c -o $(TARGET) -lpthread

clean:
	rm -f *.o $(TARGET)
