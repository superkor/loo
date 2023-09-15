#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"
#include <stack>
#include "mathParser.cpp"
#include "shared.cpp"

class Parser{
    public:
        Parser(const std::vector<Token> tokens) : tokens(std::move(tokens)){};

        Node* createParseTree(){
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

                        //integer literal
                        if (nextToken != nullptr && nextToken->type == Type::_int){
                            Token* token = consume(true);
                            node = new Node(token->type, token->value);
                            node->setParent(curr);
                            curr->addChild(node);

                            delete token;
                            token = nullptr;

                            curr = node;

                            delete token;
                            token = nullptr;
                        } 
                        //variable
                        else if (nextToken != nullptr && nextToken->type == Type::identifier){
                            Token* token = consume(true);
                            node = new Node(token->type, token->value);
                            node->setParent(curr);
                            curr->addChild(node);

                            delete token;
                            token = nullptr;

                            curr = node;

                            delete token;
                            token = nullptr;
                        } 
                        
                        else {
                            throw Type::_int;
                        }

                        delete nextToken;
                        nextToken = peek();

                        if (nextToken == nullptr || nextToken->type != Type::closeRound){
                            std::cerr << nextToken->type << std::endl;
                            throw Type::closeRound;
                        }

                        consume();
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
                    throw Type::exit;
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

        ~Parser(){
            delete nextToken;
            nextToken = nullptr;

            traverse(root);
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
