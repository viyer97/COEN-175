#OUTS:=$(shell find testfiles/ -name '*.c' | xargs basename -s'.c')

all: 
	lex -t lexer.l > lexer.cpp
	g++ -o scc lexer.cpp lextest.cpp

noise:
	@echo $(OUTS)
	for each in $(OUTS); do \
		echo "$$each.c"; \
	done

test:
	lex -t program.l > lexer.cpp
	g++ -o scc lexer.cpp
	./scc < testfiles/hello.c > output.txt;
	diff output.txt testfiles/hello.out
	

build:
	lex -t program.l > lexer.cpp
	g++ -o scc lexer.cpp
	for f in $(OUTS); do \
		./scc < "testfiles/$$f.c" > output.txt; \
		diff output.txt "testfiles/$$f.out"; \
	done

clean:
	-rm lexer.cpp
	-rm scc
	-rm output.txt
