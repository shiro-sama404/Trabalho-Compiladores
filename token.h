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
    END_OF_FILE
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