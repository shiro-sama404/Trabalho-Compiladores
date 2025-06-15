#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>
#include <unordered_map>

using namespace std;

enum Names 
{
    UNDEF,
    ID,
    INTEGER,
    OPERATOR, 
    SEPARATOR,
    END_OF_FILE,
    // Keywords
    KW_CLASS,
    KW_PUBLIC,
    KW_STATIC,
    KW_VOID,
    KW_MAIN,
    KW_STRING,
    KW_EXTENDS,
    KW_INT,
    KW_BOOLEAN,
    KW_TRUE,
    KW_FALSE,
    KW_THIS,
    KW_NEW,
    KW_RETURN,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_LENGTH,
    KW_SYSTEM_OUT_PRINTLN,
    // Operators
    OP_AND,     // &&
    OP_LT,      // <
    OP_GT,      // >
    OP_EQ,      // ==
    OP_NEQ,     // !=
    OP_PLUS,    // +
    OP_MINUS,   // -
    OP_MULT,    // *
    OP_DIV,     // /
    OP_ASSIGN,  // =
    OP_NOT,     // !
    // Separators
    SEP_LPAREN, // (
    SEP_RPAREN, // )
    SEP_LBRACKET, // [
    SEP_RBRACKET, // ]
    SEP_LBRACE, // {
    SEP_RBRACE, // }
    SEP_SEMICOLON, // ;
    SEP_DOT,    // .
    SEP_COMMA   // ,
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};

#endif