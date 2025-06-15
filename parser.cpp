#include "parser.h"
#include <string>
using std::string;

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t)
		advance();
	else
		error("Sintax error: Expected " + std::to_string(t) + ". Found " + std::to_string(lToken->name) + " (" + lToken->lexeme + ")");
}

void
Parser::run()
{
	advance();	

	program();
	
	if (lToken->name == END_OF_FILE) {
        cout << "Compilation ended successfully!\n";
    } else {
        error("Unexpected content found after end of the program.");
    }
}

// Program -> MainClass (ClassDeclaration)* EOF
void
Parser::program()
{
    mainClass();
	while (lToken->name == KW_CLASS) {
        classDeclaration();
    }
    match(END_OF_FILE);
}

// MainClass -> class ID { public static void main (String[] ID){ Statement }}
void
Parser::mainClass()
{
    match(KW_CLASS);
    match(ID);
    match(SEP_LBRACE);
    match(KW_PUBLIC);
    match(KW_STATIC);
    match(KW_VOID);
    match(KW_MAIN);
    match(SEP_LPAREN);
    match(KW_STRING);
    match(SEP_LBRACKET);
    match(SEP_RBRACKET);
    match(ID);
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statement();
    match(SEP_RBRACE);
    match(SEP_RBRACE);
}

// ClassDeclaration -> class ID (extends ID)? { (VarDeclaration)* (MethodDeclaration)* }
void
Parser::classDeclaration()
{
    match(KW_CLASS);
    match(ID);
    if (lToken->name == KW_EXTENDS) {
        match(KW_EXTENDS);
        match(ID);
    }
    match(SEP_LBRACE);

    while (lToken->name == KW_INT || lToken->name == KW_BOOLEAN || lToken->name == ID) {
       if (lToken->name == KW_PUBLIC) {
            methodDeclaration();
        }
        // Se for 'int', 'boolean' ou 'ID' pode ser VarDeclaration
        else if (lToken->name == KW_INT || lToken->name == KW_BOOLEAN || lToken->name == ID) {
            varDeclaration();
        } else {
            break; // Não é um início de VarDeclaration ou MethodDeclaration
        }
    }
    match(SEP_RBRACE);
}

// VarDeclaration -> Type ID;
void
Parser::varDeclaration()
{
    // VarDeclaration -> Type ID;
    type();
    match(ID);
    match(SEP_SEMICOLON);
}

// MethodDeclaration -> public Type ID ((Params)? ) { (VarDeclaration)* (Statement)* return Expression; }
void
Parser::methodDeclaration()
{
    match(KW_PUBLIC);
    type();
    match(ID);
    match(SEP_LPAREN);
    // (Params)? - zero ou uma ocorrência de Params
    // Verifica se o próximo token é um dos que iniciam 'Type' (int, boolean, ID) para saber se há Params
    if (lToken->name == KW_INT || lToken->name == KW_BOOLEAN || lToken->name == ID) {
        params();
    }
    match(SEP_RPAREN);
    match(SEP_LBRACE);

    // (VarDeclaration)* - zero ou mais declarações de variáveis locais
    while (lToken->name == KW_INT || lToken->name == KW_BOOLEAN || lToken->name == ID) {
        varDeclaration();
    }

    // (Statement)* - zero ou mais instruções
    // Entra no loop se o token atual é um dos que iniciam 'Statement'
    while (lToken->name == SEP_LBRACE || lToken->name == KW_IF || lToken->name == KW_WHILE ||
           lToken->name == KW_SYSTEM_OUT_PRINTLN || lToken->name == ID) {
        statement();
    }

    match(KW_RETURN);
    expression();
    match(SEP_SEMICOLON);
    match(SEP_RBRACE);
}

// Params -> Type ID (, Type ID)*
void
Parser::params()
{
    type();
    match(ID);
    while (lToken->name == SEP_COMMA) {
        match(SEP_COMMA);
        type();
        match(ID);
    }
}

// Type -> int ([])? | boolean | ID
void
Parser::type()
{
    // Type -> int ([])? | boolean | ID
    if (lToken->name == KW_INT) {
        match(KW_INT);
        if (lToken->name == SEP_LBRACKET) {
            match(SEP_LBRACKET);
            match(SEP_RBRACKET);
        }
    } else if (lToken->name == KW_BOOLEAN) {
        match(KW_BOOLEAN);
    } else if (lToken->name == ID) {
        match(ID);
    } else {
        error("Erro de sintaxe: Esperado tipo (int, boolean ou ID).");
    }
}

