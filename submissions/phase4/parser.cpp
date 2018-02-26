#include <stdlib.h>
#include <iostream>
#include <string>
#include "tokens.h"
#include "lexer.h"
#include "Scope.h"
#include "checker.h"
#define EVER (;;)

/*TODO: 
 * may not need parameter_lst function
 * no idea when to use 'lvalue' variable
 * don't know how to handle the call to exp_brack() is prefix expr rule
 * figure out how to issue E4 for prefix expression & 
 *
 * REMEMBER THAT ASSIGNMENT FUNCTION hardcoded lvalue
 */


int la;

unsigned num_match(int t); 
std::string id_match(int t);

void error();   //done  TESTed
void match(int t);  //done  TESTed
void exp_lst();// AYBE 
Type exp_t(bool &lvalue);//done MAYBE   TESTed
Type exp_brack(bool &lvalue);//done TESTed
Type exp_unary(bool &lvalue);//done TESTed
Type exp_mul(bool &lvalue);//done   TESTed
Type exp_add(bool &lvalue);//done   TESTed
Type exp_ineq(bool &lvalue);//done  TESTed
Type exp_eq(bool &lvalue);//done    TESTed
Type exp_and(bool &lvalue);//done   TESTed
Type exp_or(bool &lvalue);//done    TESTED
void assignment();//done    TESTED
void stmt(const Type &ret_type);//done  TESTED
void stmt_s(const Type &ret_type);//done    TESTED
void declarator(int typespec);//done    TESTED
void declaration();//done
void declaration_s();
void param(Parameters *param_vector);//done
void param_lst();
void param_s_prime(Parameters *param_vector);
Parameters * param_s();
int specifier();
unsigned ptrs();
void global_dec(int typespec);
void trans_unit();  //rule for translation unit
inline void rest(int typespec);

void error()
{
    report("syntax error at %s", yytext);
}


unsigned num_match(int t)
{
    char *ptr;
    unsigned len = strtoul(yytext, &ptr, 0);
    match(t);
    return len;
} 

std::string id_match(int t)
{
    std::string iden = yytext;
    match(ID);
    return iden;
}


void match(int t)
{
   if (t == la)
       la = yylex();
   else
       error();
}


Type exp_t(bool &lvalue)
{

    if (la == ID){
        std::string id = id_match(ID);
        Symbol *sym;

        /*
         * if it is a function call, we need to check arguments and 
         * verify that it is a valid function call
         */
        if (la == '('){
            sym = call_var(id);
            Parameters arg_list;       /* might need to use 'new' here */
             
            match('(');
            if (la != ')'){ 

                /* create vector of arugments */

                /* push first argument */
                Type first_arg = exp_or(lvalue);
                arg_list.push_back(first_arg);
                
                while(la == ','){
                    match(',');
                    bool temp_value = false;
                    Type other_arg = exp_or(temp_value); 
                    arg_list.push_back(other_arg);
                }
            }
            match(')');
            Type expression = checkFunc(*sym->getType(), arg_list);
            lvalue = false;
            return expression;
        }

        sym = call_var(id);
        if (sym->getType()->isScalar()) {
            lvalue = true; 
        } else {                /* this may not be necessary */
            lvalue = false;
        }
        return *(sym->getType());
    } else if (la == NUM){
        match(NUM);
        //num_match(NUM);
        lvalue = false;
        return Type(INT, 0);    //returns an integer
    } else if (la == STRING){
        unsigned int str_length = sizeof(yytext);
        match(STRING);
        lvalue = false;
        return Type(CHAR, 0, str_length);
    } else if (la == CHARACTER){
        match(CHARACTER);
        lvalue = false;
        return Type(INT, 0); 
    } else if (la == '('){
        /* this shouldn't modify lvalue */
        match('(');
        Type expression = exp_or(lvalue);
        match(')');
        return expression;
    }

    //return Type();
}

/* this function is never used */
/*void exp_lst()
{
    exp_or();
    while(la == ','){
        match(',');
        exp_or();  //might change to exp_lst if tail recursive
    }
} */


