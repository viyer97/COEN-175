INC	= includes/tokens.h includes/lexer.h
P1 	= phase1/lexer.l 
P2	= $(P1) phase2/parser.cpp
SUBMIT	= submissions
.PHONY: phase1
.PHONY: phase2

all: 
	$(MAKE) -C phase1
	$(MAKE) -C phase2

phase1:
	mkdir $(SUBMIT)/phase1
	@echo "Make sure to change the include statements!!!!!"
	cp phase1/Makefile $(P1) $(INC) $(SUBMIT)/phase1

phase2:
	mkdir $(SUBMIT)/phase2
	@echo "Make sure to change the include statements"
	cp phase2/Makefile $(P2) $(INC) $(SUBMIT)/phase2
