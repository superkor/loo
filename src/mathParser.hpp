#pragma once

#include <iostream>
#include <stack>
#include "node.hpp"
#include "types.hpp"
#include "token.hpp"
#include "shared.cpp"
#include <algorithm>

class MathParser{
    public:
        MathParser(Node* toAppendTo, const std::vector<Token>& tokens, size_t& index) : parentOfRoot(toAppendTo), tokens(tokens), index(index) {
            try {
                current = getCurrToken();
                if (current == nullptr){
                    throw Type::_undefined;
                }
                //if the first token in the math expression is not a number or variable (do type checking later)
                if (current->type != Type::_int && current->type != Type::identifier && current->type != Type::openRound){
                    delete current;
                    current = nullptr;
                    throw Type::_int;
                } else if (current-> type == Type::openRound){
                    //if the first character in the math statement is an open round
                    delete current;
                    current = getNextToken();
                    
                    MathParser(parentOfRoot, tokens, index);

                    delete current;
                    current = getCurrToken();

                    insertTo = parentOfRoot->getChild(0);
                    bracketStack.push(parentOfRoot);


                    return;
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

        void parse(bool exitStatement);

        ~MathParser();

        private:
        Token* getCurrToken(){
            return new Token{tokens.at(index)};
        }

        Token* getNextToken(){
            if (index+1 >= tokens.size()){
                ++index;
                return nullptr;
            }
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

                std::vector<Type> operators = {Type::add, Type::subtract, Type::divide, Type::asterisk, Type::openRound, Type::closeRound};

                if (std::find(operators.begin(), operators.end(), a) == operators.end() || std::find(operators.begin(), operators.end(), b) == operators.end()){
                    throw std::string("a or b is not a binary operator");
                }

                switch(a){
                    case Type::openRound:
                        return false;
                    case Type::closeRound:
                        return false;
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

        std::stack<Node*> bracketStack;
        
        //override order of precendece (brackets)
        bool overrideOrder = false;

};