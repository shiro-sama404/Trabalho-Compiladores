#include "scanner.h"

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string inputFileName/*, SymbolTable* table*/)
{
    this->inputFileName = inputFileName;
    this->input = "";
    this->currentPos = 0;
    this->currentLine = 1;

    ifstream inputFile(inputFileName, ios::in);
    string currentFileLine;

    if (inputFile.is_open())
    {
        while (getline(inputFile, currentFileLine))
        {
            this->input.append(currentFileLine + '\n');
        }
        inputFile.close();
    }
    else
    {
        cout << "Unable to open file: " << inputFileName << endl;
        exit(EXIT_FAILURE);
    }

    // Inicializa o mapa de palavras reservadas
    initializeReservedWords();
}

// Método para inicializar o mapa de palavras reservadas
void Scanner::initializeReservedWords()
{
    reservedWords["boolean"] = KW_BOOLEAN;
    reservedWords["class"]   = KW_CLASS;  
    reservedWords["else"]    = KW_ELSE;   
    reservedWords["extends"] = KW_EXTENDS;
    reservedWords["false"]   = KW_FALSE;  
    reservedWords["if"]      = KW_IF;     
    reservedWords["int"]     = KW_INT;    
    reservedWords["length"]  = KW_LENGTH; 
    reservedWords["main"]    = KW_MAIN;   
    reservedWords["new"]     = KW_NEW;    
    reservedWords["public"]  = KW_PUBLIC; 
    reservedWords["return"]  = KW_RETURN; 
    reservedWords["static"]  = KW_STATIC; 
    reservedWords["String"]  = KW_STRING; 
    reservedWords["this"]    = KW_THIS;   
    reservedWords["true"]    = KW_TRUE;
    reservedWords["void"]    = KW_VOID;
    reservedWords["while"]   = KW_WHILE;
}

// Métodos auxiliares
bool Scanner::isEOF(int pos)
{
    return pos >= input.length();
}

