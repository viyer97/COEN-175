#OUTS:=$(shell find testfiles/ -name '*.c' | xargs basename -s'.c')

all: 
	lex -t program.l > lexer.cpp
	gcc -o scc lexer.cpp

noise:
	@echo $(OUTS)
	for each in $(OUTS); do \
		echo "$$each.c"; \
	done

test:
	lex -t program.l > lexer.cpp
	gcc -o scc lexer.cpp
	./scc < testfiles/extra.c > output.txt; \
	

build:
	lex -t program.l > lexer.cpp
	gcc -o scc lexer.cpp
	for f in $(OUTS); do \
		./scc < "testfiles/$$f.c" > output.txt; \
		diff output.txt "testfiles/$$f.out"; \
	done

clean:
	-rm lexer.cpp
	-rm scc
	-rm output.txt
