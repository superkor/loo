#pragma once

#include "token.hpp"

struct NodeExpr{
    Token _int;
};

struct NodeIdent{
    Token ident;
    NodeExpr expr;
};

struct NodeExit{
    NodeExpr expr;
};

struct Node {
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;

    Type type = Type::_undefined;
    std::string value = "";

    Node(Type type = Type::_undefined, std::string value = "fuck") : type(type), value(value) {}
};