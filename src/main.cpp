#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer.cpp"
#include "parser.cpp"
#include <vector>

int main(int argc, char* argv[]){
    if (argc != 2){
        std::cerr << "Please specify a `.loo` file:" << std::endl;
        std::cerr << "loo <input.loo>" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << argv[1] << std::endl;
    
    std::string contents;
    {
        std::stringstream contents_stream;

        std::fstream input(argv[1], std::ios::in);

        contents_stream << input.rdbuf();

        contents = contents_stream.str();
    }

    Lexer lexer(std::move(contents));

    std::vector<Token> test = lexer.lex();

    std::cout << test[1].value << std::endl;


    return EXIT_SUCCESS;
}