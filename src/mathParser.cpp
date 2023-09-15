#include <iostream>
#include <stack>
#include "node.hpp"
#include "types.hpp"
#include "token.hpp"
#include "shared.cpp"

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
                //std::cout << parentOfRoot->getChild(0)->getValue() << std::endl;
                
                delete current;
                current = getNextToken();
                
            } catch (Type type){
                std::cerr << "something went wrong at token " << index << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        
        
        void parse(bool exitStatement = false){
            try {
                if (current == nullptr){
                    throw Type::_undefined;
                }

                if (current->type == Type::semiColon || (current->type == Type::closeRound && exitStatement)){
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
                }
                //was not expecting operator but token type is a binary operator 
                else if (!expectOperator && isBinaryOperator(current->type) ){
                    throw Type::_int;
                }
                if (!expectOperator && (current->type == Type::_int || current->type == Type::identifier)){
                    //std::cout << "current insertTo " << insertTo->getType() << std::endl;
                    //std::cout << "current " << current->type << " " << current->value << std::endl;

                    //binary operator already in tree has higher (or same) precedence
                    if (insertTo->getType() == Type::_int || insertTo->getType() == Type::identifier || compare(insertTo->getType(), operationBuffer->getType())){
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

                        insertTo = insertTo->getChild(1);
                        //std::cout << "new insertTo " << insertTo->getType() << " " << insertTo->getValue() << std::endl;

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
                    throw Type::_undefined;
                }


                return;
            } catch(Type type){
                delete current;
                std::cerr << "math parser - something went wrong at token " << index << std::endl;
                exit(EXIT_FAILURE);
            }

        }

        ~MathParser(){
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

        /**
         * determines which binary operator has higher precedence. 
         * binary operators with the same precendence will return false
        */
        bool compare(const Type& a, const Type&b){
            try {

                /* std::cout << "a " << a << std::endl;
                std::cout << "b " << b << std::endl; */

                std::vector<Type> operators = {Type::add, Type::subtract, Type::divide, Type::asterisk};

                if (std::find(operators.begin(), operators.end(), a) == operators.end() || std::find(operators.begin(), operators.end(), b) == operators.end()){
                    throw std::string("a or b is not a binary operator");
                }

                switch(a){
                    case Type::add:
                        if (b == Type::asterisk || b == Type::divide){
                            return false;
                        } else {
                            return true;
                        }
                    case Type::subtract:
                        if (b == Type::asterisk || b == Type::divide){
                            return false;
                        } else {
                            return true;
                        }
                    case Type::asterisk:
                        return true;
                    case Type::divide:
                        return true;
                    default:
                        return true;
                }


            } catch (std::string message){
                std::cerr << message << " at " << index << std::endl;
                exit(EXIT_FAILURE);
            }
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