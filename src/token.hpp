//
// Created by ARYAN GUPTA on 02-06-2024.
//
#pragma once

#include <unordered_map>
#include <utility>
#include <vector>
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

struct Token {
    TokenType Type;
    std::string Literal;

    // Constructor to initialize the fields
    Token(TokenType type, std::string  literal);
};

// TokenType to string conversion for easy debugging and printing
inline std::string TokenTypeToString(const TokenType type) {
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

inline Token::Token(const TokenType type, std::string  literal)
        : Type(type), Literal(std::move(literal)) {}


inline std::unordered_map<std::string,TokenType> keywords = {
    {"function",    TokenType::FUNCTION},
    {"let",    TokenType::LET},
    {"true",   TokenType::TRUE},
    {"false",  TokenType::FALSE},
    {"if",     TokenType::IF},
    {"else",   TokenType::ELSE},
    {"return", TokenType::RETURN},
};

inline TokenType LookupIdentifier(const std::string& iden )
{
    if(const auto it = keywords.find(iden); it!=keywords.end())
    {
        return it->second;
    }
    return TokenType::IDENT;
}

inline std::vector<Token> tokenize(const std::string& str)
{
    std::vector<Token> Tokens;
    std::string buf;
    for(int i=0;i<str.size();i++)
    {
        char ch= str.at(i);
        if(isalpha(ch))
        {
            buf.push_back(ch);
            i++;
            while (isalnum(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            TokenType type = LookupIdentifier(buf);
            Tokens.emplace_back(type,type==TokenType::IDENT ?buf:TokenTypeToString(type));
            buf.clear();
        }
        else if(isdigit(ch))
        {
            buf.push_back(ch);
            i++;
            while(isdigit(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            Tokens.emplace_back(TokenType::INT,buf);
            buf.clear();
        }
        else if(ch=='=')
        {
            buf.push_back(ch);
            if(str.at(i+1) == '=')
            {
                buf.push_back(str.at(++i));
                Tokens.emplace_back(TokenType::EQ,buf);
                continue;
            }
            Tokens.emplace_back(TokenType::ASSIGN,buf);
            buf.clear();
        }
        else if (ch == '!')
        {
            buf.push_back(ch);
            if(str.at(i+1) == '=')
            {
                buf.push_back(str.at(++i));
                Tokens.emplace_back(TokenType::NOT_EQ,buf);
                continue;;
            }
            Tokens.emplace_back(TokenType::BANG,buf);
            buf.clear();
        }
        else if (ch == '+')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::PLUS, buf);
            buf.clear();
        }
        else if (ch == '-')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::MINUS, buf);
            buf.clear();
        }
        else if (ch == '*')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::ASTERISK, buf);
            buf.clear();
        }
        else if (ch == '/')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::SLASH, buf);
            buf.clear();
        }
        else if (ch == '<')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::LT, buf);
            buf.clear();
        }
        else if (ch == '>')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::GT, buf);
            buf.clear();
        }
        else if (ch == ',')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::COMMA, buf);
            buf.clear();
        }
        else if (ch == ';')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::SEMICOLON, buf);
            buf.clear();
        }
        else if (ch == '(')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::LPAREN, buf);
            buf.clear();
        }
        else if (ch == ')')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::RPAREN, buf);
            buf.clear();
        }
        else if (ch == '{')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::LBRACE, buf);
            buf.clear();
        }
        else if (ch == '}')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::RBRACE, buf);
            buf.clear();
        }
        else if(isspace(ch))
        {
            continue;
        }
    }
    return Tokens;
}