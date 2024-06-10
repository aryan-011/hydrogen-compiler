//
// Created by ARYAN GUPTA on 03-06-2024.
//
#pragma once
#include <variant>
#include <vector>

#include "token.hpp"

struct NodeExprIntLit
{
    Token int_lit;
};


struct NodeExprIdent
{
    Token ident;
};
struct NodeExpr;

struct NodeBinExprAdd
{
    NodeExpr* lhs;
    NodeExpr* rhs;
};
struct NodeBinExprMult
{
    NodeExpr* lhs;
    NodeExpr* rhs;
};
struct NodeBinExprSub
{
    NodeExpr* lhs;
    NodeExpr* rhs;
};
struct NodeBinExprDiv
{
    NodeExpr* lhs;
    NodeExpr* rhs;
};
struct NodeTermParen
{
    NodeExpr* exp;
};
struct NodeTerm
{
    std::variant<NodeExprIdent*,NodeExprIntLit*,NodeTermParen*> term;
};
struct NodeBinExpr
{
    std::variant<NodeBinExprAdd*,NodeBinExprMult*,NodeBinExprSub*,NodeBinExprDiv*> bin_exp;
};

struct NodeExpr
{
    std::variant<NodeTerm*,NodeBinExpr*> exp;
};
struct NodeReturn
{
    NodeExpr* exp;
};
struct NodeLet
{
    Token iden;
    NodeExpr* expr ;
};
struct NodeStatement;

struct NodeScopedStmts
{
    std::vector<NodeStatement*> stmts;
};
struct NodeLTE
{
    NodeTerm* lhs;
    NodeTerm* rhs;
};
struct NodeGTE
{
    NodeTerm* lhs;
    NodeTerm* rhs;
};

struct NodeLT
{
    NodeTerm* lhs;
    NodeTerm* rhs;
};
struct NodeGT
{
    NodeTerm* lhs;
    NodeTerm* rhs;
};
struct NodeEQ
{
    NodeTerm* lhs;
    NodeTerm* rhs;
};
struct NodeNEQ
{
    NodeTerm* lhs;
    NodeTerm* rhs;
};
struct NodeCond
{
    std::variant<NodeLTE*,NodeGTE*,NodeLT*,NodeGT*,NodeEQ*,NodeNEQ*> condn;
};
struct NodeIfStmt
{
    NodeCond* cond;
    NodeScopedStmts* stmts;
};
struct NodeIfElseStmt
{
    NodeCond* cond;
    NodeScopedStmts* if_stmts;
    NodeScopedStmts* else_stmts;
};
struct NodeStatement
{
    std::variant<NodeReturn*,NodeLet*,NodeScopedStmts*,NodeIfStmt*,NodeIfElseStmt*> stmt;
};

struct NodeProg
{
    std::vector<NodeStatement*> stmts;
};
