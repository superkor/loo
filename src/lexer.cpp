#include <vector>
#include <string>
#include <iostream>

enum class Type{
    _return,
    _int,
    _float,
    semiColon,
    openRound,
    closeRound,
    identifier,
    let
};

struct Token{
    Type type;
    std::string value = "";
};

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
        int index = 0;
        const std::string src;
};

std::vector<Token> Lexer::lex(){
    std::vector<Token> tokens;
    std::string buffer;

    while(peek() != '\0'){
        if (std::isalpha(peek())){
            buffer.push_back(consume());
            while(peek() != '\0' && std::isalnum(peek())){
                buffer.push_back(consume());
            }


            if (buffer == "return"){
                tokens.push_back({.type = Type::_return});
            } else if (buffer == "let"){
                tokens.push_back({.type = Type::let});
            } else {
                tokens.push_back({.type = Type::identifier, .value = buffer});
            }

            buffer.clear();
            continue;
        } else if (std::isdigit(peek())){
            buffer.push_back(consume());
            while(peek() != '\0' && std::isdigit(peek())){
                buffer.push_back(consume());
            }

            tokens.push_back({.type = Type::_int, .value = buffer});
            buffer.clear();
            continue;
        } else if (peek() == '('){
            consume();
            tokens.push_back({.type = Type::openRound});
            continue;
        } else if (peek() == ')'){
            consume();
            tokens.push_back({.type = Type::closeRound});
            continue;
        } else if (peek() == ';'){
            consume();
            tokens.push_back({.type = Type::semiColon});
            continue;
        } else if (std::iswspace(peek())){
            consume(); 
            continue;
        } else {
            std::cerr << "Error at " << index << std::endl;
            exit(EXIT_FAILURE);
        }
        
    }

    index = 0;
    return tokens;
}
