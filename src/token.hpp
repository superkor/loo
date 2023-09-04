#pragma once

#include "types.hpp"
#include <string>

struct Token{
    Type type;
    std::string value = "";
};