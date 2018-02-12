#include "../includes/Scope.h"
#include "../includes/lexer.h"
#include "../includes/checker.h"
#include "../includes/tokens.h"

/* Global varialbes for Scope LL defined below */
Scope *global = NULL;  //global scope i think
Scope *top = NULL;   //current scope?

/***********************************************/

/* Helpful strings for reporting errors */
std::string typeConflict = "conflicting types for %s";
std::string redef = "redefinition of %s";
std::string redecl = "redeclaration of %s";
std::string undecl = "%s undeclared"; 
std::string voidType = "%s has type void";

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

        /*case where the function has previously been defined,
         * report a redefine error and return
         */
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

void call_func(std::string name)
{
    Symbol *sym;
    sym = global->find(name);
    /* if the symbol is not in the table, report undeclared and return */
    if (!sym){
        report(undecl, name);
        return;
    }
    /* if the symbol is in the table, but is not a function,
     * report undeclared
     */
    if (!sym->getType()->isFunction()){
        report(undecl, name);
        return;
    }
    
}

void call_var(std::string name)
{
    Symbol *sym;
    Scope *curr = top;
    while (curr){
        sym = curr->find(name);
        /* case where we found the variable declared
         * in current or enclosing scope
         */
        if (sym){
            return;
        }
        curr = curr->enclosing();
    }
    report(undecl, name);
}
