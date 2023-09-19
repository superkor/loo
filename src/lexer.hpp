#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"

class Lexer{
    public:
        Lexer(std::string in) : src(std::move(in)){};
        std::vector<Token> lex();
    private:
        char peek(int offset = 0) const{
            if (index + offset >= src.length()){
                return '\0';
            }

            return src.at(index + offset);
        };
        
        char consume(){
            return src.at(index++);
        };
        size_t index = 0;
        const std::string src;
};