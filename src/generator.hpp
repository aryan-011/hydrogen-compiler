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

    void genExpression(const NodeExpr& node_expr)
    {
        struct ExprVisitor
        {
            Generator* gen;
            void operator()(const NodeExprIdent& node_expr_ident) const
            {
                //todo
            }
            void operator()(const NodeExprIntLit& node_expr_int_lit) const
            {
                gen->m_output <<"    mov rax, " << node_expr_int_lit.int_lit.Literal << "\n" ;
                gen->m_output <<"    push rax\n";
            }
        };
        ExprVisitor visitor{.gen = this};
        std::visit(visitor,node_expr.exp);
    }

    void genStatements(const NodeStatement& node_statement)
    {
        struct StmtVisitor
        {
            Generator* gen;
            void operator()(const NodeLet& node_let) const
            {

            }
            void operator()(const NodeReturn& node_ret) const
            {
                gen->genExpression(node_ret.exp);
                gen->m_output << "    mov rax, 60\n";
                gen->m_output << "    pop rdi \n";
                gen->m_output << "    syscall\n";
            }
        };

        StmtVisitor visitor{.gen = this};
        std::visit(visitor,node_statement.stmt);
    }


    [[nodiscard]] std::string genProg()
    {
        m_output << "global _start\n_start:\n";
        for (const NodeStatement& stmt : m_prog.stmts) {
            genStatements(stmt);
        }

        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0\n";
        m_output << "    syscall\n";
        return m_output.str();
    }

private:
    std::stringstream m_output;
    NodeProg  m_prog;
    // size_t m_index=0;
};
