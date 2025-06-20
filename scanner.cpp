#include "scanner.h"

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string inputFileName/*, SymbolTable* table*/)
{
    this->inputFileName = inputFileName;
    this->input = "";
    this->pos = 0;

    ifstream inputFile(inputFileName, ios::in);
    string currentLine;

    if (inputFile.is_open())
    {
        while (getline(inputFile, currentLine))
        {
            this->input.append(currentLine + '\n');
        }
        inputFile.close();
    }
    else
    {
        cout << "Unable to open file: " << inputFileName << endl;
        exit(EXIT_FAILURE);
    }

    //cout << "input: " << input << endl << "size: " << input.length() << endl;

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
bool Scanner::isEOF()
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

char Scanner::nextChar()
{
    return isEOF() ? EOF : input[pos++];
}

char Scanner::peekChar()
{
    return isEOF() ? EOF : input[pos];
}

void Scanner::goBack()
{
    if (pos > 0)  pos--;
}

void Scanner::skipWhitespace()
{
    char c = peekChar();
    while (!isEOF() && (c == ' ' || c == '\t' || c == '\n' || c == '\r'))
    {
        if (peekChar() == '\n')
        {
            line++;
        }
        nextChar();
        c = peekChar();
    }
}

void Scanner::skipComment()
{
    if (peekChar() == '/')
    { // Comentário de linha // ...
        nextChar(); // Consome o outro '/'
        while (!isEOF() && peekChar() != '\n')
        {
            nextChar();
        }
    }
    else if (peekChar() == '*')
    { // comentário de bloco /* ... */ 
        nextChar(); // Consome '*'
        bool foundEnd = false;
        while (!isEOF())
        {
            if (peekChar() == '*')
            {
                nextChar();
                if (peekChar() == '/')
                {
                    nextChar();
                    foundEnd = true;
                    break;
                }
            }
            else
            {
                if (peekChar() == '\n')
                {
                    line++;
                }
                nextChar();
            }
        }
        if (!foundEnd)
        {
            error("Unterminated block comment.");
        }
        cout << "Saiu /" << endl;
    }
    else
    {
        // Provavelmente é um operator.
        goBack();
    }
}

int Scanner::getPos()
{
    return pos;
}

int Scanner::getLine()
{
    return line;
}

string Scanner::getFilename()
{
    return inputFileName;
}

// Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    string lexeme = "";
    char c;

    while (true)
    {
        skipWhitespace(); // Ignora espaços 

        if (isEOF())
        {
            return new Token(END_OF_FILE, "EOF");
        }

        c = nextChar();

        // Verifica comentários 
        if (c == '/')
        {
            if (peekChar() == '/' || peekChar() == '*')
            {
                skipComment();
                continue; // Itera novamente após ignorar o comentário
            }else{
                // Se não for um comentário é um operador de divisão.
                cout << "Entrou no /" << endl;
                return new Token(OP_DIV, "/");
            }
        }

        // Identifiers e Reserved Words 
        if (isLetter(c))
        {
            lexeme += c;
            while (isLetter(peekChar()) || isDigit(peekChar()) || peekChar() == '_')
            {
                lexeme += nextChar();
            }

            // Tratamento do System.out.println 
            if (lexeme == "System")
            {
                int current_pos = pos;
                string potential_full_lexeme = lexeme;

                if (peekChar() == '.')
                {
                    potential_full_lexeme += nextChar(); // Consome '.'
                    string out_part = "";
                    // Consome "out"
                    for (int i = 0; i < 3 && !isEOF(); ++i)
                    {
                        out_part += peekChar();
                        nextChar();
                    }
                    if (out_part == "out")
                    {
                        potential_full_lexeme += out_part;
                        // Consome '.'
                        if (peekChar() == '.')
                        {
                            potential_full_lexeme += nextChar(); // Consome '.'
                            string println_part = "";
                            // Consome "println"
                            for (int i = 0; i < 7 && !isEOF(); ++i)
                            {
                                println_part += peekChar();
                                nextChar();
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
                pos = current_pos;
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
            while (isDigit(peekChar()))
            {
                lexeme += nextChar();
            }
            return new Token(INTEGER, lexeme);
        }

        // Operators 
        if (c == '&' && peekChar() == '&'){
            nextChar();
            return new Token(OP_AND, "&&");
        }
        if (c == '<')return new Token(OP_LT, "<");
        if (c == '>')return new Token(OP_GT, ">");
        if (c == '+')return new Token(OP_PLUS, "+");
        if (c == '-') return new Token(OP_MINUS, "-");
        if (c == '*') return new Token(OP_MULT, "*");
        if (c == '=')
        {
            if (peekChar() == '=')
            {
                nextChar();
                return new Token(OP_EQ, "==");
            }
            else
            {
                return new Token(OP_ASSIGN, "=");
            }
        }
        if (c == '!')
        {
            if (peekChar() == '=')
            {
                nextChar();
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

        // Caracter inválido
        error("Lexical error: '" + string(1, c) + "'");
        return nullptr;
    }
}

void Scanner::error(string msg)
{
    cout << inputFileName + ":" << line << " lexical error: " << msg << endl;
    exit(EXIT_FAILURE);
}