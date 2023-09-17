#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer.cpp"
#include "parser.cpp"
#include <vector>
#include "generator.cpp"

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Please specify a `.loo` file." << std::endl;
        std::cerr << "Usage: loo <input.loo>" << std::endl;
        return EXIT_FAILURE;
    } else if (argc > 2){
        std::cerr << "You have specified too many parameters." << std::endl;
        std::cerr << "Usage: loo <input.loo>" << std::endl;
        return EXIT_FAILURE;
    }
    //std::cout << argv[1] << std::endl;
    
    std::string contents;

    std::stringstream contents_stream;

    std::fstream input(argv[1], std::ios::in);

    if (input.fail()){
        std::cerr << "args[1] does not exist. Please ensure the file exists." << std::endl;
        return EXIT_FAILURE;
    }

    contents_stream << input.rdbuf();

    contents = contents_stream.str();

    input.close();

    Lexer lexer(std::move(contents));

    std::vector<Token> tokens = lexer.lex();
    
    Parser* parser = new Parser(std::move(tokens));

    Node* tree = parser->createParseTree();

    Generator generator(std::move(tree));

    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.generate();
    }

    //assemble the x86_64 assembly code
    system("nasm -felf64 out.asm");

    //link asm
    system("ld -o out out.o");

    delete parser;
    parser = nullptr;

    std::cout << argv[1] << " has been compiled." << std::endl;
    std::cout << "Run ./out to run the program." << std::endl;

    return EXIT_SUCCESS;
}