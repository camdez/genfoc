CXXFLAGS=-Wall
PROGNAME=genfoc

$(PROGNAME): main.o expression.o
	$(CXX) -o $(PROGNAME) main.o expression.o

clean:
	$(RM) *~ *.o $(PROGNAME)
