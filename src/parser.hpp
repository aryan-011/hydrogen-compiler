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
        ,m_allocator(1024 *1024*4 ) //4mb
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
        else if (auto open_paren = tryConsume(TokenType::LPAREN)) {
            auto expr = parseExpr();
            if (!expr.has_value()) {
                std::cerr << "Expected expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            auto rParen = tryConsume(TokenType::RPAREN);
            if(!rParen.has_value())
            {
                std::cerr << "Expected ')'";
                exit(EXIT_FAILURE);
            }
            auto term_paren = m_allocator.alloc<NodeTermParen>();
            term_paren->exp = expr.value();
            auto term = m_allocator.alloc<NodeTerm>();
            term->term = term_paren;
            return term;
        }
        else {
            return {};
        }
    }

    std::optional<NodeExpr*> parseExpr(const int min_prec =0)
    {
        std::optional<NodeTerm*> term_lhs = parseTerm();
        if(!term_lhs.has_value())
        {
            return {};
        }
        auto expr_lhs = m_allocator.alloc<NodeExpr>();
        expr_lhs->exp = term_lhs.value();
        while(true)
        {
            std::optional<Token> curr_token= peek();
            std::optional<int> bin_prec;
            if(curr_token.has_value())
            {
                bin_prec =  binOpPrec(curr_token->Type);
                if(!bin_prec.has_value() || bin_prec<min_prec)
                {
                    break;
                }
            }
            else
            {
                break;
            }

            auto op = consume();
            int next_prec = bin_prec.value()+1;
            auto expr_rhs = parseExpr(next_prec);

            if(!expr_rhs.has_value())
            {
                std::cerr << "Unable to parse Expreesiion";
                exit(EXIT_FAILURE);
            }
            auto expr = m_allocator.alloc<NodeBinExpr>();
            auto expr_lhs2 = m_allocator.alloc<NodeExpr>();
            if(op.Type == TokenType::PLUS)
            {
                auto add= m_allocator.alloc<NodeBinExprAdd>();
                expr_lhs2->exp = expr_lhs->exp;
                add->lhs=expr_lhs2;
                add->rhs = expr_rhs.value();
                expr->bin_exp = add;
            }
            else if(op.Type == TokenType::ASTERISK)
            {
                auto multi= m_allocator.alloc<NodeBinExprMult>();
                expr_lhs2->exp = expr_lhs->exp;
                multi->lhs=expr_lhs2;
                multi->rhs = expr_rhs.value();
                expr->bin_exp = multi;
            }
            else if(op.Type == TokenType::MINUS)
            {
                auto sub= m_allocator.alloc<NodeBinExprSub>();
                expr_lhs2->exp = expr_lhs->exp;
                sub->lhs=expr_lhs2;
                sub->rhs = expr_rhs.value();
                expr->bin_exp = sub;
            }
            else if(op.Type == TokenType::SLASH)
            {
                auto div= m_allocator.alloc<NodeBinExprDiv>();
                expr_lhs2->exp = expr_lhs->exp;
                div->lhs=expr_lhs2;
                div->rhs = expr_rhs.value();
                expr->bin_exp = div;
            }
            expr_lhs->exp = expr;
        }
        return expr_lhs;
    }

    std::optional<NodeCond*> parseCond()
    {
        auto term_lhs = parseTerm();
        if (!term_lhs) {
            return {};
        }

        auto op = consume();
        auto term_rhs = parseTerm();
        if (!term_rhs) {
            return {};
        }

        auto cond = m_allocator.alloc<NodeCond>();

        auto allocateAndSetCond = [&](auto node) {
            node->lhs = term_lhs.value();
            node->rhs = term_rhs.value();
            cond->condn = node;
            return cond;
        };

        switch (op.Type) {
        case TokenType::EQ:
            return allocateAndSetCond(m_allocator.alloc<NodeEQ>());
        case TokenType::NOT_EQ:
            return allocateAndSetCond(m_allocator.alloc<NodeNEQ>());
        case TokenType::LT:
            return allocateAndSetCond(m_allocator.alloc<NodeLT>());
        case TokenType::LTE:
            return allocateAndSetCond(m_allocator.alloc<NodeLTE>());
        case TokenType::GT:
            return allocateAndSetCond(m_allocator.alloc<NodeGT>());
        case TokenType::GTE:
            return allocateAndSetCond(m_allocator.alloc<NodeGTE>());
        default:
            std::cerr << "Unable to parse condition" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::optional<NodeScopedStmts*> parseScopedStmts()
    {
        if(!tryConsume(TokenType::LBRACE).has_value())
        {
            std::cerr << "expected '{'";
        }
        const auto scope_stmts = m_allocator.alloc<NodeScopedStmts>();
        while(auto stmt= parseStmts())
        {
            scope_stmts->stmts.push_back(stmt.value());
        }
        if(!tryConsume(TokenType::RBRACE).has_value())
        {
            std::cerr << "Expected '}'"<<std::endl;
            exit(EXIT_FAILURE);
        }
        return scope_stmts;
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
        else if(peek().has_value() && peek().value().Type == TokenType::LBRACE)
        {
            auto scope_stmts = parseScopedStmts();
            auto stmts = m_allocator.alloc<NodeStatement>();
            stmts->stmt=scope_stmts.value();
            return stmts;
        }
        else if(tryConsume(TokenType::IF).has_value())
        {
            if(!tryConsume(TokenType::LPAREN).has_value())
            {
                std::cerr << "Expected '('"<<std::endl;
                exit(EXIT_FAILURE);
            }
            auto condn = parseCond();
            if(!tryConsume(TokenType::RPAREN).has_value())
            {
                std::cerr << "Expected ')'"<<std::endl;
                exit(EXIT_FAILURE);
            }
            if(!condn.has_value())
            {
                std::cerr << "Unable to parse Condition"<<std::endl;
                exit(EXIT_FAILURE);
            }
            auto if_stmt = m_allocator.alloc<NodeIfStmt>();

            auto if_scope_stmts = parseScopedStmts();
            auto stmts = m_allocator.alloc<NodeStatement>();
            if(peek().has_value() && peek().value().Type == TokenType::ELSE)
            {
                consume();
                auto else_scope_stmts = parseScopedStmts();
                auto if_else_node = m_allocator.alloc<NodeIfElseStmt>();

                if_else_node->cond = condn.value();
                if_else_node->if_stmts = if_scope_stmts.value();
                if_else_node->else_stmts = else_scope_stmts.value();
                stmts->stmt = if_else_node;
                return stmts;
            }

            if_stmt->cond = condn.value();
            if_stmt->stmts =  if_scope_stmts.value();

            stmts->stmt = if_stmt;
            return stmts;
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
