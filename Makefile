INC	= includes/*
P1 	= phase1/lexer.l 
P2	= $(P1) phase2/parser.cpp
P3	= $(P1) phase3/parser.cpp phase3/Type.cpp phase3/Symbol.cpp phase3/Scope.cpp phase3/checker.cpp
P4 	= $(P1) phase4/parser.cpp phase4/Type.cpp phase4/Symbol.cpp phase4/Scope.cpp phase4/checker.cpp
SUBMIT	= submissions
.PHONY: phase1
.PHONY: phase2
.PHONY: phase3
.PHONY: phase4

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

phase3:
	mkdir $(SUBMIT)/phase3
	@echo "Make sure to change the include statements"
	cp phase3/Makefile $(P3) $(INC) $(SUBMIT)/phase3

phase4:
	mkdir $(SUBMIT)/phase4
	@echo "CHNAGE THE INCLUDE STATEMENTS"
	cp phase4/Makefile $(P4) $(INC) $(SUBMIT)/phase4
	
