CFLAGS=-Wall -Wextra -pedantic -g
EXEC_NAME=bpe

all:
	gcc *.c -o $(EXEC_NAME) $(CFLAGS)
clean:
	rm -rf *.o $(EXEC_NAME) *.txt *.out
