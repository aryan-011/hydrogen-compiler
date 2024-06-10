//
// Created by ARYAN GUPTA on 02-06-2024.
//
#pragma once

#include <optional>
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
    LTE,
    GTE,
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

std::optional<int> binOpPrec(TokenType type)
{
    switch (type)
    {
    case TokenType::PLUS:
        return 1;
    case TokenType::MINUS:
        return 1;
    case TokenType::ASTERISK:
        return 2;
    case TokenType::SLASH:
        return 2;
    default:
        return {};
    }
}

struct Token {
    TokenType Type;
    std::optional<std::string> Literal{};

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
    case TokenType::LTE: return "<=";
    case TokenType::GTE: return ">=";
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
class Tokenizer
{
public:
    explicit Tokenizer(std::string src)
            : m_str(std::move(src))
    {
    }

inline std::vector<Token> tokenize()
{
    std::vector<Token> Tokens;
    std::string buf;
    while(peek().has_value())
    {
        if(isalpha(peek().value()))
        {
            buf.push_back(consume());
            while (peek().has_value() && isalnum(peek().value()))
            {
                buf.push_back(consume());
            }
            TokenType type = LookupIdentifier(buf);
            Tokens.emplace_back(type,type==TokenType::IDENT ?buf:TokenTypeToString(type));
            buf.clear();
        }
        else if(isdigit(peek().value()))
        {
            buf.push_back(consume());
            while(peek().has_value() && isdigit(peek().value()))
            {
                buf.push_back(consume());
            }
            Tokens.emplace_back(TokenType::INT,buf);
            buf.clear();
        }
        else if(peek().value()=='=')
        {
            buf.push_back(consume());
            if(peek().has_value() && peek().value() == '=')
            {
                buf.push_back(consume());
                Tokens.emplace_back(TokenType::EQ,buf);
                buf.clear();
                continue;
            }
            Tokens.emplace_back(TokenType::ASSIGN,buf);
            buf.clear();
        }
        else if (peek().value() == '!')
        {
            buf.push_back(consume());
            if(peek().has_value() && peek().value() == '=')
            {
                buf.push_back(consume());
                Tokens.emplace_back(TokenType::NOT_EQ,buf);
                buf.clear();
                continue;;
            }
            Tokens.emplace_back(TokenType::BANG,buf);
            buf.clear();
        }
        else if (peek().value() == '+')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::PLUS, buf);
            buf.clear();
        }
        else if (peek().value() == '-')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::MINUS, buf);
            buf.clear();
        }
        else if (peek().value() == '*')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::ASTERISK, buf);
            buf.clear();
        }
        else if (peek().value() == '/')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::SLASH, buf);
            buf.clear();
        }
        else if (peek().value() == '<')
        {
            buf.push_back(consume());
            if(peek().has_value() && peek().value() == '=')
            {
                buf.push_back(consume());
                Tokens.emplace_back(TokenType::LTE,buf);
                buf.clear();
                continue;
            }
            Tokens.emplace_back(TokenType::LT, buf);
            buf.clear();
        }
        else if (peek().value() == '>')
        {
            buf.push_back(consume());
            if(peek().has_value() && peek().value() == '=')
            {
                buf.push_back(consume());
                Tokens.emplace_back(TokenType::GTE,buf);
                buf.clear();
                continue;
            }
            Tokens.emplace_back(TokenType::GT, buf);
            buf.clear();
        }
        else if (peek().value() == ',')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::COMMA, buf);
            buf.clear();
        }
        else if (peek().value() == ';')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::SEMICOLON, buf);
            buf.clear();
        }
        else if (peek().value() == '(')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::LPAREN, buf);
            buf.clear();
        }
        else if (peek().value() == ')')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::RPAREN, buf);
            buf.clear();
        }
        else if (peek().value() == '{')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::LBRACE, buf);
            buf.clear();
        }
        else if (peek().value() == '}')
        {
            buf.push_back(consume());
            Tokens.emplace_back(TokenType::RBRACE, buf);
            buf.clear();
        }
        else if(isspace(peek().value()))
        {
            consume();
            continue;
        }
        else
        {
            std::cerr << "Missing something , See Doccs for more"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return Tokens;
}

private:
    [[nodiscard]]std::optional<char> peek(const int len = 0) const
    {

        if (m_index + len >= m_str.size())
        {
            return {};
        }
        else
        {
            return m_str.at(m_index + len);
        }
    }

    char consume()
    {
        return m_str.at(m_index++);
    }
    const std::string m_str;
    int m_index=0;
};