bool Scanner::isLetter(char c)
{
    return isalpha(c);
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

char Scanner::nextChar(int* pos)
{
    return isEOF(*pos) ? EOF : input[(*pos)++];
}

char Scanner::peekChar(int pos)
{
    return isEOF(pos) ? EOF : input[pos];
}

void Scanner::goBack(int* pos)
{
    if (*pos > 0)  (*pos)--;
}

void Scanner::skipWhitespace(int *pos, int* line)
{
    char c = peekChar(*pos);
    while (!isEOF(*pos) && (c == ' ' || c == '\t' || c == '\n' || c == '\r'))
    {
        if (peekChar(*pos) == '\n')
        {
            (*line)++;
        }
        nextChar(pos);
        c = peekChar(*pos);
    }
}

void Scanner::skipComment(int *pos, int* line)
{
    if (peekChar(*pos) == '/')
    { // Comentário de linha // ...
        nextChar(pos); // Consome o outro '/'
        while (!isEOF(*pos) && peekChar(*pos) != '\n')
        {
            nextChar(pos);
        }
    }
    else if (peekChar(*pos) == '*')
    { // comentário de bloco /* ... */ 
        nextChar(pos); // Consome '*'
        bool foundEnd = false;
        while (!isEOF(*pos))
        {
            if (peekChar(*pos) == '*')
            {
                nextChar(pos);
                if (peekChar(*pos) == '/')
                {
                    nextChar(pos);
                    foundEnd = true;
                    break;
                }
            }
            else
            {
                if (peekChar(*pos) == '\n')
                {
                    (*line)++;
                }
                nextChar(pos);
            }
        }
        if (!foundEnd)
        {
            error("Unterminated block comment.");
        }
    }
    else
    {
        // Provavelmente é um operator.
        goBack(pos);
    }
}

int Scanner::getPos()
{
    return currentPos;
}

int Scanner::getLine()
{
    return currentLine;
}

string Scanner::getFilename()
{
    return inputFileName;
}

Token* Scanner::getToken(int* pos, int* line){
    string lexeme = "";
    char c;

    while (true)
    {
        skipWhitespace(pos, line); // Ignora espaços 

        if (isEOF(*pos))
        {
            return new Token(END_OF_FILE, "EOF");
        }

        c = nextChar(pos);

        // Verifica comentários 
        if (c == '/')
        {
            if (peekChar(*pos) == '/' || peekChar(*pos) == '*')
            {
                skipComment(pos, line);
                continue; // Itera novamente após ignorar o comentário
            }else{
                // Se não for um comentário é um operador de divisão.
                return new Token(OP_DIV, "/");
            }
        }

        // Identifiers e Reserved Words 
        if (isLetter(c))
        {
            lexeme += c;
            while (isLetter(peekChar(*pos)) || isDigit(peekChar(*pos)) || peekChar(*pos) == '_')
            {
                lexeme += nextChar(pos);
            }

            // Tratamento do System.out.println 
            if (lexeme == "System")
            {
                int current_pos = *pos;
                string potential_full_lexeme = lexeme;

                if (peekChar(*pos) == '.')
                {
                    potential_full_lexeme += nextChar(pos); // Consome '.'
                    string out_part = "";
                    // Consome "out"
                    for (int i = 0; i < 3 && !isEOF(*pos); ++i)
                    {
                        out_part += peekChar(*pos);
                        nextChar(pos);
                    }
                    if (out_part == "out")
                    {
                        potential_full_lexeme += out_part;
                        // Consome '.'
                        if (peekChar(*pos) == '.')
                        {
                            potential_full_lexeme += nextChar(pos); // Consome '.'
                            string println_part = "";
                            // Consome "println"
                            for (int i = 0; i < 7 && !isEOF(*pos); ++i)
                            {
                                println_part += peekChar(*pos);
                                nextChar(pos);
                            }
                            if (println_part == "println")
                            {
                                // Casa com "System.out.println" 
                                return new Token(KW_SYSTEM_OUT_PRINTLN, potential_full_lexeme + println_part);
                            }
                        }
                    }
                }
                // Se não for "System.out.println", reverte a posição e trata como ID
                *pos = current_pos;
            }

            // Verifica se o lexema é uma palavra reservada
            auto it = reservedWords.find(lexeme);
            if (reservedWords.find(lexeme) != reservedWords.end())
            {
                // É palavra reservada
                return new Token(it->second, lexeme);
            }
            else
            {
                // Não é palavra reservada, trata como ID
                return new Token(ID, lexeme);
            }
        }

        // Integers
        if (isDigit(c))
        {
            lexeme += c;
            while (isDigit(peekChar(*pos)))
            {
                lexeme += nextChar(pos);
            }
            return new Token(INTEGER, lexeme);
        }

        // Operators 
        if (c == '&' && peekChar(*pos) == '&'){
            nextChar(pos);
            return new Token(OP_AND, "&&");
        }
        if (c == '<')return new Token(OP_LT, "<");
        if (c == '>')return new Token(OP_GT, ">");
        if (c == '+')return new Token(OP_PLUS, "+");
        if (c == '-') return new Token(OP_MINUS, "-");
        if (c == '*') return new Token(OP_MULT, "*");
        if (c == '=')
        {
            if (peekChar(*pos) == '=')
            {
                nextChar(pos);
                return new Token(OP_EQ, "==");
            }
            else
            {
                return new Token(OP_ASSIGN, "=");
            }
        }
        if (c == '!')
        {
            if (peekChar(*pos) == '=')
            {
                nextChar(pos);
                return new Token(OP_NEQ, "!=");
            }
            else
            {
                return new Token(OP_NOT, "!");
            }
        }

        // Separators
        if (c == '(') return new Token(SEP_LPAREN, "(");   
        if (c == ')') return new Token(SEP_RPAREN, ")");   
        if (c == '[') return new Token(SEP_LBRACKET, "["); 
        if (c == ']') return new Token(SEP_RBRACKET, "]"); 
        if (c == '{') return new Token(SEP_LBRACE, "{");   
        if (c == '}') return new Token(SEP_RBRACE, "}");   
        if (c == ';') return new Token(SEP_SEMICOLON, ";");
        if (c == '.') return new Token(SEP_DOT, ".");      
        if (c == ',') return new Token(SEP_COMMA, ",");

        return new Token(UNDEF, string(1, c));
    }
}

// Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    Token* token = getToken(&currentPos, &currentLine);

    if(token->getName() == UNDEF){
        // Token inválido
        error("Invalid token: '" + token->getLexeme() + "'");
    }
    
    return token;
}

// Método que retorna o n-ésimo próximo token da entrada
Token* Scanner::peekNextToken(int n)
{
    if (n < 1){ n = 1; }

    int pos = currentPos;
    int line = currentLine;

    Token* token;

    for(int i = 0; i < n; i++){
        token = getToken(&pos, &line);
    }
    
    return token;
}

void Scanner::error(string msg)
{
    cout << inputFileName + ":" << currentLine << " lexical error: " << msg << endl;
    
    Token* token;

    while (!isEOF(currentPos)){
        token = getToken(&currentPos, &currentLine);
        
        if(token->getName() == 0){
            // Token inválido
            cout << inputFileName + ":" << currentLine << " lexical error: " << "Invalid token: '" + token->getLexeme() + "'" << endl;
        }
    }

    exit(EXIT_FAILURE);
}