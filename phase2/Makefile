OUTS:=$(shell find examples/ -name '*.c' | xargs basename -s'.c')
CXX		= g++
CXXFLAGS	= -g -Wall
EXTRAS		= lexer.cpp
OBJS		= lexer.o parser.o
PROG		= scc

all:		$(PROG)	

$(PROG):	$(EXTRAS) $(OBJS) 
		$(CXX) -o $(PROG) $(OBJS)
	
clean:;		$(RM) $(PROG) core *.o

clobber:;	$(RM) $(EXTRAS) $(PROG) core *.o

lexer.cpp:	../phase1/lexer.l	
		$(LEX) $(LFLAGS) -t ../phase1/lexer.l > lexer.cpp

lextest:	lextest.cpp lexer.o
		$(CXX) -o lextest lextest.cpp lexer.o

test:
	for f in $(OUTS); do \
		./scc < "examples/$$f.c" > output.txt; \
		diff output.txt "examples/$$f.out"; \
	done
