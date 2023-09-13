#include <iostream>
#include <stack>
#include "node.hpp"
#include "types.hpp"
#include "token.hpp"

class MathParser{
    public:
        MathParser(Node* toAppendTo, const std::vector<Token>& tokens, size_t& index) : parentOfRoot(toAppendTo), tokens(tokens), index(index) 
        {
            try {
                current = getCurrToken();
                if (current == nullptr){
                    throw Type::_undefined;
                }
                //if the first token in the math expression is not a number or variable (do type checking later)
                if (current->type != Type::_int && current->type != Type::identifier){
                    delete current;
                    current = nullptr;
                    throw Type::_int;
                }
                insertTo = new Node(current->type, current->value);
                insertTo->setParent(parentOfRoot);
                parentOfRoot->addChild(insertTo);
                std::cout << parentOfRoot->getChild(0)->getValue() << std::endl;
                delete current;
                current = getNextToken();
                
            } catch (Type type){
                std::cerr << "something went wrong at token " << index << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        
        //not complete
        void parse(){
            try {
                if (current == nullptr){
                    throw Type::_undefined;
                }

                if (current->type == Type::semiColon){
                    delete current;
                    current = getNextToken();
                    return;
                }

                
                //expected operator and the token type is a binary operator
                if (expectOperator && isBinaryOperator(current->type)){
                    operationBuffer = new Node(current->type);
                    expectOperator = false;

                    delete current;
                    current = getNextToken();                    
                    parse();
                    
                    return;
                }
                //was not expecting operator but token type is a binary operator 
                else if (!expectOperator && isBinaryOperator(current->type) ){
                    throw Type::_int;
                }
                if (!expectOperator && (current->type != Type::_int || current->type != Type::identifier)){
                    //set binary operation as the root
                    Node* tempParent = insertTo->getParent();
                    insertTo->unsetParent();
                    tempParent->removeChild(insertTo);
                    Node* temp = insertTo;
                    std::cout << "temp" << temp->getValue() << std::endl;
                    insertTo = operationBuffer;
                    operationBuffer = nullptr;
                    std::cout << "insertTo" << insertTo->getValue() << std::endl;
                    //add integer literals/variables to child
                    insertTo->addChild(temp);
                    Node* newNode = new Node{current->type, current->value};
                    insertTo->addChild(newNode);
                    insertTo->setParent(tempParent);
                    tempParent->addChild(insertTo);
                    newNode->setParent(insertTo);
                    temp->setParent(insertTo);
                    
                    std::cout << "insertToChild0" << insertTo->getChild(0)->getValue() << std::endl;
                    std::cout << "insertToChild1" << insertTo->getChild(1)->getValue() << std::endl;

                    delete current;
                    current = getNextToken();
                    expectOperator = true;
                    parse();
                    return;
                    
                } //detected bracket 
                /* else if (!expectOperator && current->type == Type::openRound){
                    brackets.push(true);
                    //right child
                    insertTo = insertTo->getChild(1);
                }  */
                
                else {
                    throw Type::_undefined;
                }


                return;
            } catch(Type type){
                std::cerr << "math parser - something went wrong at token " << index << std::endl;
                exit(EXIT_FAILURE);
            }

        }

        ~MathParser(){
            delete operationBuffer;
            operationBuffer = nullptr;
            delete current;
            current = nullptr;
        }
    private:
        Token* getCurrToken(){
            return new Token{tokens.at(index)};
        }

        Token* getNextToken(){
            return new Token{tokens.at(++index)};
        }

        bool isBinaryOperator(Type type){
            switch(type){
                case Type::add:
                    return true;
                case Type::divide:
                    return true;
                case Type::subtract:
                    return true;
                case Type::asterisk:
                    return true;
                default:
                    return false;
            };      
        }

        Node* parentOfRoot = nullptr;
        //tracks open brackets to ensure all brackets are closed
        std::stack<bool> brackets;

        Node* operationBuffer = nullptr;

        //points to where in the tree should a node be inserted
        Node* insertTo = nullptr;
        Token* current = nullptr;

        size_t& index;
        const std::vector<Token>& tokens;

        //flag to expect a binary operator in the next token
        bool expectOperator = true;

};