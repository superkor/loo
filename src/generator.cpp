#include <string>
#include <iostream>
#include "node.hpp"
#include "types.hpp"
#include <sstream>
#include <unordered_map>

class Generator{
    public:
        Generator(Node* root) : root(std::move(root)){}

        std::string generate(){
            try {
                output << "global _start\n_start:\n";

                for (int i = 0; i < root->getChildren().size(); i++){
                    Node* currNode = root->getChild(i);
                    //variable
                    if (currNode->getType() == Type::int_type){
                        //variable already exists
                        if (variables.contains(currNode->getChild(0)->getValue())){
                            throw "Identifier " + currNode->getChild(0)->getValue() + " already exists";
                        }

                        std::cout << "on identifier " << currNode->getChild(0)->getValue() << std::endl;
                        
                        //value of variable is pushed to stack                        
                        output << "    mov rax, " << compute(currNode->getChild(0)->getChild(0)) << "\n";
                        //variable assigned a space in stack
                        variables[currNode->getChild(0)->getValue()] = stack_size;
                        std::cout << "pushed " << currNode->getChild(0)->getValue() << std::endl;
                        push("rax");
                        

                        continue;
                    }

                    if (currNode->getType() == Type::exit){
                        currNode = currNode->getChild(0);
                        output << "    mov rax, 60\n";

                        if (currNode->getType() == Type::identifier){
                            std::unordered_map<std::string, size_t>::iterator variable = variables.find(currNode->getValue());
                            if (variable == variables.end()){
                                throw "Variable " + currNode->getValue() + " not declared";
                            }

                            //variable exists in stack; get value from stack
                            std::stringstream offset;
                            offset << "QWORD [rsp + " << (stack_size - variables[currNode->getValue()] - 1)* 8 << "]";
                            push(offset.str());
                            pop("rdi");
                            output << "    syscall\n";

                            continue;
                        } else {

                        }
                    }
                }

                return output.str();
                
            } catch(std::string message) {
                std::cerr << message << std::endl;
                exit(EXIT_FAILURE);

            }
        }

    private:
        Node* root;
        size_t stack_size = 0;
        std::unordered_map<std::string, size_t> variables;
        std::stringstream output;

        void push(const std::string& reg){
            output << "    push " << reg << "\n";
            stack_size++;
        }

        void pop(const std::string& reg){
            output << "    pop " << reg << "\n";
            stack_size--;
        }

        int compute(Node* node){
            if (isBinaryOperator(node->getType())){
                return calculate(node);
            }
            return stoi(node->getValue());
        }

        //incomplete
        int calculate(Node* node){
            switch(node->getType()){
                case Type::add:
                    return stoi(node->getChild(0)->getValue()) + stoi(node->getChild(1)->getValue());
                case Type::divide:
                    return stoi(node->getChild(0)->getValue()) / stoi(node->getChild(1)->getValue());;
                case Type::subtract:
                    return stoi(node->getChild(0)->getValue()) - stoi(node->getChild(1)->getValue());;
                case Type::asterisk:
                    return stoi(node->getChild(0)->getValue()) * stoi(node->getChild(1)->getValue());;
                default:
                    return false;
            };  
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
};
