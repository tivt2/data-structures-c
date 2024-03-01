run:
	gcc -fsanitize=address -g -o main.o main.c ./linked-list/*.c ./stack/*.c ./ring-buffer/*.c ./heap/*.c ./binary-tree/*.c -I./linked-list -I./stack -I./ring-buffer -I./heap -I./binary-tree
	./main.o