Type exp_brack(bool &lvalue)
{
    Type left = exp_t(lvalue);

    while (la == '['){  
        match('[');
        bool temp_val = false;
        Type right = exp_or(temp_val);
        left = checkPostfix(left, right);   //don't think i need to pass anything extra in
        match(']');
        lvalue = true;
    }

    return left;
}

/* I don't know how to edit this one */
Type exp_unary(bool &lvalue)
{
    if (la == '&') {
        match('&');
        Type right = exp_unary(lvalue);
//        if (right.isArray() || right.isFunction() 
        right = checkPrefix(right, "&", lvalue);        /*i don't know if this is correct */
        lvalue = false;
        return right;
    } else if (la == '!') {
        match('!');
        Type right = exp_unary(lvalue);
        right = checkPrefix(right, "!", lvalue);        /*i don't know if this is correct */
        lvalue = false;
        return right;
    } else if (la == '-') {
        match('-');
        Type right = exp_unary(lvalue);
        right = checkPrefix(right, "-", lvalue);        /*i don't know if this is correct */
        lvalue = false;
        return right;
    } else if (la == '*') {
        match('*');
        Type right = exp_unary(lvalue);
        right = checkPrefix(right, "*", lvalue);        /*i don't know if this is correct */
        lvalue = true;                  /* this change was in the assignment instructions */
        return right;
    } else if (la == SIZEOF){
        match(SIZEOF);
        Type right = exp_unary(lvalue);
        right = checkPrefix(right, "sizeof", lvalue);        /*i don't know if this is correct */
        lvalue = false;
        return right;
    } else {
        Type right = exp_brack(lvalue);   //don't know if this is correct either lol
        return right;
    }

    //return right;
}


Type exp_mul(bool &lvalue)
{
    //not sure the star thing works properly
    Type left = exp_unary(lvalue);

    for EVER{
        if (la == '*'){
            match('*');
            Type right = exp_unary(lvalue);
            left = checkMul(left, right, "*");
            lvalue = false;
        } else if (la == '/'){
            match('/');
            Type right = exp_unary(lvalue);
            left = checkMul(left, right, "/");
        } else if (la == '%'){
            match('%');
            Type right = exp_unary(lvalue);
            left = checkMul(left, right, "%");
        } else {
            break;
        }
    }

    return left;
}


Type exp_add(bool &lvalue)
{
    Type left = exp_mul(lvalue);

    for EVER{
        if (la == '+'){
            match('+');
            Type right = exp_mul(lvalue);
            left = checkAdd(left, right, "+");  //watch out for single quotes, may lead to issue
            lvalue = false; 
        } else if (la == '-'){
            match('-');
            Type right = exp_mul(lvalue);
            left = checkAdd(left, right, "-");
            lvalue = false;
        } else {
            break;
        }
    }

    return left;
}
        

Type exp_ineq(bool &lvalue)
{
    Type left = exp_add(lvalue);

    for EVER{
        if (la == '>'){
            match('>');
            Type right = exp_add(lvalue);
            left = checkRelation(left, right, ">");
            lvalue = false;
        } else if (la == '<'){
            match('<');
            Type right = exp_add(lvalue);
            left = checkRelation(left, right, "<");
            lvalue = false;
        } else if (la == LTE){
            match(LTE);
            Type right = exp_add(lvalue);
            left = checkRelation(left, right, "<=");
            lvalue = false;
        } else if (la == GTE){
            match(GTE);
            Type right = exp_add(lvalue);
            left = checkRelation(left, right, ">=");
            lvalue = false;
        } else {
            break;
        }
    }

    return left;
}


Type exp_eq(bool &lvalue)
{
    Type left = exp_ineq(lvalue);

    for EVER{
        if (la == IS_EQ){
            match(IS_EQ);
            Type right = exp_ineq(lvalue);
            left = checkEQ(left, right, "==");    //check operands of '==' are ok
            lvalue = false;
        } else if (la == NOT_EQ){
            match(NOT_EQ);
            Type right = exp_ineq(lvalue);
            left = checkEQ(left, right, "!=");  //check operands of '!=' are ok
            lvalue = false; 
        } else {
            break;
        }
    }

    return left;
}


