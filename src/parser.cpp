#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"

class Parser{
    public:
        Parser(std::vector<Token> tokens) : tokens(std::move(tokens)){};

        NodeExit parse(){
            Token* nextToken = peek();
            Token* consumeToken = nullptr;
            while (nextToken != nullptr){
                //keyword exit
                NodeExit exitNode;
                if (nextToken->type == Type::exit){
                    //syntax: exit([expr])

                    //check if '(' is not next
                    if (nextToken == nullptr || nextToken->type != Type::openRound){
                        delete nextToken;
                        nextToken = nullptr;

                        exitError();
                    }

                    //consume '(' token
                    consumeToken = consume();
                    delete consumeToken;
                    consumeToken = nullptr;

                    //parse for expression
                    NodeExpr* nodeExprPtr = parseExitExpr();
                    if (nodeExprPtr != nullptr){
                        exitNode = NodeExit{.expr = nodeExprPtr->_int};

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
                    nextToken = peek();

                    //check if semi colon is next
                    if (nextToken == nullptr || nextToken->type != Type::semiColon){
                        delete nextToken;
                        nextToken = nullptr;

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
        }

        NodeExpr* parseExitExpr(){
            Token* peekToken = peek();

            //check expression (int)
            if (peekToken != nullptr && peekToken->type == Type::_int){
                Token* consumeToken = consume();

                return new NodeExpr{._int = *consumeToken};

                delete consumeToken;
                consumeToken = nullptr;
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

    private:
        Token* peek(int offset = 0) const {
            if (index + offset >= tokens.size()){
                return nullptr;
            }
            return new Token{tokens.at(index+offset)};
        }
        Token* consume(){
            if (peek() != nullptr){
                return new Token{tokens.at(index++)};
            } else {
                return nullptr;
            }
        }
        std::vector<Token> tokens;
        size_t index = 0;
};