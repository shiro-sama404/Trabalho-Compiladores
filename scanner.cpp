#include "scanner.h"    
#include <unordered_map>

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.

unordered_map<string, Names> keywords = {
    {"&&",OP_AND},
    {"||",OP_OR},
    {"<",OP_L_ARROW},
    {">",OP_R_ARROW},
    {"+",OP_ADD},
    {"-",OP_SUB},
    {"*",OP_MUL},
    {"/",OP_DIV},
    {"=",OP_ATR},
    {"==",OP_EQUAL},
    {"!=",OP_N_EQUAL},
    {"!",OP_NOT},
    {"(",SEP_L_PAR},
    {")",SEP_R_PAR},
    {"[",SEP_L_BRA},
    {"]",SEP_R_BRA},
    {"{",SEP_L_CUR_BRA},
    {"}",SEP_R_CUR_BRA},
    {";",SEMICOLON},
    {"boolean",W_BOO},
    {"class",W_CLA},
    {"else",W_ELS},
    {"extends",W_EXT},
    {"false",W_FAL},
    {"if",W_IF},
    {"int",W_INT},
    {"length",W_LEN},
    {"main",W_MAI},
    {"new",W_NEW},
    {"public",W_PUB},
    {"return",W_RET},
    {"static",W_STA},
    {"String",W_STR},
    {"System.out.println",W_SYS},
    {"this",W_THI},
    {"true",W_TRU},
    {"void",W_VOI},
    {"while",W_WHI},
    {"UNDEFINED",UNDEFINED}
};

Scanner::Scanner(string input/*, SymbolTable* table*/)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    //st = table;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.

    //cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok = new Token(UNDEFINED);
    string lexeme;
    char c;

//Ignora comentarios
while (pos < input.length()) {
    c = input[pos];

    if (c == ' ' || c == '\t' || c == '\r') {
        pos++;
    } else if (c == '\n') {
        line++;
        pos++;
    } else if (c == '/' && input[pos + 1] == '/') {
        while (pos < input.length() && input[pos] != '\n') pos++;
    } else if (c == '/' && input[pos + 1] == '*') {
        pos += 2;
        bool fechado = false;

        while (pos < input.length()) {
            if (input[pos] == '\n') line++;

            if (input[pos] == '*' && input[pos + 1] == '/') {
                pos += 2;
                fechado = true;
                break;
            }
            pos++;
        }

        if (!fechado) {
            lexicalError("Comentário de bloco não fechado");
        }

        // continua ignorando mais espaços/comentários
    } else {
        break;  // achou caractere útil
    }
}

// Verifica o fim do arquivo
    if (pos >= input.length()) {
        tok->name = END_OF_FILE;
        tok->attribute = 0;
        return tok;
    }

c = input[pos];

// Identifica as palavras chave e IDs
    if (isalpha(c)) {
        while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_')) {
            lexeme += input[pos++];
        }

        if (keywords.find(lexeme) != keywords.end()) {
            tok->name = keywords[lexeme]; // Palavra-chave
            cout << "CHAVE" << endl;
        } else {
            tok->name = ID; // Identificador
            cout << "ID" << endl;
        }
        tok->lexeme = lexeme;
        return tok;
    }
    
// Numeros inteiros
    if (isdigit(c)) {
        string number;
        while (isdigit(input[pos])) {
            number += input[pos++];
        }
        int number_INT = stoi(number);
        tok->name = INT_LIT;
        tok->lexeme = number;
        tok->attribute = number_INT;
        cout << "digit" << endl;
        return tok;
    }

// Caracteres especiais
    switch (c) {
        case '(': tok->name = SEP_L_PAR; pos++; break;
        case ')': tok->name = SEP_R_PAR; pos++; break;
        case '{': tok->name = SEP_R_CUR_BRA; pos++; break;
        case '}': tok->name = SEP_L_CUR_BRA; pos++; break;
        case '[': tok->name = SEP_L_BRA; pos++; break;
        case ']': tok->name = SEP_R_BRA; pos++; break;
        case ';': tok->name = SEMICOLON; pos++; break;
        case '=':
            if (input[pos + 1] == '=') {
                tok->name = OP_ATR; pos += 2;
            } else {
                tok->name = OP_EQUAL; pos++;
            }
            break;
        case '!':
            if (input[pos + 1] == '=') {
                tok->name = OP_N_EQUAL; pos += 2;
            } else {
                tok->name = OP_NOT; pos++;
            }
            break;
        case '&':
            if(input[pos + 1] == '&')
                tok->name = OP_AND; cout << "&&" << endl; pos += 2;
        case '|':
            if(input[pos + 1] == '|')
                tok->name = OP_OR; pos += 2;
        case '<': tok->name = OP_L_ARROW; pos++; break;
        case '>': tok->name = OP_R_ARROW; pos++; break;
        case '+': tok->name = OP_ADD; pos++; break;
        case '-': tok->name = OP_SUB; pos++; break;
        case '*': tok->name = OP_MUL; pos++; break;
        case '/': tok->name = OP_DIV; pos++; break;
                
        default:
            lexicalError("Símbolo inválido: " + string(1, c));
    }
    return tok;
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
