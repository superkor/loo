#include <vector>
#include <string>
#include <iostream>
#include "types.hpp"
#include "token.hpp"

struct NodeTerms{

};

class Parser{
    public:
        Parser(std::vector<Token> tokens) : tokens(std::move(tokens)){};

    private:
        Token peek(int offset = 0) const {
            if (index + offset >= tokens.size()){
                return {.type = Type::_undefined};
            }
            return tokens.at(index+offset);
        }
        Token consume(){
            if (peek().type != Type::_undefined){
                return tokens.at(index++);
            } else {
                return {};
            }
        }
        std::vector<Token> tokens;
        size_t index = 0;
};