//
// Created by ARYAN GUPTA on 02-06-2024.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    ILLEGAL,
    EOFF,
    IDENT,
    INT,
    ASSIGN,
    PLUS,
    MINUS,
    BANG,
    ASTERISK,
    SLASH,
    LT,
    GT,
    EQ,
    NOT_EQ,
    COMMA,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    FUNCTION,
    LET,
    TRUE,
    FALSE,
    IF,
    ELSE,
    RETURN
};

// TokenType to string conversion for easy debugging and printing
std::string TokenTypeToString(TokenType type);

// Token structure with constructor
struct Token {
    TokenType Type;
    std::string Literal;

    // Constructor to initialize the fields
    Token(TokenType type, std::string  literal);
};

// Function to lookup identifiers and keywords
TokenType LookupIdentifier(const std::string& ident);

#endif //TOKEN_H
