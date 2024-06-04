#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include "token.hpp"
#include "ast.hpp"

class Parser
{
public:
    explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {
    }

    std::optional<NodeExpr> parseExpr()
    {
        if (auto token = peek(); token.has_value() && token.value().Type == TokenType::INT)
        {
            return NodeExpr{.exp = NodeExprIntLit{.int_lit = consume()}};
        }
        else if (auto token = peek(); token.has_value() && token.value().Type == TokenType::IDENT)
        {
            // Handle identifier expression
            return {};
        }
        return {};
    }

    std::optional<NodeStatement> parseStmts()
    {
        if (peek().has_value() && peek().value().Type == TokenType::RETURN)
        {
            consume();
            auto expr = parseExpr();
            if (expr.has_value() && peek().has_value() && peek().value().Type == TokenType::SEMICOLON)
            {
                consume(); // Consume the semicolon
                return NodeStatement{.stmt = NodeReturn{.exp = expr.value()}};
            }
            else
            {
                std::cerr << "Error: expected expression and semicolon after return.\n";
                exit(EXIT_FAILURE);
            }
        }

        if (peek().has_value() && peek().value().Type == TokenType::LET)
        {
            consume();
            if (auto ident = tryConsume(TokenType::IDENT); ident.has_value() && peek().has_value() && peek().value().Type == TokenType::EQ)
            {
                consume(); // Consume '='
                auto expr = parseExpr();
                if (expr.has_value())
                {
                    return NodeStatement{.stmt = NodeLet{.iden = ident.value(), .expr = expr.value()}};
                }
                else
                {
                    std::cerr << "Error: expected expression after '=' in let statement.\n";
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                std::cerr << "Error: expected identifier and '=' in let statement.\n";
                exit(EXIT_FAILURE);
            }
        }

        return {};
    }

    std::optional<NodeProg> parseProg()
    {
        NodeProg prog;
        while (peek().has_value())
        {
            std::cout << "Parsing token: " << TokenTypeToString(peek().value().Type) << std::endl;
            if (auto stmt = parseStmts(); stmt.has_value())
            {
                prog.stmts.push_back(stmt.value());
            }
            else
            {
                std::cerr << "Error: expected statement.\n";
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }

private:
    [[nodiscard]] inline std::optional<Token> peek(const size_t len = 0) const
    {
        if (m_index + len >= m_tokens.size())
        {
            return {};
        }
        else
        {
            return m_tokens.at(m_index + len);
        }
    }

    inline Token consume()
    {
        return m_tokens.at(m_index++);
    }

    inline std::optional<Token> tryConsume(const TokenType type)
    {
        if (const auto token = peek(); token.has_value() && token.value().Type == type)
        {
            return consume();
        }
        return {};
    }

    std::vector<Token> m_tokens;
    size_t m_index = 0;
};
