#pragma once
#include <ostream>
#include "types.hpp"

std::ostream& operator << (std::ostream &os, Type const& types){
    switch(types){
        case Type::openRound:
            return os << "(";
        case Type::closeRound:
            return os << ")";
        case Type::semiColon:
            return os << ";";
        case Type::exit:
            return os << "exit";
        case Type::_int:
            return os << "int";
        case Type::identifier:
            return os << "identifier";
        case Type::equals:
            return os << "=";
        case Type::add:
            return os << "+";
        case Type::subtract:
            return os << "-";
        case Type::divide:
            return os << "/";
        case Type::asterisk:
            return os << "*";
        case Type::_undefined:
            return os << "Unknown Type";
        default:
            return os;
    }
} 