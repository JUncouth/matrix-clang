CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
SRC = src/arithmetic.c src/helpers.c
OBJ = $(SRC:.c=.o)
LIB = matrix.a
TEST_SRC = tests/test.c
TEST_EXE = test_matrix
CHECK_LIBS = -lcheck -pthread -lm
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage --coverage
LDFLAGS =

.PHONY: all clean test gcov_report valgrind

all: matrix.a

matrix.a: $(OBJ)
	   ar rcs $(LIB) $(OBJ)

%.o: %.c
	   $(CC) $(CFLAGS) -c $< -o $@

test: clean all
	   $(CC) $(CFLAGS) $(COVERAGE_FLAGS) $(TEST_SRC) $(SRC) -o $(TEST_EXE) $(LDFLAGS) $(CHECK_LIBS)
	   ./$(TEST_EXE)

gcov_report: test
	   @echo "Generating gcov report..."
	   gcovr --exclude tests --html --html-details -o coverage.html .

valgrind: clean
	   $(CC) $(CFLAGS) $(TEST_SRC) $(SRC) -o $(TEST_EXE) $(LDFLAGS) $(CHECK_LIBS)
	   valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose \
	            --log-file=valgrind_log.txt ./$(TEST_EXE)
	   @echo "Valgrind report saved to valgrind_log.txt"
	   @echo "Summary:"
	   @tail -20 valgrind_log.txt || true

clean:
	   rm -f *.o *.a src/*.o src/*.a $(TEST_EXE) coverage*.{info,html,css,js} *.gc{no,da} *.gc{no,da} valgrind_log.txt
