#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h> //Funções de caracteres
#include <string>
#include <unordered_map>

using namespace std;

enum Names 
{
    UNDEF,       // 0
    ID,          // 1
    INTEGER,     // 2
    OPERATOR,    // 3
    SEPARATOR,   // 4
    END_OF_FILE, // 5
    // Keywords
    KW_CLASS,              // 6
    KW_PUBLIC,             // 7
    KW_STATIC,             // 8
    KW_VOID,               // 9
    KW_MAIN,               // 10
    KW_STRING,             // 11
    KW_EXTENDS,            // 12
    KW_INT,                // 13
    KW_BOOLEAN,            // 14
    KW_TRUE,               // 15
    KW_FALSE,              // 16
    KW_THIS,               // 17
    KW_NEW,                // 18
    KW_RETURN,             // 19
    KW_IF,                 // 20
    KW_ELSE,               // 21
    KW_WHILE,              // 22
    KW_LENGTH,             // 23
    KW_SYSTEM_OUT_PRINTLN, // 24
    // Operators
    OP_AND,     // 25 &&
    OP_LT,      // 26 <
    OP_GT,      // 27 >
    OP_EQ,      // 28 ==
    OP_NEQ,     // 29 !=
    OP_PLUS,    // 30 +
    OP_MINUS,   // 31 -
    OP_MULT,    // 32 *
    OP_DIV,     // 33 /
    OP_ASSIGN,  // 34 =
    OP_NOT,     // 35!
    // Separators
    SEP_LPAREN,    // 36 (
    SEP_RPAREN,    // 37 )
    SEP_LBRACKET,  // 38 [
    SEP_RBRACKET,  // 39 ]
    SEP_LBRACE,    // 40 {
    SEP_RBRACE,    // 41 }
    SEP_SEMICOLON, // 42 ;
    SEP_DOT,       // 43 .
    SEP_COMMA      // 44 ,
};

static const string lexemeList[] = {
    "UNDEF",
    "ID",
    "INTEGER",
    "OPERATOR",
    "SEPARATOR",
    "END_OF_FILE",
    "KW_CLASS",
    "KW_PUBLIC",
    "KW_STATIC",
    "KW_VOID",
    "KW_MAIN",
    "KW_STRING",
    "KW_EXTENDS",
    "KW_INT",
    "KW_BOOLEAN",
    "KW_TRUE",
    "KW_FALSE",
    "KW_THIS",
    "KW_NEW",
    "KW_RETURN",
    "KW_IF",
    "KW_ELSE",
    "KW_WHILE",
    "KW_LENGTH",
    "KW_SYSTEM_OUT_PRINTLN",
    "OP_AND",
    "OP_LT",
    "OP_GT",
    "OP_EQ",
    "OP_NEQ",
    "OP_PLUS",
    "OP_MINUS",
    "OP_MULT",
    "OP_DIV",
    "OP_ASSIGN",
    "OP_NOT",
    "SEP_LPAREN",
    "SEP_RPAREN",
    "SEP_LBRACKET",
    "SEP_RBRACKET",
    "SEP_LBRACE",
    "SEP_RBRACE",
    "SEP_SEMICOLON",
    "SEP_DOT",
    "SEP_COMMA"
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

        static string toLexeme(int name){
           return (name >= 0 && name <= 44) ? lexemeList[name] : lexemeList[0];
        }

        int getName(){
            return this->name;
        }

        int getAttribute(){
            return this->attribute;
        }

        string getLexeme(){
            return this->lexeme;
        }
};

#endif