CC = gcc
CFLAGS = -Wall -Wextra -g3
TARGET = output/main
SRCS = main.c funcoes.c funcoesSecundarias.c utilities.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS) | output
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

output:
	mkdir -p output

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
