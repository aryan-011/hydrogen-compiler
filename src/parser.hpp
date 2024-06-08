#pragma once

#include <iostream>
#include <optional>
#include <vector>

#include "arena.hpp"
#include "token.hpp"
#include "ast.hpp"

class Parser
{
public:
    explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
        ,m_allocator(1024 *1024 ) //4mb
    {
    }

    std::optional<NodeTerm*> parseTerm()
    {
        if (auto int_lit = tryConsume(TokenType::INT)) {
            auto term_int_lit = m_allocator.alloc<NodeExprIntLit>();
            term_int_lit->int_lit = int_lit.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->term = term_int_lit;
            return term;
        }
        else if (auto ident = tryConsume(TokenType::IDENT)) {
            auto expr_ident = m_allocator.alloc<NodeExprIdent>();
            expr_ident->ident = ident.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->term = expr_ident;
            return term;
        }
        else {
            return {};
        }
    }

    std::optional<NodeExpr*> parseExpr()
    {
        if (auto term = parseTerm()) {
            if (tryConsume(TokenType::PLUS).has_value()) {
                auto bin_expr = m_allocator.alloc<NodeBinExpr>();
                auto bin_expr_add = m_allocator.alloc<NodeBinExprAdd>();
                auto lhs_expr = m_allocator.alloc<NodeExpr>();
                lhs_expr->exp= term.value();
                bin_expr_add->lhs = lhs_expr;
                if (auto rhs = parseExpr()) {
                    bin_expr_add->rhs = rhs.value();
                    bin_expr->add = bin_expr_add;
                    auto expr = m_allocator.alloc<NodeExpr>();
                    expr->exp = bin_expr;
                    return expr;
                }
                else {
                    std::cerr << "Expected expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else {
                auto expr = m_allocator.alloc<NodeExpr>();
                expr->exp = term.value();
                return expr;
            }
        }
        else {
            return {};
        }
    }

    std::optional<NodeStatement*> parseStmts()
    {
        if (peek().has_value() && peek().value().Type == TokenType::RETURN)
        {
            consume();
            auto expr = parseExpr();
            if (expr.has_value() && peek().has_value() && peek().value().Type == TokenType::SEMICOLON)
            {
                consume(); // Consume the semicolon
                auto ret = m_allocator.alloc<NodeReturn>();
                ret->exp = expr.value();
                auto stmt = m_allocator.alloc<NodeStatement>();
                stmt->stmt = ret;
                return stmt;
            }
            else
            {
                std::cerr << "Error: expected expression and semicolon after return.\n";
                exit(EXIT_FAILURE);
            }
        }

        else if (peek().has_value() && peek().value().Type == TokenType::LET)
        {
            consume();//let
            auto ident = tryConsume(TokenType::IDENT);
            if ( ident.has_value() && peek().has_value() && peek().value().Type == TokenType::ASSIGN)
            {

                auto stmt_let = m_allocator.alloc<NodeLet>();
                stmt_let->iden = ident.value();
                consume();
                if (auto expr = parseExpr()) {
                    stmt_let->expr = expr.value();
                }
                else {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
                tryConsume(TokenType::SEMICOLON);
                auto stmt = m_allocator.alloc<NodeStatement>();
                stmt->stmt = stmt_let;
                return stmt;
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
        while (peek().has_value()) {
            if (auto stmt = parseStmts()) {
                prog.stmts.push_back(stmt.value());
            }
            else {
                std::cerr << "Invalid statement" << std::endl;
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

    inline std::optional<Token> tryConsume(TokenType type)
    {
        if (const auto token = peek(); token.has_value() && token.value().Type == type)
        {
            return consume();
        }
        return {};
    }

    std::vector<Token> m_tokens;
    size_t m_index = 0;
    ArenaAllocator m_allocator;
};
