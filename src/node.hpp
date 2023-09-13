#pragma once

#include <vector>

class Node {
    public:
        Node(Type type = Type::_undefined, std::string value = "") : type(type), value(value) {}

        Node* getParent(){ return parent; }

        void setParent(Node* parent){
            this->parent = parent;
        }

        std::vector<Node*> getChildren() { return children; }

        Node* getChild(int index){
            return children.at(index);
        }

        void addChild(Node* child){
            children.push_back(child);
        }

        void removeChild(int index){
            if (index < 0 || index > children.size()){
                return;
            }
            children.erase(std::next(children.begin(), index));
        }

        void removeChild(Node* child){
            for (int i = 0; i < children.size(); i++){
                if (child == children[i]){
                    children.erase(std::next(children.begin(), i));
                }
            }
        }

        void unsetParent(){
            parent = nullptr;
        }

        Type getType() { return type; }

        std::string getValue() { return value; }

    private:
        Node* parent = nullptr;
        std::vector<Node*> children;

        Type type = Type::_undefined;
        std::string value = "";
};