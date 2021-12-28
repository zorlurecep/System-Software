# if you created a subfolder for every exercise this target will
# build and execute the main.c file in the ex3 folder
compile_program:
	mkdir -p build
	gcc -Wall -Werror -o ./build/output main.c ma_malloc.c
	./build/output

debug_program:
	mkdir -p debug
	gcc -g -o ./debug/output main.c ma_malloc.c
	echo "/home/roza/Documents/source/repos/recepomer.zorlu/lab3/debug/output" | xclip -selection clipboard

debug_program_ddd:
	mkdir -p debug
	gcc -g -o ./debug/output main.c ma_malloc.c
	ddd ./debug/output &

# the main file in the ex3 folder will be ziped and is then ready to
# be submitted to labtools.groept.be
zip:
	rm lab3.zip
	zip ./lab3.zip ma_malloc.c ma_malloc.h

cppCheck: 
	cppcheck --enable=all --suppress=missingIncludeSystem main.c
	cppcheck --enable=all --suppress=missingIncludeSystem ma_malloc.c
	cppcheck --enable=all --suppress=missingIncludeSystem ma_malloc.h