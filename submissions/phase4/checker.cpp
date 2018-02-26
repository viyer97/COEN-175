#include "Scope.h"
#include "lexer.h"
#include "checker.h"
#include "tokens.h"
#include "Type.h"

/* Global varialbes for Scope LL defined below */
Scope *global = NULL;  //global scope i think
Scope *top = NULL;   //current scope?

static const Type error;

int in_loop = 0;    //check if inside for/while loop (need for break stmt)
/***********************************************/

/* Helpful strings for reporting errors */
std::string typeConflict = "conflicting types for '%s'";
std::string redef = "redefinition of '%s'";
std::string redecl = "redeclaration of '%s'";
std::string undecl = "'%s' undeclared"; 
std::string voidType = "'%s' has type void";

std::string badBreak = "break statement not within loop";
std::string badRetType = "invalid return type";
std::string badTestExpr = "invalid type for test expression";
std::string badLvalue = "lvalue required in expression";
std::string badBinaryOp = "invalid operands to binary %s";
std::string badUnaryOp = "invalid operand to unary %s";
std::string badFuncCall = "called object is not a function";
std::string badArgs = "invalid arguments to called function";
/***************************************************/

//typedef integer Type(INT, 0); //IDK IF THIS IS CORRECT


void declare_var(std::string name, Type *t)
{
    /*Might need to add to scope also */
    Symbol *sym;
    Symbol *curr_symbol = new Symbol(name, t, false);
    sym = top->find(name);

    /* if we are at the global scope, make sure declaration
     * doesn't have type conflicts wit other declarations
     */
    if (top == global){
        if (sym){
            Type *found_type = sym->getType();

            /* if symbol exists and types not the same
             * issue a type error
             */
            if (*found_type != *t){
                report(typeConflict, name);
                return;
            }  

            
            //global.insert(curr_symbol); //insert symbol into global scope
            //return;
        }
        if ((t->specifier() == VOID) && (t->indirection() == 0)){
            Type *error_type = new Type();
            Symbol *error_symbol = new Symbol(name, error_type, false);
            top->insert(error_symbol);   //insert variable into scope
            report(voidType, name);
            return;
        }

        /* if symbol is not found, insert it into the table at global scope */
        top->insert(curr_symbol);
        return;
    }

    /* if a local variable has been declared in
     * the current scope, return a redeclare error
     */
    if (sym){
        report(redecl, name);
        return;
    }
    
    if ((t->specifier() == VOID) && (t->indirection() == 0)){
        report(voidType, name);
        top->insert(curr_symbol);   //insert variable into scope
        return;
    }
    top->insert(curr_symbol);   //insert variable into scope
    return;
}

void declare_func(std::string name, Type *t)
{
    Symbol *sym;

    /* find function declaration in the global scope */
    sym = global->find(name);
    Symbol *curr_symbol = new Symbol(name, t, false);
    
    if(sym){
        Type *found_type = sym->getType();

        /* if the function signatures not identical, report
         * an error and return
         */
        if (*found_type != *t){
           report(typeConflict, name);
           return;
        }

        /* all new redeclarations are discarded, so return */
        return;
    }
    global->insert(curr_symbol);
}

void define_func(std::string name, Type *t)
{
    Symbol *sym;
    sym = global->find(name);
    if (sym){
        Type *found_type = sym->getType();
        if (*found_type != *t){
           report(typeConflict, name);
           return;
        }    
        else if (sym->getDef()){
            report(redef, name);
            return;
        }
        delete sym;
    } 
    Symbol *curr_symbol = new Symbol(name, t, true);
    global->insert(curr_symbol);
}

void open_scope()
{
    Scope *newScope = new Scope(top);
    if (global == NULL){
        global = newScope;
    }
    top = newScope;
}

void close_scope()
{
    top = top->enclosing();
}

Symbol * call_func(std::string name)
{
    Symbol *sym;
    sym = global->find(name);
    /* if the symbol is not in the table, report undeclared and return */
    if (!sym){
        report(undecl, name);
        Type *err_type = new Type;
        std::string new_name = name;
        sym = new Symbol(new_name, err_type, false);
        global->insert(sym);
    }
    /* if the symbol is in the table, but is not a function,
     * report undeclared 
     * DON'T NEED THIS
     * 
    if (!sym->getType()->isFunction()){
        report(badFuncCall);
        return NULL;
    }
    */
    return sym; 
}


