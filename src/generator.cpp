#include <string>
#include "node.hpp"
#include <sstream>

class Generator{
    public:
        Generator(NodeExit root) : root(std::move(root)){}

        std::string generate(){
            std::stringstream output;
            output << "global _start\n_start:\n";
            output << "    mov rax, 60\n";
            output << "    mov rdi, " << root.expr._int.value <<"\n";
            output << "    syscall\n";
            return output.str();
        }

    private:
        NodeExit root;
};
