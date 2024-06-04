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

struct NodeExpr
{
    std::variant<NodeExprIdent,NodeExprIntLit> exp;
};
struct NodeReturn
{
    NodeExpr exp;
};
struct NodeLet
{
    Token iden;
    NodeExpr expr ;
};
struct NodeStatement
{
    std::variant<NodeReturn,NodeLet> stmt;
};

struct NodeProg
{
    std::vector<NodeStatement> stmts;
};
