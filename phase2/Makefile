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