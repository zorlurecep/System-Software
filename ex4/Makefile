test:
	mkdir -p build
	gcc -Wall -Werror -o ./build/output main.c dplist.c
	./build/output

debug_ddd:
	mkdir -p debug
	gcc -g -o ./debug/output dplist.c main.c
	ddd ./debug/output &

gdbgui_debug:
	mkdir -p debug	
	gcc dplist.c main.c -g -o ./debug/output
	echo "/home/roza/Documents/source/repos/recepomer.zorlu/lab4/ex4/debug/output" | xclip -selection clipboard

valgrind_check:
	gcc -Wall -Werror -o ./build/output main.c dplist.c
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./build/output