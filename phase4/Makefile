CXX		= g++
CXXFLAGS	= -g -Wall -std=c++11
EXTRAS		= lexer.cpp
OBJS		= Scope.o Symbol.o Type.o checker.o lexer.o parser.o 
PROG		= scc

all:		$(PROG)	

$(PROG):	$(EXTRAS) $(OBJS) 
		$(CXX) -o $(PROG) $(OBJS)
	
clean:;		$(RM) $(PROG) core *.o

clobber:;	$(RM) $(EXTRAS) $(PROG) core *.o

lexer.cpp:	../phase1/lexer.l	
		$(LEX) $(LFLAGS) -t ../phase1/lexer.l > lexer.cpp

test:
	for f in $(OUTS); do \
		./scc < "examples/$$f.c" > output.txt; \
		diff output.txt "examples/$$f.out"; \
	done