Type exp_and(bool &lvalue)
{
    Type left = exp_eq(lvalue);
    
    while (la == LAND){
        match(LAND);
        Type right = exp_eq(lvalue);
        left = checkLogicalAND(left, right);
        lvalue = false;
    }
    return left;
}

/* primary expression */
Type exp_or(bool &lvalue)
{
    Type left = exp_and(lvalue);

    while (la == LOR){
        match(LOR);
        Type right = exp_and(lvalue);
        left = checkLogicalOR(left,right);
        lvalue = false;
    }
    return left;    //left must be Type INT
}


void assignment()
{
    bool lvalue = true;
    Type left = exp_or(lvalue);   //this must be an lvalue

    if (la == '='){
        match('=');
        bool temp_val = false;
        Type right = exp_or(temp_val);
        checkAssign(left, right, lvalue);
    }
}

void stmt(const Type &ret_type)
{
    if (la == '{'){
        match('{');
        open_scope();   //open scope block
        declaration_s();
        stmt_s(ret_type);
        match('}');
        close_scope();
        return;
    } if (la == BREAK){
        match(BREAK);
        checkBreak();   //check to see if break statement in loop
        match(';');
    } else if (la == RETURN){
        match(RETURN);
        bool temp_value = false;
        Type expression = exp_or(temp_value);
        checkReturn(expression, ret_type);
        match(';');
    } else if (la == WHILE){
        enterLoop();
        match(WHILE);
        match('(');
        bool temp_value = false;
        Type expression = exp_or(temp_value);
        checkLoop(expression);
        match(')');
        stmt(ret_type);
        exitLoop();
    } else if (la == FOR){
        enterLoop();
        match(FOR);
        match('(');
        assignment();
        match(';');
        bool temp_val = false;
        Type expression = exp_or(temp_val);
        checkLoop(expression);
        match(';');
        assignment();
        match(')');
        stmt(ret_type);
        exitLoop();
    } else if (la == IF){
        match(IF);
        match('(');
        bool temp_value = false;
        Type expression = exp_or(temp_value);
        checkLoop(expression);
        match(')');
        stmt(ret_type);
        if (la == ELSE){
            match(ELSE);
            stmt(ret_type);
        }
    } else {
        assignment();
        match(';');
    }
}


void stmt_s(const Type &ret_type)
{
    while (la != '}'){
        stmt(ret_type);
    }
}


void declarator(int typespec)
{
    unsigned ind = ptrs();
    std::string id = id_match(ID); 
    unsigned len = 0;
    if (la == '['){
        match('[');
        len = num_match(NUM);
        match(']');
        Type *t = new Type(typespec, ind, len);
        declare_var(id, t);
    } else {
        Type *t = new Type(typespec, ind);
        declare_var(id, t);   //this might segfault
    }
}


void declaration()
{
    int typespec = specifier();
    declarator(typespec);
    while (la == ','){  //might need to be changed to if and recur
        match(',');
        declarator(typespec);
    } 
    match(';');
}

void declaration_s()
{
    while (la == INT || la == CHAR || la == VOID){
        declaration();
    }
}


void param(Parameters *param_vector)
{
    int typespec = specifier();
    int ind = ptrs();
    //match(ID);
    std::string id = id_match(ID);
    Type *t = new Type(typespec, ind);
    declare_var(id, t);
    param_vector->push_back(*t);
    //std::cout << ind << id << std::endl;
   // std::cout << "parameter" << std::endl;
}

/*
void param_lst()
{
    param();
    while (la == ','){  
        match(',');
        param();
    }
    //std::cout << "parameter list" << std::endl;
}*/


void param_s_prime(Parameters *param_vector)
{
    if (la == ','){
        match(',');
        if (la  == TRIPLE_DOT){
            Type *t = new Type(TRIPLE_DOT);
            param_vector->push_back(*t);
            match(TRIPLE_DOT);
            return;
        }
        param(param_vector);
        param_s_prime(param_vector);
    }
}