// Statement -> { (Statement)* }
void
Parser::statement()
{
    // Statement -> { (Statement)* }
    //             | if (Expression) Statement else Statement
    //             | while (Expression) Statement
    //             | System.out.println (Expression);
    //             | ID ([Expression ])? = Expression;

    if (lToken->name == SEP_LBRACE) {
        match(SEP_LBRACE);
        while (lToken->name == SEP_LBRACE || lToken->name == KW_IF || lToken->name == KW_WHILE ||
               lToken->name == KW_SYSTEM_OUT_PRINTLN || lToken->name == ID) {
            statement();
        }
        match(SEP_RBRACE);
    } else if (lToken->name == KW_IF) {
        match(KW_IF);
        match(SEP_LPAREN);
        expression();
        match(SEP_RPAREN);
        statement();
        match(KW_ELSE);
        statement();
    } else if (lToken->name == KW_WHILE) {
        match(KW_WHILE);
        match(SEP_LPAREN);
        expression();
        match(SEP_RPAREN);
        statement();
    } else if (lToken->name == KW_SYSTEM_OUT_PRINTLN) {
        match(KW_SYSTEM_OUT_PRINTLN);
        match(SEP_LPAREN);
        expression();
        match(SEP_RPAREN);
        match(SEP_SEMICOLON);
    } else if (lToken->name == ID) {
        match(ID);
        if (lToken->name == SEP_LBRACKET) { // ID [Expression] = Expression;
            match(SEP_LBRACKET);
            expression();
            match(SEP_RBRACKET);
        }
        match(OP_ASSIGN);
        expression();
        match(SEP_SEMICOLON);
    } else {
        error("Erro de sintaxe: Esperado uma declaração de instrução.");
    }
}

// Expression -> Primary ( (OP_AND | OP_LT | OP_GT | OP_EQ | OP_NEQ | OP_PLUS | OP_MINUS | OP_MULT | OP_DIV) Expression )*
void
Parser::expression()
{
    // Primeiro, faz o parser de uma expressão "primária" ou a parte inicial de uma expressão.
    // lida com o lado esquerdo das operações binárias e os literais/variáveis/chamadas de métodos
    primaryExpression();

    // Em seguida, lida com os operadores e as recursões à esquerda.
    // Continua enquanto o próximo token for um operador binário ou um ponto/colchete para acesso.
    while (lToken->name == OP_AND || lToken->name == OP_LT || lToken->name == OP_GT ||
           lToken->name == OP_EQ || lToken->name == OP_NEQ || lToken->name == OP_PLUS ||
           lToken->name == OP_MINUS || lToken->name == OP_MULT || lToken->name == OP_DIV ||
           lToken->name == SEP_LBRACKET || lToken->name == SEP_DOT)
    {
        if (lToken->name == OP_AND || lToken->name == OP_LT || lToken->name == OP_GT ||
            lToken->name == OP_EQ || lToken->name == OP_NEQ || lToken->name == OP_PLUS ||
            lToken->name == OP_MINUS || lToken->name == OP_MULT || lToken->name == OP_DIV)
        {
            // Expression Op Expression
            match(lToken->name); // Consome o operador
            primaryExpression(); // O lado direito da operação
        } else if (lToken->name == SEP_LBRACKET) {
            // Expression [Expression] (acesso a array)
            match(SEP_LBRACKET);
            expression();
            match(SEP_RBRACKET);
        } else if (lToken->name == SEP_DOT) {
            match(SEP_DOT);
            if (lToken->name == KW_LENGTH) {
                // Expression . length
                match(KW_LENGTH);
            } else if (lToken->name == ID) {
                // Expression . ID ((ExpressionsList)?) (chamada de método)
                match(ID);
                match(SEP_LPAREN);
                // (ExpressionsList)? - zero ou uma ocorrência de ExpressionsList
                if (lToken->name != SEP_RPAREN) { // Se não for ')' de cara, então há uma lista de expressões
                    expressionsList();
                }
                match(SEP_RPAREN);
            } else {
                error("Erro de sintaxe: Esperado 'length' ou ID após '.'.");
            }
        }
    }
}

// Helper function: primaryExpression
// Lida com as partes não-recursivas à esquerda de Expression.
// INTEGER LITERAL | true | false | ID | this | new int [Expression ] | new ID () | ! Expression | (Expression)
void
Parser::primaryExpression()
{
    if (lToken->name == INTEGER) {
        match(INTEGER);
    } else if (lToken->name == KW_TRUE) {
        match(KW_TRUE);
    } else if (lToken->name == KW_FALSE) {
        match(KW_FALSE);
    } else if (lToken->name == ID) {
        match(ID);
    } else if (lToken->name == KW_THIS) {
        match(KW_THIS);
    } else if (lToken->name == KW_NEW) {
        match(KW_NEW);
        if (lToken->name == KW_INT) { // new int [Expression ]
            match(KW_INT);
            match(SEP_LBRACKET);
            expression();
            match(SEP_RBRACKET);
        } else if (lToken->name == ID) { // new ID ()
            match(ID);
            match(SEP_LPAREN);
            match(SEP_RPAREN);
        } else {
            error("Erro de sintaxe: Esperado 'int' ou ID após 'new'.");
        }
    } else if (lToken->name == OP_NOT) { // ! Expression
        match(OP_NOT);
        expression();
    } else if (lToken->name == SEP_LPAREN) { // (Expression)
        match(SEP_LPAREN);
        expression();
        match(SEP_RPAREN);
    } else {
        error("Erro de sintaxe: Esperado o início de uma expressão válida.");
    }
}

// ExpressionsList -> Expression (, Expression)*
void
Parser::expressionsList()
{
    // ExpressionsList -> Expression (, Expression)*
    expression();
    while (lToken->name == SEP_COMMA) {
        match(SEP_COMMA);
        expression();
    }
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;
	exit(EXIT_FAILURE);
}