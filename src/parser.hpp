#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"
#include <stack>

class Parser{
    public:
        Parser(const std::vector<Token> tokens) : tokens(std::move(tokens)){};

        ~Parser();

        Node* createParseTree();

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

        void traverse(Node* node){
            if (node == nullptr){
                return;
            }

            for (int i = 0; i < node->getChildren().size(); i++){
                traverse(node->getChild(i));
            }

            node->setParent(nullptr);
            delete node;
            node = nullptr;
            return;
        }


        const std::vector<Token> tokens;
        size_t index = 0;
        Token* nextToken = nullptr;
        Node* root = new Node();
        Node* curr = root;
};