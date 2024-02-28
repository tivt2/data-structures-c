run:
	gcc -fsanitize=address -g -o main.o main.c ./linked-list/*.c ./stack/*.c -I./linked-list -I./stack
	./main.o
