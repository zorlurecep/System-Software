HEADERS=dplist.h
SOURCE=dplist.c
EXE=build/dplist

FLAGS = -std=c11 -Werror -lm $(shell pkg-config --cflags --libs check) -D DEBUG

test : dplist.c dplist_test.c
	@echo -e '\n*******************************'
	@echo -e '*** Compiling for UNIT TEST ***'
	@echo -e '*******************************'
	mkdir -p build
	gcc dplist.c dplist_test.c -o build/dplist_test $(FLAGS)
	@echo -e '\n*************************'
	@echo -e '*** Running UNIT TEST ***'
	@echo -e '*************************'
	./build/dplist_test

.PHONY : clean check

check : $(SOURCE) $(HEADERS)
	@echo "Running cppcheck :"
	cppcheck --enable=all --suppress=missingIncludeSystem --error-exitcode=1 $(SOURCE) $(HEADERS)

clean:
	rm -f *.o $(EXE) $(EXE)_test *~

valgrind_check:
	gcc -Wall -Werror -o ./build/output main.c dplist.c
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./build/output

# To debu in gdb use the following command
# set environment CK_FORK=no
debug_gdb:
	mkdir -p debug
	gcc dplist.c dplist_test.c -g -o ./debug/output `pkg-config --cflags --libs check`
	gdb -tui ./debug/output

test_2: $(PARAMS)
	mkdir -p build
	gcc -Wall -Werror -o ./build/output main.c dplist.c
	./build/output

debug_gdbgui:
	mkdir -p debug	
	gcc dplist.c main.c -g -o ./debug/output
	echo "/home/roza/Documents/source/repos/recepomer.zorlu/lab4/ex1/debug/output" | xclip -selection clipboard
	
debug_ddd:
	mkdir -p debug
	gcc -g -o ./debug/output dplist.c main.c
	ddd ./debug/output &
