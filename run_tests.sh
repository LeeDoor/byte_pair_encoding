make tests
valgrind --track-origins=yes --leak-check=full ./test
