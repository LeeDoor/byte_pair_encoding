CFLAGS=-Wall -Wextra -pedantic -g
EXEC_NAME=bpe

bpe:
	gcc *.c -o $(EXEC_NAME) $(CFLAGS)
tests:
	gcc hash_map.c hash_map_test.c -o test $(CFLAGS)
clean:
	rm -rf *.o $(EXEC_NAME) *.txt *.out
