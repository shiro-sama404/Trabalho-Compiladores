#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
    globaltable = new SymbolTable();
    compilationResult = true;
}

void
Parser::initSymbolTable(){
    Token* t;

    t = new Token(KW_CLASS, "class");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_PUBLIC, "public");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_STATIC, "static");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_VOID, "void");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_MAIN, "main");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_STRING, "String");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_EXTENDS, "extends");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_TRUE, "true");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_FALSE, "false");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_THIS, "this");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_NEW, "new");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_RETURN, "return");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_IF, "if");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_ELSE, "else");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_WHILE, "while");
    globaltable->add(new STEntry(t, CAT_RESERVED, SYM_RESERVED, true));
    t = new Token(KW_LENGTH, "length");
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
    cout << "Position: " + to_string(scanner->getPos()) + ". Token: " << lToken->name << " '" << lToken->lexeme << "'" << endl;
}

void
Parser::match(int t)
{
	if (lToken->name == t)
		advance();
	else
		error("Expected '" + Token::toLexeme(t) + "'. Found " + " '" + lToken->lexeme + "'");
}

void
Parser::declaration(SymbolTable* table, STEntry* newSymbol){
    STEntry* reservedEntry = globaltable->get(newSymbol->token->getLexeme());

    if(reservedEntry && reservedEntry->reserved){
        error("'" + lToken->lexeme + "' is a reserved keyword");
    }

    if(table->getParent()){
        reservedEntry = table->getParent()->get(newSymbol->token->getLexeme());
        if(reservedEntry && reservedEntry->type == SYM_CLASS){
            error("Conflicting declaration of '" + lToken->lexeme + "'");
        }
    }

    if(!table->add(newSymbol)){
        error("Redeclaration of: " + lToken->lexeme);
    }
}

bool
Parser::validOperation(int op, SymbolType sType){
    if(sType == SYM_INT || sType == SYM_UNKNOWN){
        if(op == OP_PLUS || op == OP_MINUS || op == OP_MULT || op == OP_DIV){
            return true;
        }
    }else if(sType == SYM_BOOLEAN || sType == SYM_UNKNOWN){
        if(op == OP_AND || op == OP_LT || op == OP_GT || op == OP_EQ || op == OP_NEQ || op == OP_NOT){
            return true;
        }
    }
    return false;
}

