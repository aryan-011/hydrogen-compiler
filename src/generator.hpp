//
// Created by ARYAN GUPTA on 04-06-2024.
//

#pragma once
#include <algorithm>
#include <sstream>

#include "ast.hpp"

class Generator
{
public:
    explicit Generator(NodeProg prog)
        :m_prog(std::move(prog))
    {
    }

    void genTerm(const NodeTerm* term) {
        struct TermVisitor {
            Generator* gen;
            void operator()(const NodeExprIntLit* term_int_lit) const {
                gen->m_output << "    mov rax, " << term_int_lit->int_lit.Literal.value() << "\n";
                gen->push("rax");
            }
            void operator()(const NodeExprIdent* term_ident) const {
                auto it = std::find_if(gen->m_vars.cbegin(),gen->m_vars.cend(),[&](const var& v )
                {
                    return v.name == term_ident->ident.Literal.value();
                });
                if (it == gen->m_vars.cend()) {
                    std::cerr << "Undeclared identifier: " << term_ident->ident.Literal.value() << std::endl;
                    exit(EXIT_FAILURE);
                }
                const auto& var = (*it);
                std::stringstream offset;
                offset << "QWORD [rsp + " << (gen->m_stack_size - var.var_loc - 1) * 8 << "]";
                gen->push(offset.str());
            }
            void operator()(const NodeTermParen* node_term_paren) const
            {
                gen->genExpression(node_term_paren->exp);
            }
        };
        TermVisitor visitor({.gen = this});
        std::visit(visitor, term->term);
    }
    void genBinExpr(const NodeBinExpr* node_bin_expr)
    {
        struct BinExpVisitor
        {
            Generator* gen;
            void operator()(const NodeBinExprAdd* node_bin_expr_add) const
            {
                gen->genExpression(node_bin_expr_add->rhs);
                gen->genExpression(node_bin_expr_add->lhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    add rax, rbx\n";
                gen->push("rax");
            }
            void operator()(const NodeBinExprMult* node_bin_expr_mult) const
            {
                gen->genExpression(node_bin_expr_mult->rhs);
                gen->genExpression(node_bin_expr_mult->lhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    mul rbx\n";
                gen->push("rax");
            }
            void operator()(const NodeBinExprSub* node_bin_expr_sub)const
            {
                gen->genExpression(node_bin_expr_sub->rhs);
                gen->genExpression(node_bin_expr_sub->lhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    sub rax, rbx\n";
                gen->push("rax");
            }
            void operator()(const NodeBinExprDiv* node_bin_expr_div)const
            {
                gen->genExpression(node_bin_expr_div->rhs);
                gen->genExpression(node_bin_expr_div->lhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    div rbx\n";
                gen->push("rax");
            }
        };
        BinExpVisitor visitor{.gen = this};
        std::visit(visitor,node_bin_expr->bin_exp);
    }
    void genExpression(const NodeExpr* node_expr)
    {
        struct ExprVisitor {
            Generator* gen;
            void operator()(const NodeTerm* term) const
            {
                gen->genTerm(term);
            }
            void operator()(const NodeBinExpr* bin_expr) const
            {
                gen->genBinExpr(bin_expr);
            }
        };

        ExprVisitor visitor { .gen = this };
        std::visit(visitor, node_expr->exp);
    }

    void genStatements(const NodeStatement* node_statement)
    {
        struct StmtVisitor
        {
            Generator* gen;
            void operator()(const NodeLet* node_let) const
            {
                auto it = std::find_if(gen->m_vars.cbegin(),gen->m_vars.cend(),[&](const var& v )
                {
                    return v.name == node_let->iden.Literal.value();
                });
                if(it!=gen->m_vars.cend())
                {
                    std::cerr << "Alredy used variable "<<node_let->iden.Literal.value();
                    exit(EXIT_FAILURE);
                }
                gen->m_vars.push_back({.name = node_let->iden.Literal.value(), .var_loc = gen->m_stack_size});
                gen->genExpression(node_let->expr);
            }
            void operator()(const NodeReturn* node_ret) const
            {
                gen->genExpression(node_ret->exp);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }
            void operator()(const NodeScopedStmts* node_scoped_stmts) const
            {
                gen->beginScope();
                for(auto stmt:node_scoped_stmts->stmts)
                {
                    gen->genStatements(stmt);
                }
                gen->endScope();
            }
        };

        StmtVisitor visitor{.gen = this};
        std::visit(visitor,node_statement->stmt);
    }


    [[nodiscard]] std::string genProg()
    {
        m_output << "global _start\n_start:\n";
        for (const NodeStatement* stmt : m_prog.stmts) {
            genStatements(stmt);
        }

        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0\n";
        m_output << "    syscall\n";
        return m_output.str();
    }

private:
    struct var
    {
        std::string name;
        size_t var_loc;
    };
    void push(const std::string& reg)
    {
        m_output << "    push "<<reg<<"\n";
        m_stack_size++;
    }
    void pop(const std::string& reg)
    {
        m_output << "    pop "<<reg<<"\n";
        m_stack_size--;
    }

    void beginScope()
    {
        m_scopes.push_back(m_vars.size());
    }
    void endScope()
    {
        size_t scopeVarCount = m_vars.size() - m_scopes.back();
        m_output<<"    add rsp, " << scopeVarCount*8 <<"\n";
        m_stack_size-=scopeVarCount;
        for(int i=0;i<scopeVarCount;i++)
        {
            m_vars.pop_back();
        }
        m_scopes.pop_back();
    }

    std::stringstream m_output;
    NodeProg  m_prog;
    size_t m_stack_size=0;
    // std::unordered_map<std::string,var> m_vars; removed to handle scopes
    std::vector<var> m_vars{};
    std::vector<size_t> m_scopes{};
};
