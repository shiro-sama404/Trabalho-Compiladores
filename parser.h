#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;

		void advance();
		void match(int);
		
	public:
		Parser(string);
		void run();
        void program();
        void mainClass();
        void classDeclaration();
		void varDeclaration();
		void methodDeclaration();
		void params();
		void type();
		void statement();
		void expression();
		void primaryExpression();
		void expressionsList();
        void error(string);
};