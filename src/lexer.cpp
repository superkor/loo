#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"
#include "lexer.hpp"

std::vector<Token> Lexer::lex(){
    std::vector<Token> tokens;
    std::string buffer;

    while(peek() != '\0'){
        if (std::isalpha(peek())){
            buffer.push_back(consume());
            while(peek() != '\0' && std::isalnum(peek())){
                buffer.push_back(consume());
            }
            //std::cout << buffer << "\n";


            if (buffer == "return"){
                tokens.push_back({.type = Type::_return});
            } else if (buffer == "exit") {
                tokens.push_back({.type = Type::exit});
            } else if (buffer == "int"){
                tokens.push_back({.type = Type::int_type});
            }
            else {
                tokens.push_back({.type = Type::identifier, .value = buffer});
            } 

            buffer.clear();
        } else if (std::isdigit(peek())){
            buffer.push_back(consume());
            while(peek() != '\0' && std::isdigit(peek())){
                buffer.push_back(consume());
            }
            tokens.push_back({.type = Type::_int, .value = buffer});
            buffer.clear();
        } else if (peek() == '('){
            consume();
            tokens.push_back({.type = Type::openRound});
        } else if (peek() == ')'){
            consume();
            tokens.push_back({.type = Type::closeRound});
        } else if (peek() == ';'){
            consume();
            tokens.push_back({.type = Type::semiColon});
        } else if (std::iswspace(peek())){
            consume(); 
        } else if (peek() == '{'){
            consume();
            tokens.push_back({.type = Type::openCurly});
        } else if (peek() == '}'){
            consume();
            tokens.push_back({.type = Type::closeCurly});
        } else if (peek() == '='){
            consume();
            tokens.push_back({.type = Type::equals});
        } else if (peek() == '+'){
            consume();
            tokens.push_back({.type = Type::add});
        } else if (peek() == '/'){
            consume();
            tokens.push_back({.type = Type::divide});
        } else if (peek() == '-'){
            consume();
            tokens.push_back({.type = Type::subtract});
        } else if (peek() == '*'){
            consume();
            tokens.push_back({.type = Type::asterisk});
        }
        else {
            std::cerr << "Error at " << index << std::endl;
            exit(EXIT_FAILURE);
        }
        
    }

    index = 0;
    return tokens;
}
