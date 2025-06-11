#include "scanner.h"

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string inputFileName/*, SymbolTable* table*/)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/

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

    // Inicializa o mapa de palavras reservadas
    initializeReservedWords();
}

// Método para inicializar o mapa de palavras reservadas
void Scanner::initializeReservedWords()
{
    reservedWords["boolean"] = ID;
    reservedWords["class"]   = ID; 
    reservedWords["else"]    = ID; 
    reservedWords["extends"] = ID; 
    reservedWords["false"]   = ID; 
    reservedWords["if"]      = ID; 
    reservedWords["int"]     = ID; 
    reservedWords["length"]  = ID; 
    reservedWords["main"]    = ID; 
    reservedWords["new"]     = ID; 
    reservedWords["public"]  = ID; 
    reservedWords["return"]  = ID; 
    reservedWords["static"]  = ID; 
    reservedWords["String"]  = ID; 
    reservedWords["this"]    = ID; 
    reservedWords["true"]    = ID; 
    reservedWords["void"]    = ID; 
    reservedWords["while"]   = ID; 
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
    char c = nextChar();

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
            lexicalError("Unterminated block comment.");
        }
    }
    else
    {
        // Provavelmente é um operator.
        goBack();
    }
}

int Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    string lexeme = "";
    char c;

    while (true)
    {
        skipWhitespace(); // Ignora whitespace 

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
            }
            // Se não for um comentário é um operador de divisão.
            return new Token(OPERATOR, "/");
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
                                return new Token(SEPARATOR, potential_full_lexeme); // Tratando como separator 
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
            return new Token(OPERATOR, "&&");
        }
        if (c == '<')return new Token(OPERATOR, "<");
        if (c == '>')return new Token(OPERATOR, ">");
        if (c == '+')return new Token(OPERATOR, "+");
        if (c == '-') return new Token(OPERATOR, "-");
        if (c == '*') return new Token(OPERATOR, "*");
        if (c == '=')
        {
            if (peekChar() == '=')
            {
                nextChar();
                return new Token(OPERATOR, "==");
            }
            else
            {
                return new Token(OPERATOR, "=");
            }
        }
        if (c == '!')
        {
            if (peekChar() == '=')
            {
                nextChar();
                return new Token(OPERATOR, "!=");
            }
            else
            {
                return new Token(OPERATOR, "!");
            }
        }

        // Separators 
        if (c == '(') return new Token(SEPARATOR, "(");
        if (c == ')') return new Token(SEPARATOR, ")");
        if (c == '[') return new Token(SEPARATOR, "[");
        if (c == ']') return new Token(SEPARATOR, "]");
        if (c == '{') return new Token(SEPARATOR, "{");
        if (c == '}') return new Token(SEPARATOR, "}");
        if (c == ';') return new Token(SEPARATOR, ";");
        if (c == '.') return new Token(SEPARATOR, ".");
        if (c == ',') return new Token(SEPARATOR, ",");

        // Caracter inválido
        lexicalError("Símbolo inválido: '" + string(1, c) + "'");
        return nullptr;
    }
}

void Scanner::lexicalError(string msg)
{
    cout << "Line " << line << ": " << msg << endl;
    exit(EXIT_FAILURE);
}