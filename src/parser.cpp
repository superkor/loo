#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"

class Parser{
    public:
        Parser(std::vector<Token> tokens) : tokens(std::move(tokens)){};

        NodeExit* parse(){
            nextToken = peek();
            while (nextToken != nullptr){
                //keyword exit
                NodeExit* exitNode = nullptr;
                if (nextToken->type == Type::exit){
                    //syntax: exit([expr])

                    //consume exit token
                    consumeToken = consume();

                    delete nextToken;
                    nextToken = peek();

                    //check if '(' is not next
                    if (nextToken == nullptr || nextToken->type != Type::openRound){
                        delete nextToken;
                        nextToken = nullptr;
                        exitError();
                    }

                    //consume '(' token
                    consumeToken = consume();

                    //parse for expression
                    NodeExpr* nodeExprPtr = parseExitExpr();
                    if (nodeExprPtr != nullptr){
                        exitNode = new NodeExit{.expr = nodeExprPtr->_int};

                        delete nodeExprPtr;
                        nodeExprPtr = nullptr;
                    } else {
                        delete nextToken;
                        nextToken = nullptr;
                        exitError();
                    }

                    delete nextToken;
                    nextToken = peek();

                    //check if ')' exists

                    if (nextToken == nullptr || nextToken->type != Type::closeRound){
                        delete nextToken;
                        nextToken = nullptr;
                        exitError();
                    }

                    delete nextToken;

                    //consume ')' token
                    consumeToken = consume();

                    nextToken = peek();

                    //check if semi colon is next
                    if (nextToken == nullptr || nextToken->type != Type::semiColon){
                        delete nextToken;
                        nextToken = nullptr;

                        delete exitNode;
                        exitNode = nullptr;

                        exitError();
                    } else {
                        delete nextToken;
                        nextToken = nullptr;

                        return exitNode;
                    }
                }

                //get next token
                delete nextToken;
                nextToken = peek();
            }
            delete nextToken;
            nextToken = nullptr;

            return nullptr;
            
        }

        ~Parser(){
            delete nextToken;
            nextToken = nullptr;

            delete consumeToken;
            consumeToken = nullptr; 
        }

    private:
        Token* peek(int offset = 0) const {
            if (index + offset >= tokens.size()){
                return nullptr;
            }
            return new Token{tokens.at(index+offset)};
        }
        Token* consume(bool output = false){
            Token* peekToken = peek();
            if (peekToken != nullptr){
                delete peekToken;
                peekToken = nullptr;
                if (output){
                    return new Token{tokens.at(index++)};
                } else {
                    index++;
                    return nullptr;
                }
            } else {
                return nullptr;
            }
        }

        NodeExpr* parseExitExpr(){
            Token* peekToken = peek();

            //check expression (int)
            if (peekToken != nullptr && peekToken->type == Type::_int){
                Token* consumeToken = consume(true);

                NodeExpr* output = new NodeExpr{._int = *consumeToken};

                delete consumeToken;
                consumeToken = nullptr;

                delete peekToken;
                peekToken = nullptr;

                return output;
            } else {
                delete peekToken;
                peekToken = nullptr;

                return nullptr;
            }
        }

        void exitError(){
            std::cerr << "Invalid Expression" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::vector<Token> tokens;
        size_t index = 0;
        Token* nextToken = nullptr;
        Token* consumeToken = nullptr;
};