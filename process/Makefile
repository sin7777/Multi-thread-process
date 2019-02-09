multisum:processMain.o
	gcc -lpthread -o multisum processMain.o
processMain.o:processMain.c
	gcc -c processMain.c
.PHONY: clean
clean:
	rm *.o
test:
	./test.sh