multisum:threadMain.o
	gcc -lpthread -o multisum threadMain.o
threadMain.o:threadMain.c
	gcc -c threadMain.c
.PHONY: clean
clean:
	rm *.o
test:
	./test.sh