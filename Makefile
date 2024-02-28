run:
	gcc -fsanitize=address -g -o main.o main.c ./linked-list/*.c ./stack/*.c ./ring-buffer/*.c -I./linked-list -I./stack -I./ring-buffer
	./main.o
