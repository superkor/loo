#include <string>
#include "node.hpp"
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

                        //value of variable is pushed to stack
                        output << "    mov rax, " << currNode->getChild(0)->getChild(0)->getValue() << "\n";
                        //variable assigned a space in stack
                        variables[currNode->getChild(0)->getValue()] = stack_size;
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
                            output << "    pop rdi\n";
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
};
