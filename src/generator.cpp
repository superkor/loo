#include <string>
#include <iostream>
#include "node.hpp"
#include "types.hpp"
#include <sstream>
#include <unordered_map>
#include "generator.hpp"

std::string Generator::generate(){
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
                
                if (currNode->getChild(0)->getChild(0)->getType() == Type::_int || isBinaryOperator(currNode->getChild(0)->getChild(0)->getType())){
                    //value of variable is pushed to stack              
                    //std::cout << "TEST TEST TEST" << (currNode->getChild(0)->getChild(0)->getType() == Type::add) << std::endl;          
                    compute(currNode->getChild(0)->getChild(0));
                    pop("rax");

                    //variable assigned a space in stack
                    variables[currNode->getChild(0)->getValue()] = stack_size;
                    //std::cout << "pushed " << currNode->getChild(0)->getValue() << std::endl;
                    push("rax");
                    continue;
                }

                else if (currNode->getChild(0)->getChild(0)->getType() == Type::identifier){
                    if (!variables.contains(currNode->getChild(0)->getChild(0)->getValue())){
                        throw "Identifier " + currNode->getChild(0)->getChild(0)->getValue() + " is not declared";
                    }

                    //variable assigned a space in stack
                    variables[currNode->getChild(0)->getValue()] = stack_size;
                    //std::cout << "pushed " << currNode->getChild(0)->getValue() << std::endl;

                    std::stringstream offset;
                    offset << "QWORD [rsp + " << (stack_size - variables[currNode->getValue()] - 1)* 8 << "]";
                    push(offset.str());
                    continue;
                } else {
                    throw "Something went wrong. Not an identifier or int";
                }

            }

            if (currNode->getType() == Type::exit){
                currNode = currNode->getChild(0);

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
                    output << "    mov rax, 60\n";
                    output << "    syscall\n";

                    continue;
                } else if (currNode->getType() == Type::_int || isBinaryOperator(currNode->getType())){
                    compute(currNode);
                    pop("rdi");
                    output << "    mov rax, 60\n";
                    output << "    syscall\n";
                    continue;
                }
            }
        }

        return output.str();
        
    } catch(std::string message) {
        std::cerr << message << std::endl;
        exit(EXIT_FAILURE);

    }
}