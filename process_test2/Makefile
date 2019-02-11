multisum:fock.o
	gcc -lpthread -o multisum fock.o
fock.o:fock.c
	gcc -c fock.c
.PHONY: clean
clean:
	rm *.o