#OUTS:=$(shell find examples/ -name '*.c' | xargs basename -s'.c')
CXX		= g++
CXXFLAGS	= -g -Wall -std=c++11
EXTRAS		= lexer.cpp
OBJS		= lexer.o parser.o Type.o Symbol.o Scope.o checker.o
PROG		= scc

all:		$(PROG)	

$(PROG):	$(EXTRAS) $(OBJS) 
		$(CXX) -o $(PROG) $(OBJS)
	
clean:;		$(RM) $(PROG) core *.o

clobber:;	$(RM) $(EXTRAS) $(PROG) core *.o

#lexer.cpp:	lexer.l	
#		$(LEX) $(LFLAGS) -t lexer.l > lexer.cpp

test:
	for f in $(OUTS); do \
		./scc < "examples/$$f.c" > output.txt; \
		diff output.txt "examples/$$f.out"; \
	done
