#ifndef CHECKER_H
#define CHECKER_H
extern Scope *global;
extern Scope *top;

void declare_var(std::string name, Type *t);
void declare_func(std::string name, Type *t);
void define_func(std::string name, Type *t);
void call_var(std::string name);
void call_func(std::string name);

void open_scope();
void close_scope();

#endif
