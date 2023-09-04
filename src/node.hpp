#pragma once

#include "token.hpp"

struct NodeExpr{
    Token _int;
};

struct NodeExit{
    NodeExpr expr;
};