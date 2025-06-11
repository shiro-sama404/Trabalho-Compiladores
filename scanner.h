#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string input; //Armazena o texto de entrada
        int pos; //Posição atual
        int line;
        std::unordered_map<string, int> reservedWords;

        // Métodos auxiliares
        void initializeReservedWords(); // Inicializa o mapa de palavras reservadas
        bool isEOF();                   // Verifica se chegou ao final do arquivo
        bool isLetter(char c);          // Verifica se é uma letra
        bool isDigit(char c);           // Verifica se é um dígito
        char nextChar();                // Retorna o próximo caractere e avança a posição
        char peekChar();                // Retorna o próximo caractere sem avançar a posição
        void goBack();                  // Volta uma posição
        
        void skipWhitespace();          // Ignora espaços em branco
        void skipComment();             // Ignora comentários de linha e de bloco
        
    
    public:
    //Construtor
        Scanner(string);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
    
        //Método para manipular erros
        void lexicalError(string);
};

#endif