#include "parser.h"

int main(int argc, char* argv[]) 
{
    //Verifica se foi executado corretamente
    //Essa main espera receber o nome do arquivo a ser
    //executado na linha de comando.
    if (argc != 2)
    {
        cout<< "Uncorrect format. Correct use: ./mj_compiler 'file_name'.mj\n";
        return 1;
    }

    Parser* parser = new Parser(argv[1]);
    parser->run();
    delete parser;

    return 0;
}