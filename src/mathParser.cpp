#include <iostream>
#include <stack>
#include "node.hpp"
#include "types.hpp"
#include "token.hpp"
#include "shared.cpp"
#include <algorithm>
#include "mathParser.hpp"

void MathParser::parse(bool exitStatement = false){
    try {
        //std::cout << current->type << std::endl;

        if (current == nullptr){
            throw Type::_undefined;
        }

        if (current->type == Type::semiColon || (current->type == Type::closeRound && exitStatement && tokens.at(index+1).type == Type::semiColon)){
            if (!bracketStack.empty()){
                std::cerr << "Unclosed brackets!" << std::endl;
                std::cerr << "No ')' found at token " << index << std::endl;
                exit(EXIT_FAILURE);
            }
            //std::cout << "; FOUND " << std::endl;

            delete current;
            current = getNextToken();
            return;
        }             
        //expected operator and the token type is a binary operator
        if (expectOperator && isBinaryOperator(current->type)){

            operationBuffer = new Node(current->type);

            expectOperator = false;

            //std::cout << "got  " << current->type << " operator" << std::endl;

            delete current;
            current = getNextToken();                                        
            parse(exitStatement);
            
            return;
        } else if (current->type == Type::openRound || current->type == Type::closeRound) {             
            if (!expectOperator && current->type == Type::openRound){
                bracketStack.push(insertTo->getParent());

                //std::cout << "PUSHED BRACKET " << std::endl;

                overrideOrder = true;
                
                delete current;
                current = getNextToken();
                parse(exitStatement);
                return;
            } else if (expectOperator && current->type == Type::closeRound){
                if (bracketStack.empty()){
                    std::cerr << "No opening bracket to close!"<< std::endl;
                    std::cerr << "Found ')' at " << index << std::endl;
                    exit(EXIT_FAILURE);
                }
                insertTo = bracketStack.top();
                bracketStack.pop();

                //std::cout << "POPPED BRACKET" << std::endl;

                delete current;
                current = getNextToken();
                overrideOrder = true;
                parse(exitStatement);
                return;
            }
        }
        //was not expecting operator but token type is a binary operator 
        else if (!expectOperator && isBinaryOperator(current->type) ){
            throw Type::_int;
        }

        if (!expectOperator && (current->type == Type::_int || current->type == Type::identifier)){
            //std::cout << "current insertTo " << insertTo->getType() << std::endl;
            //std::cout << "current " << current->type << " " << current->value << std::endl;

            //binary operator already in tree has higher (or same) precedence
            if (!overrideOrder && (insertTo->getType() == Type::_int || insertTo->getType() == Type::identifier || compare(insertTo->getType(), operationBuffer->getType()))){
                //set binary operation as the root
                Node* tempParent = insertTo->getParent();
                //std::cout << "tempParent " << insertTo->getParent()->getType() << " " << insertTo->getParent()->getValue() << std::endl;

                insertTo->unsetParent();
                tempParent->removeChild(insertTo);
                Node* temp = insertTo;
                //std::cout << "temp " << temp->getType() << " " << temp->getValue() << std::endl;
                
                insertTo = operationBuffer;
                operationBuffer = nullptr;
                //std::cout << "new insertTo " << insertTo->getType() << " " << insertTo->getValue() << std::endl;
                
                //add integer literals/variables to child
                insertTo->addChild(temp);
                Node* newNode = new Node{current->type, current->value};
                //std::cout << "newNode " << newNode->getType() << " " << newNode->getValue() << std::endl;
                
                insertTo->addChild(newNode);
                insertTo->setParent(tempParent);
                tempParent->addChild(insertTo);
                newNode->setParent(insertTo);
                temp->setParent(insertTo);
                
                //std::cout << "insertToChild0 " << insertTo->getChild(0)->getType() << " " << insertTo->getChild(0)->getValue() << std::endl;
                //std::cout << "insertToChild1 " << insertTo->getChild(1)->getType() << " " << insertTo->getChild(1)->getValue() << std::endl;
            } else {
                //std::cout << "detected operator to be higher precendece " << current->type<< std::endl;
                

                //binary operator to be inserted has higher precedence
                Node* tempParent = insertTo;
                //std::cout << "tempParent " << insertTo->getType() << " " << insertTo->getValue() << std::endl;

                if (insertTo == parentOfRoot){
                    insertTo = insertTo->getChild(0);
                    //std::cout << "new insertTo " << insertTo->getType() << " " << insertTo->getValue() << std::endl;
                } else {
                    insertTo = insertTo->getChild(1);
                    //std::cout << "new insertTo " << insertTo->getType() << " " << insertTo->getValue() << std::endl;
                }

                tempParent->removeChild(insertTo);
                Node* temp = insertTo;
                //std::cout << "temp " << temp->getType() << " " << temp->getValue() << std::endl;

                insertTo = operationBuffer;
                tempParent->addChild(insertTo);
                insertTo->setParent(tempParent);

                insertTo->addChild(temp);
                Node* newNode = new Node{current->type, current->value};
                //std::cout << "newNode " << newNode->getType() << " " << newNode->getValue() << std::endl;

                insertTo->addChild(newNode);
                temp->setParent(insertTo);
                newNode->setParent(insertTo);

                //std::cout << "insertToChild0 " << insertTo->getChild(0)->getType() << " " << insertTo->getChild(0)->getValue() << std::endl;
                //std::cout << "insertToChild1 " << insertTo->getChild(1)->getType() << " " << insertTo->getChild(1)->getValue() << std::endl;
            }
            
            overrideOrder = false;
            delete current;
            current = getNextToken();
            expectOperator = true;
            parse(exitStatement);
            return;
            
        } //detected bracket 
        /* else if (!expectOperator && current->type == Type::openRound){
            brackets.push(true);
            //right child
            insertTo = insertTo->getChild(1);
        }  */
        
        else {
            delete insertTo;
            throw Type::_undefined;
        }


        return;
    } catch(Type type){
        delete current;
        std::cerr << "math parser - something went wrong at token " << index << " \nLooking at " << type << std::endl;
        exit(EXIT_FAILURE);
    }

}

MathParser::~MathParser(){
    delete current;
    current = nullptr;
}