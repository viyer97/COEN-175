#include <stdlib.h>
#include <iostream>
#include "../includes/tokens.h"
#include "../includes/lexer.h"
#define EVER (;;)

/*TODO: 
 * write terminal matching thing
 * double check if assignment is correct
 * might not need prints of things like statement, declaration, etc
 * declaration_s might not be correct THIS IS HARD
 * exp_brack might not have to have a while loop because we only match brackets once
 *
 */

using namespace std;

int la;

void error();   //done
void match(int t);  //done
void exp_lst();//done MAYBE
void exp_t();//done MAYBE
void exp_brack();//done
void exp_unary();//done
void exp_mul();//done
void exp_add();//done
void exp_ineq();//done
void exp_eq();//done
void exp_and();//done
void exp_or();//done
void assignment();//done
void stmt();//done
void stmt_s();//done
void declarator();//done
void declarator_lst();//done
void declaration();//done
void declaration_s();//DIFFICULT
void param();//done
void param_lst();
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
        cout << "assignment" << endl;
    } 
}

void stmt()
{
    /*if (la == '{'){
        match('{');
        declaration_s();
        stmt_s();
        match('}');
        cout << "statement" << endl;
    }*/ if (la == BREAK){
        match(BREAK);
        match(';');
        cout << "statement" << endl;
    } else if (la == RETURN){
        match(RETURN);
        exp_or();
        match(';');
        cout << "statement" << endl;
    } else if (la == WHILE){
        match(WHILE);
        match('(');
        exp_or();
        match(')');
        stmt();
        cout << "statement" << endl;
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
        cout << "statement" << endl;
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
        cout << "statement" << endl;
    } else {
        assignment();
        match(';');
        cout << "statement" << endl;
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
        cout << "declarator" << endl;
    }
}


void declarator_lst()
{
    declarator();
    while (la == ','){  //might need to be changed to if and recur
        match(',');
        declarator();
        cout << "declarator list" << endl;
    } 
}


void declaration()
{
    specifier();
    declarator_lst();
    match(';');
    cout << "declaration" << endl;
}

void declaration_s()
{
    /* I don't know if this works*/
    
/*    declaration();
    declaration_s(); */
    while (la != ';'){
        declaration(); 
    }
}


void param()
{
    specifier();
    ptrs();
    match(ID);
}


void param_lst()
{
    param();
    while (la == ','){  //might need to use if and recur
        match(',');
        param();
    }
}


void param_s()
{
    /* FINISH THIS FUNCTIONS */
    if (la == VOID || la == INT || la == CHAR){
        match(la);
        if (la != ')'){
            ptrs();
            match(ID);

            while (la == ','){
                match(',');
                if (la == TRIPLE_DOT){
                    match(TRIPLE_DOT);
                    break;  //may need to change this loop
                }
                param();
            }
        }
    }
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


void trans_unit()
{
    specifier();
    ptrs();
    match(ID);
    cout << "global declaratoin" << endl;
    if (la == '['){
        match('[');
        match(NUM);
        match(']');
        cout << "global array" << endl;
    } else if (la == '('){
        match('(');
        param_s();
        match(')');
        if (la == '{'){
            declaration_s();
            stmt_s();
            match('}');
        }
        cout << "function ting" << endl;
    } 
    rest();
}


inline void rest()
{
    /* this could be the reason why it's broken */
    /*if (la == ';'){
        match(';');
    } else if (la == ','){
        match(',');
        global_dec();
        rest();
    }*/
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
    //    trans_unit();
        stmt();
    }
    return 0;
}
