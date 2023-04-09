CC= gcc
CFLAGS= -Wall -Werror -Wextra
SOURCES= untitled3/cal.c untitled3/calc.c untitled3/other_function_for_calculate.c untitled3/reverse_polish.c untitled3/stack.c 
OBJECTS=$(SOURCES:.c=.o)
OS= $(shell uname)
TEST_FILES= tests/*.c
PROJECT_NAME= smart_calc

ifeq ($(OS), Darwin)
	TEST_FLAGS=-lcheck
else
	TEST_FLAGS=-lcheck -lpthread -pthread -lrt -lm -lsubunit
endif

all: install

install: clean
	rm -rf build untitled3.app
	mkdir build
	cd build && qmake ../untitled3/untitled3.pro&&make&&mv untitled3.app ../untitled3.app
	cd build&&rm -rf *.*&&rm -rf Makefile&&rm -rf .qmake.stash
	rm -rf uninstall/untitled3.pro.user
	chmod 777 untitled3.app/
	open untitled3.app/Contents/MacOS/untitled3

	
uninstall:
	rm -rf build
	

test: clean $(PROJECT_NAME).a
		$(CC) $(TEST_FLAGS) $(CFLAGS)  $(PROJECT_NAME).a $(TEST_FILES) -o test -g
		./test

gcov_report: clean 
		$(CC) --coverage $(TEST_FLAGS) $(CFLAGS) $(SOURCES) $(TEST_FILES) -o test
		
		./test
		lcov -t "test" -o test.info -c -d . 
		genhtml -o report test.info
	
ifeq ($(OS), Darwin)
	open report/index.html
else
	xdg-open ./report/index.html
endif

clean:
	rm -rf test *.o *.gcda *.gcno *.a build-untitled3-Desktop_x86_darwin_generic_mach_o_64bit-Debug untitled3.app
	cd untitled3; rm -rf *.a *.so *.out *.o report *.gc* *.info *.txt *.cmake test *.gch *.pdf *.tar  
	rm -rf  *.html *.css test.info test.dSYM report
	rm -rf untitled3/html

dist: install
	tar --totals -cvf smart_calc.tar untitled3 tests


dvi:
	cd untitled3&&Doxygen
	open untitled3/html/index.html

$(PROJECT_NAME).a: clean
		$(CC) $(CFLAGS) $(SOURCES) -c 
		ar rcs $(PROJECT_NAME).a *.o

rebuild:
	make clean
	make all


linter:
	cp ../materials/linters/.clang-format ..
	clang-format -i tests/*.c tests/*.h untitled3/*.c untitled3/*.h
	clang-format -n tests/*.c tests/*.h untitled3/*.c untitled3/*.h
	rm -rf ../.clang-format
