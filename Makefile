rwmain:  main.o
	g++ -pthread main.o -o rwmain
main.o: main.c
	g++ -c main.c
clean:
	rm *.o rwmain