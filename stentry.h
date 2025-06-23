#ifndef STENTRY_H
#define STENTRY_H

#include "token.h"

// Enum para a categoria/natureza do símbolo
enum SymbolCategory {
    CAT_RESERVED,
    CAT_UNKNOWN,
    CAT_CLASS,
    CAT_METHOD,
    CAT_VARIABLE,
    CAT_PARAMETER
};

// Enum para os tipos semânticos que um símbolo pode ter
enum SymbolType {
    SYM_RESERVED,    // Palavras reservadas
    SYM_UNKNOWN,     // Tipo desconhecido ou erro
    SYM_VOID,        // Para métodos main ou métodos sem retorno explícito
    SYM_INT,         // int
    SYM_BOOLEAN,     // boolean
    SYM_INT_ARRAY,   // int[]
    SYM_ID,          // Palavras reservadas
    SYM_CLASS,       // Usado para o próprio nome da classe
    SYM_STRING_ARRAY // Para String[] no main
};

class STEntry
{
public:
    Token* token;            // O token original associado ao símbolo (contém lexeme e name)
    SymbolCategory category; // Categoria do símbolo (Classe, Método, Variável, Parâmetro)
    SymbolType type;         // Tipo semântico do símbolo (int, boolean, int[], etc.)
    bool reserved;

    // Construtores
    STEntry();
    STEntry(Token*);
    STEntry(Token*, SymbolCategory, SymbolType, bool);
};

#endif