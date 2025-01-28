flags = -Wall -pedantic -fstack-protector-strong

$(cc):
	gcc -c $(flags) $(cc).c
	gcc $(cc).o -o $(cc).out
clean:
	rm *.o *.out
