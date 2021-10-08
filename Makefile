ex1: main.c
	mkdir -p build
	gcc -Wall -Werror -o ./build/ex1 ./ex1/main.c
	./build/ex1

ex2: 
	gcc -Wall -Werror -0 ex2 ./ex2/main.c
	./ex2/ex2

zip: main.c
	zip lab1_ex1.zip main.c
