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
                if(!gen->m_vars.contains(node_expr_ident.ident.Literal))
                {
                    std::cerr << "Undeclared Identifier used"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                const auto& var= gen->m_vars.at(node_expr_ident.ident.Literal);
                std::stringstream ss;
                ss << "QWORD [rsp + "<<(gen->m_stack_size - var.var_loc - 1) * 8 <<"]";//8 for 64 bits as we are only using 64 bit register
                gen->push(ss.str());
            }
            void operator()(const NodeExprIntLit& node_expr_int_lit) const
            {
                gen->m_output <<"    mov rax, " << node_expr_int_lit.int_lit.Literal << "\n" ;
                gen->push("rax");
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
                if(gen->m_vars.contains(node_let.iden.Literal))
                {
                    std::cerr << "Alredy used variable "<<node_let.iden.Literal;
                    exit(EXIT_FAILURE);
                }
                gen->m_vars.insert({node_let.iden.Literal , var{.var_loc = gen->m_stack_size}});
                gen->genExpression(node_let.expr);
            }
            void operator()(const NodeReturn& node_ret) const
            {
                gen->genExpression(node_ret.exp);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
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
