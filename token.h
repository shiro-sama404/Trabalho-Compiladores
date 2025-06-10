#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    //Continuar
    //Nomes e atributos dos tokens da linguagem
    ID, INT_LIT, OP_AND, OP_OR, OP_L_ARROW, OP_R_ARROW,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_ATR, OP_EQUAL,
    OP_N_EQUAL, OP_NOT, SEP_L_PAR, SEP_R_PAR, SEP_L_BRA,
    SEP_R_BRA, SEP_L_CUR_BRA, SEP_R_CUR_BRA, W_BOO,
    W_CLA, W_ELS, W_EXT, W_FAL, W_IF, W_INT, W_LEN,
    W_MAI, W_NEW, W_PUB, W_RET, W_STA, W_STR, W_SYS, W_THI,
    W_TRU, W_VOI, W_WHI, UNDEFINED, SEMICOLON

    ,END_OF_FILE
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
