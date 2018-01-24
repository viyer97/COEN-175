# include <stdlib.h>
# include <iostream>
# include "tokens.h"
# include "lexer.h"
#define EVER (;;)

/*TODO: 
 * write terminal matching thing
 * double check if assignment is correct
 *
 */

using namespace std;

int la;

void error();   //done
void match(int t);  //done
void term_id();
void exp_t();//done
void exp_brack();//done
void exp_unary();//done
void exp_mul();//done
void exp_add();//done
void exp_ineq();//done
void exp_eq();//done
void exp_and();//done
void exp_or();//done
void assignment();//done
void stmt();
void stmt_s();
void declarator();
void declarator_lst();
void declaration();
void declaration_s();
void param();
void parameter_lst();
void param_s();
void trans_unit();  //rule for translation unit

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


void term_id()
{

}


void exp_t()
{
    if (la == ID){
        match(ID);
        term_id();  //do something abut left factoring
        //put cool stuff here
    } else if (la == NUM){
        match(NUM);
    } else if (la == STRING){
        match(STRING);
    } else if (la == CHARACTER){
        match(CHARACTER);
    } else {    //I don't know if this is corrrect
        match('(');
        exp_or();
        match(')');
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
            cout << "mod" << endl;
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
        return;
    } 
}

void stmt()
{
    if (la == '{'){
        match('{');
        declaration_s();
        stmt_s();
        match('}');
        cout << "statement" << endl;
    } else if (la == BREAK){
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
    } //finish the rest of statement
}

int main(int argc, char *argv[])
{
    la = yylex();
    assignment();
    return 0;
}
