#ifndef TOKEN_H
#define TOKEN_H

enum tokens {
   AUTO=256,
   BREAK,
   CASE,
   CHAR,
   CONST,   //260
   CONTINUE,
   DEFAULT,
   DO,
   DOUBLE,
   ELSE,
   ENUM,
   EXTERN,
   FLOAT,
   FOR,
   GOTO,    //270
   IF,
   INT,
   LONG,
   REGISTER,
   RETURN,  //275
   SHORT,
   SIGNED,
   SIZEOF,
   STATIC,
   STRUCT,  //280
   SWITCH,
   TYPEDEF,
   UNION,
   UNSIGNED,
   VOID,
   VOLATILE,
   WHILE,
   NUM,
   STRING,
   CHARACTER,   //290
   ID,
   LOR,
   LAND,
   IS_EQ,
   NOT_EQ,
   LTE,
   GTE,
   INC,
   DEC,
   DEREF_ACC,   //300
   TRIPLE_DOT   //301
};

#endif
