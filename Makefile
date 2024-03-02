run:
	gcc -fsanitize=address -g -o main.o main.c ./*/*.c -I./*
	./main.o
