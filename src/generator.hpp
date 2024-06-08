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
                if (!gen->m_vars.contains(term_ident->ident.Literal.value())) {
                    std::cerr << "Undeclared identifier: " << term_ident->ident.Literal.value() << std::endl;
                    exit(EXIT_FAILURE);
                }
                const auto& var = gen->m_vars.at(term_ident->ident.Literal.value());
                std::stringstream offset;
                offset << "QWORD [rsp + " << (gen->m_stack_size - var.var_loc - 1) * 8 << "]\n";
                gen->push(offset.str());
            }
        };
        TermVisitor visitor({.gen = this});
        std::visit(visitor, term->term);
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
                gen->genExpression(bin_expr->add->lhs);
                gen->genExpression(bin_expr->add->rhs);
                gen->pop("rax");
                gen->pop("rbx");
                gen->m_output << "    add rax, rbx\n";
                gen->push("rax");
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
                if(gen->m_vars.contains(node_let->iden.Literal.value()))
                {
                    std::cerr << "Alredy used variable "<<node_let->iden.Literal.value();
                    exit(EXIT_FAILURE);
                }
                gen->m_vars.insert({node_let->iden.Literal.value() , var{.var_loc = gen->m_stack_size}});
                gen->genExpression(node_let->expr);
            }
            void operator()(const NodeReturn* node_ret) const
            {
                gen->genExpression(node_ret->exp);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
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
    std::stringstream m_output;
    NodeProg  m_prog;
    size_t m_stack_size=0;
    std::unordered_map<std::string,var> m_vars;
};
