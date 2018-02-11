#include "../includes/Scope.h"

/* Global varialbes for Scope LL defined below */
Scope * global;  //global scope i think
Scope * top;   //current scope?

/***********************************************/

/* Helpful strings for reporting errors */
std::string typeConflict = "conflicting types for %s";
std::string redef = "redefinition of %s";
std::string redecl = "redeclaration of %s";
std::string undecl = "%s undeclared"; 
std::string voidType = "%s has type void";

/***************************************************/
//typedef integer Type(INT, 0); //IDK IF THIS IS CORRECT

/* Function declarations */
static void declare_var(std::string name, Type t);
static void declare_func(std::string name, Type t);
static void define_func(std::string name, Type t);  //might not be needed

/*************************/ 


static void declare_var(std::string name, Type t)
{
    /*Might need to add to scope also */
    Scope *curr = top;
    Symbol *sym;
    sym = curr->find(name);
    /* if a local variable has been declared in
     * the current scope, return a redeclare error
     */
    if (sym){
        report(redecl, name);
        return;
    }

    /* while current scope is not NULL 
     * keepe looking for variable declaration
     */
    while (curr){
        sym = curr->find(name);
        /* symbol was found in a scope */
        if (sym){
            Type *found_type = sym->getType();
            /* if symbol exists and types not the same
             * issue a type error
             *
            if (*found_type != t){
                report(typeConflict, name);
                return;
            } */ 
        }
    }       

}

static void declare_func(std::string name, Type t)
{
    Symbol *sym;
    /* MAY NOT NEED
     * case where NOT in global scope so func
     * definition is illegal
     *
    if (toplevel != outermost){
        //put some shit here
    }
    */

    /* find function declaration in the global scope */
    sym = outermost->find(name);
    
    if(sym){
        Type *found_type = sym->getType();

        /* if the function signatures not identical, report
         * an error and return
         */
        if (*found_type != t){
           report(typeConflict, name);
           return;
        }

        /*case where the function has previously been defined,
         * report a redefine error and return
         */
        if (sym->getDef()){
            report(redef, name);
            return;
        }
    }
}

