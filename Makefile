CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -lm
GCOV = -fprofile-arcs -ftest-coverage -O0 -fPIC
CHECK = -lcheck
VALGRIND = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CHECK += -lm -lpthread -lrt -lsubunit
endif

PROJECT_NAME = s21_string
SRC_DIR = .
OBJ_DIR = ./objs
TEST_DIR = ./tests
REPORT_DIR = ./report

SOURCE = $(wildcard $(SRC_DIR)/s21_*.c)
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCE:%.c=%.o)))
TESTS = $(wildcard $(TEST_DIR)/test_s21_*.check)
TESTC = $(TEST_DIR)/tests.c
TEST = $(TEST_DIR)/tests

default: all

all: clean $(PROJECT_NAME).a

$(PROJECT_NAME).a: $(OBJECTS)
	ar rcs $(PROJECT_NAME).a $(OBJECTS)
	ranlib $(PROJECT_NAME).a

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(GCOV) -c $< -o $@

check_style:
	clang-format -n -style=google *.c *.h
	# cppcheck --enable=all --suppress=missingIncludeSystem  --suppress=unusedFunction *.c *.h

clean:
	rm -rf $(PROJECT_NAME).a $(OBJ_DIR)/*.o $(OBJ_DIR)
	rm -rf $(TEST_DIR)/*.c $(TEST_DIR)/$(PROJECT_NAME).* $(TEST) $(TEST).o
	rm -rf $(REPORT_DIR)
	rm -rf *.gcno *.info *.o *.gcda test_cov *.txt

test: $(PROJECT_NAME).a $(TESTC)
	cp $(PROJECT_NAME).* $(TEST_DIR)
	$(CC) $(TESTC) $(PROJECT_NAME).a $(CHECK) -o $(TEST)
	$(TEST)

valgrind: test
	valgrind $(VALGRIND) --log-file=valgrind_report.txt $(TEST)

leaks: test
	leaks -atExit -- $(TEST) > leaks_report.txt

$(TESTC): $(TESTS)
	checkmk clean_mode=1 $(TEST_DIR)/includes.check $^ $(TEST_DIR)/pre_main.check > $@

gcov_report: $(SOURCE:%.c=%.o) $(TESTC)
	cp $(PROJECT_NAME).* $(TEST_DIR)
	$(CC) $(CFLAGS) -c $(TESTC) -o $(TESTC:%.c=%.o)
	$(CC) $(SOURCE:%.c=%.o) $(TESTC:%.c=%.o) $(GCOV) $(CHECK) -o test_cov
	./test_cov
	lcov -t "test_$(PROJECT_NAME)" -o test_$(PROJECT_NAME).info -c -d .
	genhtml -o $(REPORT_DIR) test_$(PROJECT_NAME).info

.PHONY: all clean test gcov_report default clang_format valgrind leaks