Parameters * param_s()
{
    int typespec = 0;
    unsigned ind = 0; 
    Parameters *param_vector = new Parameters;
    if (la == VOID){
        typespec = specifier();
        if (la != ')'){
            ind = ptrs();
            //match(ID);
            std::string id = id_match(ID);
            Type *t = new Type(typespec, ind);
            param_vector->push_back(*t);
            declare_var(id, t);
            param_s_prime(param_vector);
            return param_vector;
        } 
        /* in case f(void) */
        Type *t = new Type(typespec, 0);
        param_vector->push_back(*t);
    } else if (la == CHAR){
       // match(CHAR);
        typespec = specifier();
        ind = ptrs();
        //match(ID);
        std::string id = id_match(ID);
        Type *t = new Type(typespec, ind);
        param_vector->push_back(*t);
        declare_var(id, t);
        param_s_prime(param_vector);
    } else if (la == INT){
        //match(INT);
        typespec = specifier();
        ind = ptrs();
        //match(ID);
        std::string id = id_match(ID);
        Type *t = new Type(typespec, ind);
        param_vector->push_back(*t);
        declare_var(id, t);
        param_s_prime(param_vector);
    }
    return param_vector;
}
       

int specifier()
{
    int typespec = 0;
    if (la == INT){
        typespec = la;
        match(INT);
    } else if (la == CHAR){
        typespec = la;
        match(CHAR);
    } else {
        typespec = la;
        match(VOID);
    }
    return typespec;
}


unsigned ptrs()
{
    unsigned count = 0;
    while (la == '*'){
        match('*');
        ++count;
    }
    return count;
}


void global_dec(int typespec)
{
    unsigned ind = ptrs();
    unsigned len = 0;
    //match(ID);
    std::string id = id_match(ID);
    /* case where we are declaraing anothe rfunction */
    if (la == '('){
        open_scope();
        Parameters *func_params;
        match('(');
        func_params = param_s();
        match(')');
        Type *t = new Type(typespec, ind, func_params); 
        declare_func(id, t);
        close_scope();
        return;
    } else if (la == '['){
        /* case where we have multiple global arrays */
        match('[');
        len = num_match(NUM);
        match(']');
        Type *t = new Type(typespec, ind, len);
        declare_var(id, t);
        return;
    }
    Type *t = new Type(typespec, ind);
    declare_var(id, t);
    //std::cout << ind << id << len << std::endl;
}

/*
 * returns void
 * params none
 *
 * This function is for the translation unit rule
 */
void trans_unit()
{
    int typespec = specifier();
    int ind = ptrs();
    //match(ID);
    std::string id = id_match(ID);
    int len = 0;
    
    /*
     * first if is matching global arrays
     * second if is matching function declarations/definitions
     */
    if (la == '['){
        match('[');
        len = num_match(NUM);
        match(']');
        Type *t = new Type(typespec, ind, len);
        declare_var(id, t);
    //    std::cout << "global array" << std::endl;
    } else if (la == '('){
        //std::cout << "open function scope" << std::endl;
        //match(VOID);
        open_scope();
        Parameters *func_params;
        match('(');
        func_params = param_s();
        match(')');
        Type *t = new Type(typespec, ind, func_params); 

        /*
         * Case when looking at function definition
         * so we only match until end brace
         * and don't need to call rest()
         */
        if (la == '{'){
            define_func(id, t);
            match('{');
            declaration_s();
            stmt_s(Type(typespec, ind));
            match('}');
            close_scope();
            return;
        }
        declare_func(id, t);
        close_scope();
        //std::cout << "close function scope" << std::endl;
    } else {
        Type *t = new Type(typespec, ind);
        declare_var(id, t);
    }
    rest(typespec);
}


inline void rest(int typespec)
{
    for EVER{
        if (la == ';'){
            match(';');
            break;
        } else if (la == ','){
            match(',');
            global_dec(typespec);
        }
    }
}


int main(int argc, char *argv[])
{
    //std::cout << "open global scope" << std::endl;
    open_scope();
    la = yylex();
    while(la != 0){
        trans_unit();
    }
    //std::cout << "close global scope" << std::endl;
    close_scope();
    return 0;
}
