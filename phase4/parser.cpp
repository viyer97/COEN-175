#include <stdlib.h>
#include <iostream>
#include <string>
#include "../includes/tokens.h"
#include "../includes/lexer.h"
#include "../includes/Scope.h"
#include "../includes/checker.h"
#define EVER (;;)

/*TODO: 
 * may not need parameter_lst function
 */


int la;

unsigned num_match(int t); 
std::string id_match(int t);

void error();   //done  TESTed
void match(int t);  //done  TESTed
void exp_lst();// AYBE 
void exp_t();//done MAYBE   TESTed
Type exp_brack(bool &lvalue);//done TESTed
Type exp_unary(bool &lvalue);//done TESTed
Type exp_mul(bool &lvalue);//done   TESTed
Type exp_add(bool &lvalue);//done   TESTed
Type exp_ineq()bool &lvalue;//done  TESTed
Type exp_eq()bool &lvalue;//done    TESTed
Type exp_and(bool &lvalue);//done   TESTed
Type exp_or(bool &lvalue);//done    TESTED
void assignment();//done    TESTED
void stmt();//done  TESTED
void stmt_s();//done    TESTED
void declarator(int typespec);//done    TESTED
void declaration();//done
void declaration_s();//DIFFICULT DOES NOT WORK
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
       error(); //write error later
}


//HERE IS WHERE WE ADD CALL VARIABLE
void exp_t()
{
    if (la == ID){
        //match(ID);
        std::string id = id_match(ID);
        if (la == '('){
            call_func(id);
            match('(');
            if (la != ')'){ 
                exp_lst();
            }
            match(')');
            return;
        }
        call_var(id);
    } else if (la == NUM){
        match(NUM);
        //num_match(NUM);
    } else if (la == STRING){
        match(STRING);
    } else if (la == CHARACTER){
        match(CHARACTER);
    } else if (la == '('){
        match('(');
        exp_or();
        match(')');
    }
}


void exp_lst()
{
    exp_or();
    while(la == ','){
        match(',');
        exp_or();  //might change to exp_lst if tail recursive
    }
}


void exp_brack()
{
    exp_t();
    while (la == '['){  
        match('[');
        exp_or();
        match(']');
        //std::cout << "index" << std::endl;
    }
}


void exp_unary()
{
    if (la == '&') {
        match('&');
        exp_unary();
        //std::cout << "addr" << std::endl;
    } else if (la == '!') {
        match('!');
        exp_unary();
        //std::cout << "not" << std::endl;
    } else if (la == '-') {
        match('-');
        exp_unary();
        //std::cout << "neg" << std::endl;
    } else if (la == '*') {
        match('*');
        exp_unary();
        //std::cout << "deref" << std::endl;
    } else if (la == SIZEOF){
        match(SIZEOF);
        exp_unary();
        //std::cout << "sizeof" << std::endl;
    } else {
        exp_brack();
    }
}


void exp_mul()
{
    //not sure the star thing works properly
    exp_unary();
    for EVER{
        if (la == '*'){
            match('*');
            exp_unary();
            //std::cout << "mul" << std::endl;
        } else if (la == '/'){
            match('/');
            exp_unary();
            //std::cout << "div" << std::endl;
        } else if (la == '%'){
            match('%');
            exp_unary();
            //std::cout << "rem" << std::endl;
        } else {
            break;
        }
    }
}


void exp_add()
{
    exp_mul();
    for EVER{
        if (la == '+'){
            match('+');
            exp_mul();
            //std::cout << "add" << std::endl;
        } else if (la == '-'){
            match('-');
            exp_mul();
            //std::cout << "sub" << std::endl;
        } else {
            break;
        }
    }
}
        

void exp_ineq()
{
    exp_add();
    for EVER{
        if (la == '>'){
            match('>');
            exp_add();
            //std::cout << "gtn" << std::endl;
        } else if (la == '<'){
            match('<');
            exp_add();
            //std::cout << "ltn" << std::endl;
        } else if (la == LTE){
            match(LTE);
            exp_add();
            //std::cout << "leq" << std::endl;
        } else if (la == GTE){
            match(GTE);
            exp_add();
            //std::cout << "geq" << std::endl;
        } else {
            break;
        }
    }
}


Type exp_eq(bool &lvalue)
{
    Type left = exp_ineq();
    for EVER{
        if (la == IS_EQ){
            match(IS_EQ);
            Type right = exp_ineq();
            lvalue = false;
        } else if (la == NOT_EQ){
            match(NOT_EQ);
            Type right = exp_ineq();
            left = checkINEQ(left, right);
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


Type exp_or(bool &lvalue)
{
    Type left = exp_and(lvalue);

    while (la == LOR){
        match(LOR);
        Type right = exp_and(lvalue);
        left = checkLogicalOR(left,right)
        lvalue = false;
    }
    return left;    //left must be Type INT
}


void assignment()
{
    exp_or();
    if (la == '='){
        match('=');
        exp_or();
        //std::cout << "assignment" << std::endl;
    } 
}

void stmt()
{
    if (la == '{'){
        match('{');
        open_scope();   //open scope block
        declaration_s();
        stmt_s();
        match('}');
        close_scope();
        return;
    } if (la == BREAK){
        match(BREAK);
        match(';');
    } else if (la == RETURN){
        match(RETURN);
        exp_or();
        match(';');
    } else if (la == WHILE){
        match(WHILE);
        match('(');
        exp_or();
        match(')');
        stmt();
    } else if (la == FOR){
        match(FOR);
        match('(');
        assignment();
        match(';');
        exp_or();
        match(';');
        assignment();
        match(')');
        stmt();
    } else if (la == IF){
        match(IF);
        match('(');
        exp_or();
        match(')');
        stmt();
        if (la == ELSE){
            match(ELSE);
            stmt();
        }
    } else {
        assignment();
        match(';');
    }
}


void stmt_s()
{
    while (la != '}'){
        stmt();
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
            Type *t = new Type(TRIPLE_DOT,0);
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
   // std::cout << "parameterssss" << std::endl;
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
            stmt_s();
            match('}');
            close_scope();
            //std::cout << "close function scope" << std::endl;
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
