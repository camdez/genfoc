CC=g++
CPP_FLAGS=-Wall
PROGNAME=genfoc

$(PROGNAME): main.o expression.o
	$(CC) -o $(PROGNAME) main.o expression.o

clean:
	$(RM) *~ *.o $(PROGNAME)
