#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "symboltable.h"
#include <vector>

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
		SymbolTable* globaltable;
		bool compilationResult;
		const std::vector<int> syncTokens = {
			SEP_SEMICOLON,         // ;
			SEP_RBRACE,            // }
			KW_CLASS,              // class
			KW_PUBLIC,             // public
			KW_IF,                 // if
			KW_WHILE,              // while
			KW_RETURN,             // return
			KW_SYSTEM_OUT_PRINTLN, // System.out.println
			END_OF_FILE            // Fim do arquivo
		};

		// Métodos auxiliares
		void advance();
		void match(int);
		void initSymbolTable();
		void declaration(SymbolTable*, STEntry*);
		bool validOperation(int, SymbolType);

		void panicMode(); // modo pânico

		// Regras da gramática
		void program();
		void mainClass();
        void classDeclaration();
		void varDeclaration(SymbolTable*);
		void methodDeclaration(SymbolTable*);
		void params(SymbolTable*);
		SymbolType type(SymbolTable*);
		void statement(SymbolTable*);
		void expression(SymbolTable*, SymbolType);
		void expressionsList(SymbolTable*, SymbolType);

		
	public:
		// Construtor
		Parser(string);

		// Inicialização da análise sintática
		void run();

		 // Método para manipular erros
        void error(string);
};

#endif