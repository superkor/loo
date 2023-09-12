#include <string>
#include "node.hpp"
#include <sstream>

class Generator{
    public:
        Generator(Node* root) : root(std::move(root)){}

        std::string generate(){
            std::stringstream output;
            output << "global _start\n_start:\n";
            output << "    mov rax, 60\n";

            //temp
            output << "    mov rdi, " << root->left->left->value <<"\n";
            output << "    syscall\n";
            return output.str();
        }

    private:
        Node* root;
};
