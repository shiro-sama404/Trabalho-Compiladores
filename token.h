#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h>//Funções de caracteres
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
           if (name == 0) return "UNDEF";
           if (name == 2) return "ID"; 
           if (name == 3) return "INTEGER"; 
           if (name == 4) return "SEPARATOR";
           if (name == 5) return "END_OF_FILE";
           if (name == 6) return "KW_CLASS"; 
           if (name == 7) return "KW_PUBLIC"; 
           if (name == 8) return "KW_STATIC";
           if (name == 9) return "KW_VOID";
           if (name == 10) return "KW_MAIN";
           if (name == 11) return "KW_STRING";
           if (name == 12) return "KW_EXTENDS";
           if (name == 13) return "KW_INT";
           if (name == 14) return "KW_BOOLEAN"; 
           if (name == 15) return "KW_TRUE"; 
           if (name == 16) return "KW_FALSE";
           if (name == 17) return "KW_THIS";
           if (name == 18) return "KW_NEW"; 
           if (name == 19) return "KW_RETURN"; 
           if (name == 20) return "KW_IF";
           if (name == 21) return "KW_ELSE";
           if (name == 22) return "KW_WHILE"; 
           if (name == 23) return "KW_LENGTH"; 
           if (name == 24) return "KW_SYSTEM_OUT_PRINTLN";
           if (name == 25) return "&&";
           if (name == 26) return "<"; 
           if (name == 27) return ">"; 
           if (name == 28) return "==";
           if (name == 29) return "!=";
           if (name == 30) return "+"; 
           if (name == 31) return "-"; 
           if (name == 32) return "*";
           if (name == 33) return "/";
           if (name == 34) return "="; 
           if (name == 35) return "!"; 
           if (name == 36) return "(";
           if (name == 37) return ")";
           if (name == 38) return "["; 
           if (name == 39) return "]"; 
           if (name == 40) return "{";
           if (name == 41) return "}";
           if (name == 42) return ";";
           if (name == 43) return ".";
           if (name == 44) return ",";
           return "";
        }
};

#endif