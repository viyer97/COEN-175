#include <stdlib.h>
#include <iostream>
#include <string>
#include "../includes/tokens.h"
#include "../includes/lexer.h"
#define EVER (;;)

/*TODO: 
 * declaration_s might not be correct THIS IS HARD
 * exp_brack might not have to have a while loop because we only match brackets once
 *
 * left factor assign function
 * may not need parameter_lst function
 */

//using namespace std;

int la;

unsigned num_match(int t); 
std::string id_match(int t);
/* put some other shit here for matching stuff*/ 

void error();   //done  TESTed
void match(int t);  //done  TESTed
void exp_lst();// AYBE 
void exp_t();//done MAYBE   TESTed
void exp_brack();//done TESTed
void exp_unary();//done TESTed
void exp_mul();//done   TESTed
void exp_add();//done   TESTed
void exp_ineq();//done  TESTed
void exp_eq();//done    TESTed
void exp_and();//done   TESTed
void exp_or();//done    TESTED
void assignment();//done    TESTED
void stmt();//done  TESTED
void stmt_s();//done    TESTED
void declarator(int typespec);//done    TESTED
void declaration();//done
void declaration_s();//DIFFICULT DOES NOT WORK
void param();//done
void param_lst();
void param_s_prime();
void param_s();
int specifier();
unsigned ptrs();
void global_dec();
void trans_unit();  //rule for translation unit
inline void rest();

void error()
{
   std::cout << "Error in matching" << std::endl;
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


void exp_t()
{
    if (la == ID){
        //match(ID);
        id_match(ID);
        if (la == '('){
            match('(');
            if (la != ')'){ 
                exp_lst();
            }
            match(')');
        }
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
        std::cout << "index" << std::endl;
    }
}


void exp_unary()
{
    if (la == '&') {
        match('&');
        exp_unary();
        std::cout << "addr" << std::endl;
    } else if (la == '!') {
        match('!');
        exp_unary();
        std::cout << "not" << std::endl;
    } else if (la == '-') {
        match('-');
        exp_unary();
        std::cout << "neg" << std::endl;
    } else if (la == '*') {
        match('*');
        exp_unary();
        std::cout << "deref" << std::endl;
    } else if (la == SIZEOF){
        match(SIZEOF);
        exp_unary();
        std::cout << "sizeof" << std::endl;
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
            std::cout << "mul" << std::endl;
        } else if (la == '/'){
            match('/');
            exp_unary();
            std::cout << "div" << std::endl;
        } else if (la == '%'){
            match('%');
            exp_unary();
            std::cout << "rem" << std::endl;
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
            std::cout << "add" << std::endl;
        } else if (la == '-'){
            match('-');
            exp_mul();
            std::cout << "sub" << std::endl;
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
            std::cout << "gtn" << std::endl;
        } else if (la == '<'){
            match('<');
            exp_add();
            std::cout << "ltn" << std::endl;
        } else if (la == LTE){
            match(LTE);
            exp_add();
            std::cout << "leq" << std::endl;
        } else if (la == GTE){
            match(GTE);
            exp_add();
            std::cout << "geq" << std::endl;
        } else {
            break;
        }
    }
}


void exp_eq()
{
    exp_ineq();
    for EVER{
        if (la == IS_EQ){
            match(IS_EQ);
            exp_ineq();
            std::cout << "eql" << std::endl;
        } else if (la == NOT_EQ){
            match(NOT_EQ);
            exp_ineq();
            std::cout << "neq" << std::endl;
        } else {
            break;
        }
    }
}


void exp_and()
{
    exp_eq();
    while (la == LAND){
        match(LAND);
        exp_eq();
        std::cout << "and" << std::endl;
    }
}


void exp_or(void)
{
    exp_and();
    while (la == LOR){
        match(LOR);
        exp_and();
        std::cout << "or" << std::endl;
    }
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
        std::cout << "open block" << std::endl;
        declaration_s();
        stmt_s();
        match('}');
        std::cout << "close block" << std::endl;
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
    }
    std::cout << ind << id << len << std::endl;
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


void param()
{
    int typespec = specifier();
    int ind = ptrs();
    //match(ID);
    std::string id = id_match(ID);
    std::cout << ind << id << std::endl;
   // std::cout << "parameter" << std::endl;
}


void param_lst()
{
    param();
    while (la == ','){  
        match(',');
        param();
    }
    //std::cout << "parameter list" << std::endl;
}


void param_s_prime()
{
    if (la == ','){
        match(',');
        if (la  == TRIPLE_DOT){
            match(TRIPLE_DOT);
            return;
        }
        param();
        param_s_prime();
    }
}


void param_s()
{
    unsigned ind; 
    if (la == VOID){
        match(VOID);
        if (la != ')'){
            ptrs();
            //match(ID);
            std::string id = id_match(ID);
            param_s_prime();
        } 
    } else if (la == CHAR){
        match(CHAR);
        ptrs();
        //match(ID);
        std::string id = id_match(ID);
        param_s_prime();
    } else if (la == INT){
        match(INT);
        ptrs();
        //match(ID);
        std::string id = id_match(ID);
        param_s_prime();
    }
   // std::cout << "parameterssss" << std::endl;
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


void global_dec()
{
    unsigned ind = ptrs();
    unsigned len;
    //match(ID);
    std::string id = id_match(ID);
    if (la == '('){
        match('(');
        param_s();
        match(')');
    } else if (la == '['){
        match('[');
        len = num_match(NUM);
        match(']');
    }
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
    specifier();
    ptrs();
    //match(ID);
    std::string id = id_match(ID);
    
    /*
     * first if is matching global arrays
     * second if is matching function declarations/definitions
     */
    if (la == '['){
        match('[');
        match(NUM);
        match(']');
    //    std::cout << "global array" << std::endl;
    } else if (la == '('){
        std::cout << "open function scope" << std::endl;
        match('(');
        param_s();
        match(')');

        /*
         * Case when looking at function definition
         * so we only match until end brace
         * and don't need to call rest()
         */
        if (la == '{'){
            match('{');
            declaration_s();
            stmt_s();
            match('}');
            std::cout << "close function scope" << std::endl;
            return;
        }
        std::cout << "close function scope" << std::endl;
    } 
    rest();
}


inline void rest()
{
    for EVER{
        if (la == ';'){
            match(';');
            break;
        } else if (la == ','){
            match(',');
            global_dec();
        }
    }
}


int main(int argc, char *argv[])
{
    std::cout << "open global scope" << std::endl;
    la = yylex();
    while(la != 0){
        trans_unit();
    }
    std::cout << "close global scope" << std::endl;
    return 0;
}
