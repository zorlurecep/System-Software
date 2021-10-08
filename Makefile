exercise1: ./ex1/main.c	
	mkdir -p build
	gcc -Wall -Werror -o ./build/ex1 ./ex1/main.c
	./build/ex1

exercise3: ./ex3/main.c	
	gcc -Wall -Werror -o ./ex3/ex3.out ./ex3/main.c
	./ex3/ex3.out	

zip: main.c
	zip lab1_ex1.zip main.c
