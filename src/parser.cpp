#include <vector>
#include <string>
#include <iostream>
#include "types.hpp"
#include "token.hpp"
#include "node.hpp"

class Parser{
    public:
        Parser(std::vector<Token> tokens) : tokens(std::move(tokens)){};

        NodeExit parseExit(){
            
        }

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