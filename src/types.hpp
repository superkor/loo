#pragma once

/**
 * All possible keywords or valid expressions
*/
enum class Type{
    //unknown
    _undefined,
    //return statement
    _return,
    //exit statement
    exit,
    //integer literal
    _int,
    //float literal
    _float,
    //;
    semiColon,
    //(
    openRound,
    //)
    closeRound,
    //variable name
    identifier,
    //{
    openCurly,
    //}
    closeCurly,
    //int datatype
    int_type,
    //'='
    equals,
    //'+'
    add,
    // '/'
    divide,
    // '-'
    subtract,
    // '*'
    asterisk,
};
