#ifndef CHECKER_H
#define CHECKER_H
extern Scope *global;
extern Scope *top;

void declare_var(std::string name, Type *t);
void declare_func(std::string name, Type *t);
void define_func(std::string name, Type *t);
Symbol * call_var(std::string name);
Symbol * call_func(std::string name);

void open_scope();
void close_scope();

bool checkVoidDeref(const Type &t);
Type checkLogicalOR(const Type &left, const Type &right);
Type checkLogicalAND(const Type &left, const Type &right);
Type checkEQ(const Type &left, const Type &right, std::string op);
Type checkRelation(const Type &left, const Type &right, std::string op);
Type checkAdd(const Type &left, const Type &right, std::string op);
Type checkMul(const Type &left, const Type &right, std::string op);
Type checkPrefix(const Type &right, std::string op, const bool &lvalue);
Type checkPostfix(const Type &left, const Type &right);
Type checkFunc(const Type &func_type, const Parameters &args);

void exitLoop();
void enterLoop();
void checkBreak();
void checkLoop(const Type &t);
void checkAssign(const Type &left, const Type &right, bool lvalue);
void checkReturn(const Type &expression, const Type &ret_type);

#endif
