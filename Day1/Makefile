TARGET =
PREFIX = ./
SRCS = app_main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET)
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)
