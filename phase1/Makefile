all: build

build:
	lex -t program.l > lexer.cpp
	gcc -o scc lexer.cpp