Symbol * call_var(std::string name)
{
    Symbol *sym;
    Scope *curr = top;
    while (curr){
        sym = curr->find(name);
        /* case where we found the variable declared
         * in current or enclosing scope
         */
        if (sym){
            return sym;
        }
        curr = curr->enclosing();
    }
    report(undecl, name);
    Type *err_type = new Type;
    sym = new Symbol(name, err_type, false);     //if symbol is not found, return a null ptr
    top->insert(sym);
    return sym;
}

bool checkVoidDeref(const Type &t) {
    if ((t.specifier() == VOID) && (t.indirection() <= 1)) {
        return true;
    }
    
    return false;
}

Type checkLogicalOR(const Type &left, const Type &right)
{
    if (left == error || right == error) {
        return error;
    }

    if (left.isValue() && right.isValue()) {
        return Type(INT, 0);
    } 
    
    report(badBinaryOp, "||");
    return error;
}

Type checkLogicalAND(const Type &left, const Type &right)
{
    if (left == error || right == error) {
        return error;
    }
    if (left.isValue() && right.isValue()) {
        return Type(INT, 0);
    } 

    report(badBinaryOp, "&&");
    return error;
}

Type checkEQ(const Type &left, const Type &right, std::string op)
{
    if (left == error || right == error) {
        return error;
    }

    Type new_left = left;
    Type new_right = right;

    /* if the two types are not compatible after promotion, report an error */
    if (new_left.isCompatible(new_right)) {
        return Type(INT, 0);
    }

    /* return an integer as result*/
    report(badBinaryOp, op);   
    return error;
}

Type checkRelation(const Type &left, const Type &right, std::string op)
{
    if (left == error || right == error) {
        return error;
    }

    Type new_left = left.promote();
    Type new_right = right.promote();

    /* operands must have identical types after promotion */
    if (new_left == new_right){
        return Type(INT, 0);
    }

    report(badBinaryOp, op);
    return error;
}

Type checkAdd(const Type &left, const Type &right, std::string op)
{
    if (left == error || right == error) {
        return error;
    }

    Type new_left = left.promote();
    Type new_right = right.promote();

    /* case where both sides are integers */
    if (new_left.isInteger() && new_right.isInteger()) {
        return Type(INT, 0);
    }

    /*
     * case where left is ptr to T and right is int, ret ptr to T
     * Make sure that left is not a pointer to void
     */
    if (new_left.isPointer() && !checkVoidDeref(new_left) && new_right.isInteger()) {
        return new_left;
    }

    /* FOR ADDITION ONLY: if left is int and right is ptr to T, ret ptr to T */
    if (op == "+") {
        if (new_left.isInteger() && new_right.isPointer() && !checkVoidDeref(new_right)) {
            return new_right;
        }
    } else if (op == "-") {
        /* for subtrction only: if both are ptr to T, T is not void, and
         * T is identical for both operands, ret int
         */
        if (new_left.isPointer() && new_right.isPointer() 
                && !checkVoidDeref(new_left) && !checkVoidDeref(new_right)) {
            if (new_left == new_right) {
                return Type(INT, 0);
            }
        }
    } 

    report(badBinaryOp, op);
    return error;
}

Type checkMul(const Type &left, const Type &right, std::string op)
{
    if (left == error || right == error) {
        return error;
    }
    
    Type new_left = left.promote();
    Type new_right = right.promote();

    /* if left and right are both ints after promotion, return int */
    if(new_left.isInteger() && new_right.isInteger()) {
        return Type(INT, 0);
    }

    report(badBinaryOp, op);
    return error;
}


