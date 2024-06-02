//
// Created by ARYAN GUPTA on 02-06-2024.
//
#include "token.h"
#include <unordered_map>
#include <utility>

// TokenType to string conversion for easy debugging and printing
std::string TokenTypeToString(const TokenType type) {
    switch (type) {
    case TokenType::ILLEGAL: return "ILLEGAL";
    case TokenType::EOFF: return "EOF";
    case TokenType::IDENT: return "IDENT";
    case TokenType::INT: return "INT";
    case TokenType::ASSIGN: return "=";
    case TokenType::PLUS: return "+";
    case TokenType::MINUS: return "-";
    case TokenType::BANG: return "!";
    case TokenType::ASTERISK: return "*";
    case TokenType::SLASH: return "/";
    case TokenType::LT: return "<";
    case TokenType::GT: return ">";
    case TokenType::EQ: return "==";
    case TokenType::NOT_EQ: return "!=";
    case TokenType::COMMA: return ",";
    case TokenType::SEMICOLON: return ";";
    case TokenType::LPAREN: return "(";
    case TokenType::RPAREN: return ")";
    case TokenType::LBRACE: return "{";
    case TokenType::RBRACE: return "}";
    case TokenType::FUNCTION: return "FUNCTION";
    case TokenType::LET: return "LET";
    case TokenType::TRUE: return "TRUE";
    case TokenType::FALSE: return "FALSE";
    case TokenType::IF: return "IF";
    case TokenType::ELSE: return "ELSE";
    case TokenType::RETURN: return "RETURN";
    default: return "UNKNOWN";
    }
}


// Why Use std::move?
// Efficiency: Moving an object is generally more efficient than copying it.
// When you move an object, resources like dynamically allocated memory can be
// transferred from the source object to the destination object without copying the data.
// Rvalue References: std::move casts an lvalue (which has a persistent state) to an rvalue
// (which is a temporary object). This allows the constructor to use the move constructor
// of std::string instead of the copy constructor

Token::Token(const TokenType type, std::string  literal)
        : Type(type), Literal(std::move(literal)) {}


std::unordered_map<std::string,TokenType> keywords = {
    {"function",    TokenType::FUNCTION},
    {"let",    TokenType::LET},
    {"true",   TokenType::TRUE},
    {"false",  TokenType::FALSE},
    {"if",     TokenType::IF},
    {"else",   TokenType::ELSE},
    {"return", TokenType::RETURN},
};

TokenType LookupIdentifier(const std::string& iden )
{
    if(const auto it = keywords.find(iden); it!=keywords.end())
    {
        return it->second;
    }
    return TokenType::IDENT;
}