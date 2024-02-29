run:
	gcc -fsanitize=address -g -o main.o main.c ./linked-list/*.c ./stack/*.c ./ring-buffer/*.c ./heap/*.c -I./linked-list -I./stack -I./ring-buffer -I./heap
	./main.o
