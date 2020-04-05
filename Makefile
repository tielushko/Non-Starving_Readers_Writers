rwmain:  main.o
	gcc -pthread main.o -o rwmain
main.o: main.c
	gcc -c main.c
clean:
	rm *.o rwmain