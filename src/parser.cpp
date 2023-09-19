#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"
#include <stack>
#include "mathParser.cpp"
#include "shared.cpp"
#include "parser.hpp"

Node* Parser::createParseTree(){
    try {
        bool hasExit = false;
        nextToken = peek();
        Node* last = curr;


        while (nextToken != nullptr){
            curr = last;
            //std::cout << nextToken->type << std::endl;
            //std::cout << "index" << index << std::endl;


            //declaring variable
            if (nextToken->type == Type::int_type){
                consume();

                Node* node = new Node(Type::int_type);
                node->setParent(curr);
                curr->addChild(node);

                curr = node;

                delete nextToken;
                nextToken = peek();

                //identifier
                if (nextToken == nullptr || nextToken->type != Type::identifier){
                    throw Type::identifier;
                }

                Token* token = consume(true);
                
                node = new Node(Type::identifier, token->value);
                node->setParent(curr);
                curr->addChild(node);

                delete token;
                token = nullptr;
                
                curr = node;

                delete nextToken;
                nextToken = peek();

                //equals

                if (nextToken == nullptr || nextToken->type != Type::equals){
                    throw Type::equals;
                }

                consume();

                delete nextToken;
                nextToken = peek();

                MathParser* math = new MathParser(curr, tokens, index);

                math->parse();

                delete math;

                delete nextToken;
                nextToken = peek();

                continue;

            }
            //get keyword exit
            if (nextToken->type == Type::exit){
                consume();

                Node* node = new Node(Type::exit);
                node->setParent(curr);
                curr->addChild(node);

                curr = node;

                delete nextToken;
                nextToken = peek();

                //check if '(' is not next
                if (nextToken == nullptr || nextToken->type != Type::openRound){
                    throw Type::openRound;
                }

                consume();

                delete nextToken;
                nextToken = peek();

                MathParser* math = new MathParser(curr, tokens, index);

                math->parse(true);

                delete math;

                delete nextToken;
                nextToken = peek();

                if (nextToken == nullptr || nextToken->type != Type::semiColon){
                    throw Type:: semiColon;
                }

                consume();
                                
                delete nextToken;
                nextToken = peek();

                hasExit = true;

                continue;
            }

        }

        if (!hasExit){
            curr = last;

            Node* node = new Node(Type::exit);
            node->setParent(curr);
            curr->addChild(node);

            curr = node;

            node = new Node(Type::_int, "0");
            node->setParent(curr);
            curr->addChild(node);
        }

        return root;
    }
    catch (Type expected){
        delete nextToken;
        nextToken = nullptr;
        std::cerr << "Invalid Expression at token " << index << "!" << std::endl;
        std::cerr << "Expected " << expected << std::endl;
        exit(EXIT_FAILURE);
    }
    
}

Parser::~Parser(){
    delete nextToken;
    nextToken = nullptr;

    traverse(root);
}