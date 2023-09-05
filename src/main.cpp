#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer.cpp"
#include "parser.cpp"
#include <vector>
#include "generator.cpp"

int main(int argc, char* argv[]){
    if (argc != 2){
        std::cerr << "Please specify a `.loo` file:" << std::endl;
        std::cerr << "loo <input.loo>" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << argv[1] << std::endl;
    
    std::string contents;

    std::stringstream contents_stream;

    std::fstream input(argv[1], std::ios::in);

    contents_stream << input.rdbuf();

    contents = contents_stream.str();

    input.close();

    Lexer lexer(std::move(contents));

    std::vector<Token> tokens = lexer.lex();

    /* for (Token x : tokens){
        std::string output = "";
        if (x.type == Type::exit){
            output = "exit";
        } else if (x.type == Type::openRound){
            output = "(";
        } else if (x.type == Type::closeRound){
            output = ")";
        } else if (x.type == Type::_int){
            output = "int";
        } else if (x.type == Type::semiColon){
            output = ";";
        }
        std::cout<< "type: " << output << "\t";
        std::cout << "value: " << x.value << std::endl;
    }
 */

    Parser parser(std::move(tokens));
    NodeExit* node = parser.parse();

    if (node != nullptr){
        std::cout << node->expr._int.value << std::endl;
    } else {
        std::cout << "something went wrong" << std::endl;
        return EXIT_FAILURE;
    }

    Generator generator(std::move(*node));

    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.generate();
    }

    delete node;
    node = nullptr;

    //assemble the x86_64 assembly code
    system("nasm -felf64 out.asm");

    //link asm
    system("ld -o out out.o");


    return EXIT_SUCCESS;
}