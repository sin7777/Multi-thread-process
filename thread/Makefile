multisum:main.o
	gcc -lpthread -o multisum main.o
main.o:main.c
	gcc -c main.c
.PHONY: clean
clean:
	rm *.o
test:
	./test.sh