void
Parser::run()
{
    initSymbolTable();
	advance();	

	program();

    if (compilationResult){
        if (lToken->name == END_OF_FILE) {
            cout << "Compilation ended successfully!\n";
        } else {
            error("Unexpected content found after end of the program.");
            exit(EXIT_FAILURE);
        }
    }else{
        exit(EXIT_FAILURE);
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
    SymbolTable* currentTable =  new SymbolTable(globaltable);

    match(KW_CLASS);
    STEntry* Entry = new STEntry(lToken, CAT_CLASS, SYM_CLASS, false);
    declaration(globaltable, Entry);
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
    declaration(currentTable, new STEntry(lToken, CAT_PARAMETER, SYM_STRING_ARRAY, false));
    match(ID);
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statement(currentTable);
    match(SEP_RBRACE);
    match(SEP_RBRACE);
}

// ClassDeclaration -> class ID (extends ID)? { (VarDeclaration)* (MethodDeclaration)* }
void
Parser::classDeclaration()
{
    SymbolTable* currentTable =  new SymbolTable(globaltable);

    match(KW_CLASS);
    declaration(globaltable, new STEntry(lToken, CAT_CLASS, SYM_CLASS, false));
    match(ID);
    if (lToken->name == KW_EXTENDS) {
        match(KW_EXTENDS);
        if(!currentTable->get(lToken->getLexeme())){
            error("‘" + lToken->getLexeme() + "’ was not declared in this scope.");
        }else{
            match(ID);
        }
    }
    match(SEP_LBRACE);

    while (lToken->name == KW_INT || lToken->name == KW_BOOLEAN || lToken->name == ID) {
        varDeclaration(currentTable);
    }
    while (lToken->name == KW_PUBLIC) {
        methodDeclaration(currentTable);
    }

    match(SEP_RBRACE);
}

// VarDeclaration -> Type ID;
void
Parser::varDeclaration(SymbolTable* currentTable)
{
    type(currentTable);
    declaration(currentTable, new STEntry(lToken, CAT_VARIABLE, SYM_STRING_ARRAY, false));
    match(ID);
    match(SEP_SEMICOLON);
}

// MethodDeclaration -> public Type ID ((Params)? ) { (VarDeclaration)* (Statement)* return Expression; }
void
Parser::methodDeclaration(SymbolTable* parentTable)
{
    SymbolTable* currentTable =  new SymbolTable(parentTable);

    match(KW_PUBLIC);
    SymbolType methodType = type(currentTable);
    declaration(currentTable, new STEntry(lToken, CAT_METHOD, methodType, false));
    match(ID);
    match(SEP_LPAREN);
    // (Params)? - zero ou uma ocorrência de Params
    // Verifica se o próximo token é um dos que iniciam 'Type' (int, boolean, ID) para saber se há Params
    if (lToken->name == KW_INT || lToken->name == KW_BOOLEAN || lToken->name == ID) {
        params(currentTable);
    }
    match(SEP_RPAREN);
    match(SEP_LBRACE);

    // (VarDeclaration)* - zero ou mais declarações de variáveis locais
    while (lToken->name == KW_INT || lToken->name == KW_BOOLEAN || (lToken->name == ID && scanner->peekNextToken(1)->getName() == ID)) {
        varDeclaration(currentTable);
    }

    // (Statement)* - zero ou mais instruções
    // Entra no loop se o token atual é um dos que iniciam 'Statement'
    while (lToken->name == SEP_LBRACE || lToken->name == KW_IF || lToken->name == KW_WHILE ||
           lToken->name == KW_SYSTEM_OUT_PRINTLN || lToken->name == ID) {
        statement(currentTable);
    }

    match(KW_RETURN);
    expression(currentTable, methodType);
    match(SEP_SEMICOLON);
    match(SEP_RBRACE);
}

// Params -> Type ID (, Type ID)*
void
Parser::params(SymbolTable* currentTable)
{
    declaration(currentTable, new STEntry(lToken, CAT_PARAMETER, type(currentTable), false));
    match(ID);
    while (lToken->name == SEP_COMMA) {
        match(SEP_COMMA);
        declaration(currentTable, new STEntry(lToken, CAT_PARAMETER, type(currentTable), false));
        match(ID);
    }
}

// Type -> int ([])? | boolean | ID
SymbolType
Parser::type(SymbolTable* currentTable)
{
    // Type -> int ([])? | boolean | ID
    if (lToken->name == KW_INT) {
        match(KW_INT);
        if (lToken->name == SEP_LBRACKET) {
            match(SEP_LBRACKET);
            match(SEP_RBRACKET);
            return SYM_INT_ARRAY;
        }else{
            return SYM_INT;
        }
    } else if (lToken->name == KW_BOOLEAN) {
        match(KW_BOOLEAN);
        return SYM_BOOLEAN;
    } else if (lToken->name == ID) {
        if(!currentTable->get(lToken->getLexeme())){
            error("‘" + lToken->getLexeme() + "’ was not declared in this scope.");
        }else{
            match(ID);
        }
        return SYM_ID;
    } else {
        return SYM_UNKNOWN;
    }
}

// Statement -> { (Statement)* }
void
Parser::statement(SymbolTable* currentTable)
{
    // Statement -> { (Statement)* }
    //             | if (Expression) Statement else Statement
    //             | while (Expression) Statement
    //             | System.out.println (Expression);
    //             | ID ([Expression ])? = Expression;

     while  (lToken->name == SEP_LBRACE || lToken->name == KW_IF || lToken->name == KW_WHILE || lToken->name == KW_SYSTEM_OUT_PRINTLN || lToken->name == ID) {

        if (lToken->name == SEP_LBRACE) {
            match(SEP_LBRACE);
            while (lToken->name == SEP_LBRACE || lToken->name == KW_IF || lToken->name == KW_WHILE ||
                lToken->name == KW_SYSTEM_OUT_PRINTLN || lToken->name == ID) {
                statement(currentTable);
            }
            match(SEP_RBRACE);
        } else if (lToken->name == KW_IF) {
            match(KW_IF);
            match(SEP_LPAREN);
            expression(currentTable, SYM_BOOLEAN);
            match(SEP_RPAREN);
            statement(currentTable);
            match(KW_ELSE);
            statement(currentTable);
        } else if (lToken->name == KW_WHILE) {
            match(KW_WHILE);
            match(SEP_LPAREN);
            expression(currentTable, SYM_BOOLEAN);
            match(SEP_RPAREN);
            statement(currentTable);
        } else if (lToken->name == KW_SYSTEM_OUT_PRINTLN) {
            match(KW_SYSTEM_OUT_PRINTLN);
            match(SEP_LPAREN);
            expression(currentTable, SYM_UNKNOWN);
            match(SEP_RPAREN);
            match(SEP_SEMICOLON);
        } else if (lToken->name == ID) {

            STEntry* entry = currentTable->get(lToken->getLexeme());
            SymbolType sType;

            if(!entry){
                sType = entry->type;
                error("‘" + lToken->getLexeme() + "’ was not declared in this scope.");
            }else{
                sType = SYM_UNKNOWN;
                match(ID);
            }
                
            if (lToken->name == SEP_LBRACKET) { // ID [Expression] = Expression;
                match(SEP_LBRACKET);
                expression(currentTable, SYM_INT);
                match(SEP_RBRACKET);
            }
            match(OP_ASSIGN);
            expression(currentTable, sType);
            match(SEP_SEMICOLON);
        }
    }
}

// Expression -> Expression Op Expression
//             | Expression [ Expression ]
//             | Expression . length
//             | Expression . ID ( ( ExpressionsList)? )
//             | INTEGER_LITERAL
//             | true
//             | false
//             | ID
//             | this
//             | new int [ Expression ]
//             | new ID ( )
//             | ! Expression
//             | ( Expression )
void
Parser::expression(SymbolTable* currentTable, SymbolType sType)
{
    do{
        if (lToken->name == INTEGER) {
            match(INTEGER);
        } else if (lToken->name == KW_TRUE) {
            match(KW_TRUE);
        } else if (lToken->name == KW_FALSE) {
            match(KW_FALSE);
        } else if (lToken->name == ID) {
            if(!currentTable->get(lToken->getLexeme())){
                error("‘" + lToken->getLexeme() + "’ was not declared in this scope.");
            }else{
                match(ID);
            }
        } else if (lToken->name == KW_THIS) {
            match(KW_THIS);
        } else if (lToken->name == KW_NEW) {
            match(KW_NEW);
            if (lToken->name == KW_INT) { // new int [Expression ]
                match(KW_INT);
                match(SEP_LBRACKET);
                expression(currentTable, SYM_INT);
                match(SEP_RBRACKET);
            } else if (lToken->name == ID) { // new ID ()
                if(!currentTable->get(lToken->getLexeme())){
                    error("‘" + lToken->getLexeme() + "’ was not declared in this scope.");
                }else{
                    match(ID);
                    match(SEP_LPAREN);
                    match(SEP_RPAREN);
                }
            }
        } else if (lToken->name == OP_NOT) { // ! Expression
            match(OP_NOT);
            expression(currentTable, SYM_BOOLEAN);
        } else if (lToken->name == SEP_LPAREN) { // (Expression)
            match(SEP_LPAREN);
            expression(currentTable, sType);
            match(SEP_RPAREN);
        }else{
            error("Invalid start of expression: ‘" + lToken->getLexeme() + "’.");
        }

        if (lToken->name == OP_AND || lToken->name == OP_LT || lToken->name == OP_GT ||
            lToken->name == OP_EQ || lToken->name == OP_NEQ || lToken->name == OP_PLUS ||
            lToken->name == OP_MINUS || lToken->name == OP_MULT || lToken->name == OP_DIV)
        {
            // Expression Op Expression
            if(!validOperation(lToken->name, sType)){
                error("'" + lToken->getLexeme() + "' is not a valid operation for type '" + to_string(sType) + "'.");
            }else{
                match(lToken->name); // Consome o operador
                expression(currentTable, sType); // O lado direito da operação
            }
        } else if (lToken->name == SEP_LBRACKET) {
            // Expression [Expression] (acesso a array)
            match(SEP_LBRACKET);
            expression(currentTable, sType);
            match(SEP_RBRACKET);
        } else if (lToken->name == SEP_DOT) {
            match(SEP_DOT);
            if (lToken->name == KW_LENGTH) {
                // Expression . length
                match(KW_LENGTH);
            } else if (lToken->name == ID) {
                // Expression . ID ((ExpressionsList)?) (chamada de método)
                if(!currentTable->get(lToken->getLexeme())){
                    error("‘" + lToken->getLexeme() + "’ was not declared in this scope.");
                }else{
                    match(ID);
                    match(SEP_LPAREN);
                    // (ExpressionsList)? - zero ou uma ocorrência de ExpressionsList
                    if (lToken->name != SEP_RPAREN) { // Se não for ')' de cara, então há uma lista de expressões
                        expressionsList(currentTable, sType);
                    }
                    match(SEP_RPAREN);
                }
            } else {
                error("Expected 'length' or ID after '.'.");
            }

            if (lToken->name == OP_AND || lToken->name == OP_LT || lToken->name == OP_GT ||
            lToken->name == OP_EQ || lToken->name == OP_NEQ || lToken->name == OP_PLUS ||
            lToken->name == OP_MINUS || lToken->name == OP_MULT || lToken->name == OP_DIV)
            {
                // Expression Op Expression
                if(!validOperation(lToken->name, sType)){
                    error("'" + lToken->getLexeme() + "' is not a valid operation for type '" + to_string(sType) + "'.");
                }else{
                    match(lToken->name); // Consome o operador
                    expression(currentTable, sType); // O lado direito da operação
                }
            }
        }
    }while (lToken->name == INTEGER || lToken->name == KW_TRUE || lToken->name == KW_FALSE || lToken->name == ID || lToken->name == KW_THIS || lToken->name == KW_NEW || lToken->name == OP_NOT || lToken->name == SEP_LPAREN);
}

// ExpressionsList -> Expression (, Expression)*
void
Parser::expressionsList(SymbolTable* currentTable, SymbolType sType)
{
    // ExpressionsList -> Expression (, Expression)*
    expression(currentTable, sType);
    while (lToken->name == SEP_COMMA) {
        match(SEP_COMMA);
        expression(currentTable, sType);
    }
}

// modo pânico
void
Parser::panicMode(){
    while (lToken->name != END_OF_FILE) {
        bool foundSyncToken = false;
        for (int syncToken : syncTokens) {
            if (lToken->name == syncToken) {
                foundSyncToken = true;
                break;
            }
        }

        if (foundSyncToken) {
            break;
        } else {
            advance();
        }
    }
}

void
Parser::error(string str)
{
	cout << scanner->getFilename() + ":" << scanner->getLine() << " sintax error: " << str << endl;
    compilationResult = false;
    panicMode();
}