Type checkPrefix(const Type &right, std::string op, const bool &lvalue)
{
    if (right == error) {
        return error;
    }

    Type new_right = right.promote();

    if (op == "*") {
        if (new_right.isPointer() && !checkVoidDeref(new_right)) {
            return Type(new_right.specifier(), new_right.indirection() - 1);
        }

    } else if (op == "&") {
        /* 
         * if op is & and right is type T, ret ptr to T 
         * Operand does not undergo promotion
         */
        if (!lvalue) {
            report(badLvalue);
            return error;
        }

        return Type(right.specifier(), right.indirection() + 1); 
    } else if (op == "!") {
        if (right.isValue()) {
            return Type(INT, 0);
        }
    } else if (op == "-") {
        if (new_right.isInteger()) {
            return Type(INT, 0);
        }
    } else if (op == "sizeof") {
        if (right.isValue()) { 
            return Type(INT, 0);
        }
    }

    report(badUnaryOp, op);
    return error;
}

Type checkPostfix(const Type &left, const Type &right)
{
    if (left == error || right == error) {
        return error;
    }
    
    Type new_left = left.promote();
    Type new_right = right.promote();

    if (new_left.isPointer() && !checkVoidDeref(new_left) 
            && new_right.isInteger()) {
        return Type(new_left.specifier(), new_left.indirection() - 1);
    }

    report(badBinaryOp, "[]");
    return error;
}

Type checkFunc(const Type &func_type, const Parameters &args)
{
    /* if called object is not a function, report an error */
    if (!func_type.isFunction()) {
        report(badFuncCall);
        return error;
    }

    /* if called function has only argument (void) */
    Parameters *checkVoid = func_type.parameters();
    if ((func_type.parameters()->size() == 1) && ((*checkVoid)[0].specifier() == VOID)
            && !(*checkVoid)[0].indirection()) {

        return Type(func_type.specifier(), func_type.indirection());
    }

    /* DONT NEED
     * promote all arugments in decl
    Parameters::iterator it;
    Parameters decl_args;

    for(it = func_type.parameters->begin(); it != func_type.parameters->end();
            it++) {
        Type new_arg = (*it).promote();
        decl_args.push_back(new_arg);
    } 
    */

    /* promote all arguments in args */
    Parameters promoted_args;
    for(int i = 0; i < args.size(); i++) {
        Type new_arg = args[i].promote();
        promoted_args.push_back(new_arg);
    }


    Parameters *it = func_type.parameters();

    /* if the last argument in the function signature is NOT an ellipsis */
    if (!(func_type.parameters()->back().isEllipsis())) {
        if (promoted_args.size() != func_type.parameters()->size()) {
            report(badArgs);
            return error;
        }

        for(int i = 0; i < args.size(); i++) {
            if (!(promoted_args[i].isCompatible((*it)[i]))) {
                report(badArgs);
                return error;
            }
        }
    } else {
        /* case where the last argument in function signature is ellipsis */

        /* if there are fewer arguments in function call, report an error */
        if (promoted_args.size() < (func_type.parameters()->size() - 1)) {
            report(badArgs);
            return error;
        }
        
        /*reset it pointer to beginning */
        it = func_type.parameters();

        /* check to make sure all values up until the ellipsis are compatible */
        for(int i = 0; i < func_type.parameters()->size()-1; i++) {
            if (!(promoted_args[i].isCompatible((*it)[i]))) {
                report(badArgs);
                return error;
            }
        }
    }

    return Type(func_type.specifier(), func_type.indirection());
}

void checkBreak() {
    /* if we are not in a loop, report an error */
    if (!in_loop) {
        report(badBreak);
    }
}

void enterLoop() 
{
    in_loop++;
}

void exitLoop() 
{
    in_loop--;
}

void checkLoop(const Type &t)
{
    if (!t.isValue()) {
        report(badTestExpr);
    }
}

void checkAssign(const Type &left, const Type &right, bool lvalue) {
    if (!lvalue) {
        report(badLvalue);
    }
    Type new_left = left.promote();
    
    if (new_left == error || right == error) {
        return;
    }

    if (!new_left.isCompatible(right)) { 
        report(badBinaryOp, "=");
    }
}

void checkReturn(const Type &expression, const Type &ret_type) {
    Type new_expr = expression;
    if (!new_expr.isCompatible(ret_type)) { 
        report(badRetType);
    }
}
