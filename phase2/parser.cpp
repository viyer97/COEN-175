#include <stdlib.h>
#include <iostream>
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

using namespace std;

int la;

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
void declarator();//done    TESTED
void declarator_lst();//done    TESTED
void declaration();//done
void declaration_s();//DIFFICULT DOES NOT WORK
void param();//done
void param_lst();
void param_s_prime();
void param_s();

void specifier();
void ptrs();
void global_dec();
void trans_unit();  //rule for translation unit
inline void rest();

void error()
{
    cout << "Error in matching" << endl;
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
        match(ID);
        if (la == '('){
            match('(');
            if (la != ')'){ //NOT SURE IF THIS WORKS
                exp_lst();
            }
            match(')');
        }
    } else if (la == NUM){
        match(NUM);
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
/*  THIS MAY NOT BE CORRECT */
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
        cout << "index" << endl;
    }
}


void exp_unary()
{
    if (la == '&') {
        match('&');
        exp_unary();
        cout << "addr" << endl;
    } else if (la == '!') {
        match('!');
        exp_unary();
        cout << "not" << endl;
    } else if (la == '-') {
        match('-');
        exp_unary();
        cout << "neg" << endl;
    } else if (la == '*') {
        match('*');
        exp_unary();
        cout << "deref" << endl;
    } else if (la == SIZEOF){
        match(SIZEOF);
        exp_unary();
        cout << "sizeof" << endl;
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
            cout << "mul" << endl;
        } else if (la == '/'){
            match('/');
            exp_unary();
            cout << "div" << endl;
        } else if (la == '%'){
            match('%');
            exp_unary();
            cout << "rem" << endl;
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
            cout << "add" << endl;
        } else if (la == '-'){
            match('-');
            exp_mul();
            cout << "sub" << endl;
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
            cout << "gtn" << endl;
        } else if (la == '<'){
            match('<');
            exp_add();
            cout << "ltn" << endl;
        } else if (la == LTE){
            match(LTE);
            exp_add();
            cout << "leq" << endl;
        } else if (la == GTE){
            match(GTE);
            exp_add();
            cout << "geq" << endl;
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
            cout << "eql" << endl;
        } else if (la == NOT_EQ){
            match(NOT_EQ);
            exp_ineq();
            cout << "neq" << endl;
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
        cout << "and" << endl;
    }
}


void exp_or(void)
{
    exp_and();
    while (la == LOR){
        match(LOR);
        exp_and();
        cout << "or" << endl;
    }
}


void assignment()
{
    exp_or();
    if (la == '='){
        match('=');
        exp_or();
        //cout << "assignment" << endl;
    } 
}

void stmt()
{
    if (la == '{'){
        match('{');
        declaration_s();
        stmt_s();
        match('}');
        //cout << "statement" << endl;
    } if (la == BREAK){
        match(BREAK);
        match(';');
        //cout << "statement" << endl;
    } else if (la == RETURN){
        match(RETURN);
        exp_or();
        match(';');
        //cout << "statement" << endl;
    } else if (la == WHILE){
        match(WHILE);
        match('(');
        exp_or();
        match(')');
        stmt();
        //cout << "statement" << endl;
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
        //cout << "statement" << endl;
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
        //cout << "statement" << endl;
    } else {
        assignment();
        match(';');
        //cout << "statement" << endl;
    }
}


void stmt_s()
{
    while (la != '}'){
        stmt();
    }
}


void declarator()
{
    ptrs();
    match(ID);
    if (la == '['){
        match('[');
        match(NUM);
        match(']');
    }
//    cout << "declarator" << endl;
}


void declarator_lst()
{
    declarator();
    while (la == ','){  //might need to be changed to if and recur
        match(',');
        declarator();
//        cout << "declarator list" << endl;
    } 
}


void declaration()
{
    specifier();
    declarator_lst();
    match(';');
 //   cout << "declaration" << endl;
}

void declaration_s()
{
    /* I don't know if this works*/
    
/*    declaration();
    declaration_s(); */
    while (la == INT || la == CHAR || la == VOID){
        declaration();
    }
  //  cout << "declarationSSSS" << endl;
}


void param()
{
    specifier();
    ptrs();
    match(ID);
   // cout << "parameter" << endl;
}


void param_lst()
{
    param();
    while (la == ','){  //might need to use if and recur
        match(',');
        param();
    }
    //cout << "parameter list" << endl;
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
    /* FINISH THIS FUNCTIONS */
    if (la == VOID){
        match(VOID);
        if (la != ')'){
            ptrs();
            match(ID);
            param_s_prime();
        } /*else {
            match(')');
        }*/
    } else if (la == CHAR){
        match(CHAR);
        ptrs();
        match(ID);
        param_s_prime();
    } else if (la == INT){
        match(INT);
        ptrs();
        match(ID);
        param_s_prime();
    }
   // cout << "parameterssss" << endl;
}
       

void specifier()
{
    if (la == INT){
        match(INT);
    } else if (la == CHAR){
        match(CHAR);
    } else {
        match(VOID);
    }
}


void ptrs()
{
    while (la == '*'){
        match('*');
    }
}


void global_dec()
{
    ptrs();
    match(ID);
    if (la == '('){
        match('(');
        param_s();
        match(')');
    } else if (la == '['){
        match('[');
        match(NUM);
        match(']');
    }
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
    match(ID);
    /*
     * first if is matching global arrays
     * second if is matching function declarations/definitions
     */
    if (la == '['){
        match('[');
        match(NUM);
        match(']');
    //    cout << "global array" << endl;
    } else if (la == '('){
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
            return;
        }
    //    cout << "function ting" << endl;
    } 
    rest();
    //cout << "global declaratoin" << endl;
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
    la = yylex();
    while(la != 0){
        trans_unit();
    }
    return 0;
}
