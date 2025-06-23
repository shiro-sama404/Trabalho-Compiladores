#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string inputFileName; // Armazena o nome do arquivo de entrada
        string input;         // Armazena o texto de entrada
        int currentPos;              // Posição atual
        int currentLine;
        unordered_map<string, int> reservedWords;

        // Métodos auxiliares
        void initializeReservedWords(); // Inicializa o mapa de palavras reservadas

        bool isEOF(int);              // Verifica se chegou ao final do arquivo
        bool isLetter(char);          // Verifica se é uma letra
        bool isDigit(char);           // Verifica se é um dígito

        char nextChar(int*);          // Retorna o próximo caractere e avança a posição
        char peekChar(int);           // Retorna o próximo caractere sem avançar a posição
        void goBack(int*);            // Volta uma posição
        
        void skipWhitespace(int*, int*); // Ignora espaços em branco
        void skipComment(int*, int*);    // Ignora comentários de linha e de bloco

        Token* getToken(int*, int*); // Retorna o token lido a partir de uma posição
        
    public:
        // Construtor
        Scanner(string);

        // Getters
        int getPos();
        int getLine();
        string getFilename();
    
        Token* nextToken();          // Retorna o próximo token da entrada
        Token* peekNextToken(int); // Retorna o n-ésimo próximo token da entrada

        // Método para manipular erros
        void error(string);
};

#endif