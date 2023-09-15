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

        void add(){
            output << "    add rax, qword rbx\n";
            return;
        }

        void idivide(){
            output << "    idiv qword rbx\n";
            return;
        }

        void sub(){
            output << "    sub rax, qword rbx\n";
        }

        void mul(){
            output << "    mul qword rbx\n";
        }

        void compute(Node* node){
            if (node->getType() != Type::_int && node->getType() != Type::identifier){
                calculate(node);
                return;
            }
            if (node->getType() == Type::_int){
                output << "    mov rax, " << node->getValue() << "\n";
                push("rax");
                //std::cout << "pushed " << node->getValue() << std::endl;
                
                return;
            } else {
                //get variable from stack
                if (!variables.contains(node->getValue())){
                    throw "Identifier " + node->getValue() + " is not declared";
                }

                std::stringstream offset;
                offset << "QWORD [rsp + " << (stack_size - variables[node->getValue()] - 1)* 8 << "]";
                push(offset.str());
                return;
            }
        }

        //incomplete
        void calculate(Node* node){
            try {
                switch(node->getType()){
                    case Type::add:
                        //std::cout << "in add " << std::endl;
                        //left
                        compute(node->getChild(0));

                        //right
                        compute(node->getChild(1));
                        pop("rax");
                        pop("rbx");

                        add();

                        push("rax");

                        return;
                        
                    case Type::divide:
                        //std::cout << "in divide " << std::endl;

                        //left
                        compute(node->getChild(0));

                        //right
                        compute(node->getChild(1));
                        pop("rbx");
                        output << "    cdq\n";
                        pop("rax");

                        idivide();
                        push("rax");
                        return;
                        
                    case Type::subtract:
                        //std::cout << "in sub " << std::endl;

                        //left
                        compute(node->getChild(0));

                        //right
                        compute(node->getChild(1));

                        pop("rbx");
                        pop("rax");

                        sub();
                        push("rax");

                        return;
                        
                    case Type::asterisk:
                        //std::cout << "in mul " << std::endl;

                        //left
                        compute(node->getChild(0));

                        //right
                        compute(node->getChild(1));

                        pop("rbx");

                        pop("rax");

                        mul();
                        push("rax");

                        return;
                    default:
                        throw "got " + node->getValue();
                };  
            } catch(std::string message){
                std::cerr << "Invalid operator: " << message << std::endl;
                exit(EXIT_FAILURE);
            }
            
        }
};
