# Hydrogen Lang (CPP Compiler)

Hydrogen is a Hobby programming language. It is called Hydrogen because it is simple, lightweight, and will catch on
fire if handled improperly🔥

This compiler is written in C++.

## Building

Requires `nasm` and `ld` on a Linux operating system.

```bash
git clone https://github.com/aryan-011/hydrogen-compiler.git
cd hydrogen-compiler
mkdir build
cmake -S . -B build
cmake --build build

```
$$
\begin{align*}
[\text{Prog}] &\to [\text{Stmt}]^* \\
[\text{Stmt}] &\to
\begin{cases}
\text{return} [\text{Expr}]; \\
\text{let}\ \text{ident} = [\text{Expr}];\\
\{ [\text{Stmt}]^*\}\\
 if( \text{Cond} ) \{ [\text{Stmt}]^*\} \\
 if( \text{Cond} ) \{ [\text{Stmt}]^*\}else\{[\text{Stmt}]^*\} \\
\end{cases} \\
[Cond] &\to
\begin{cases}
[\text{term}] < [\text{term}]\\
[\text{term}] > [\text{term}]\\
[\text{term}] <= [\text{term}]\\
[\text{term}] >= [\text{term}]\\
[\text{term}] != [\text{term}]\\
[\text{term}] == [\text{term}]\\
\end{cases}\\
[\text{Expr}] &\to
\begin{cases}
[\text{Term}] \\
[\text{BinExpr}]
\end{cases} \\
[\text{BinExpr}] &\to
\begin{cases}
[\text{Expr}] * [\text{Expr}] & \text{prec} = 2 \\
[\text{Expr}] / [\text{Expr}] & \text{prec} = 2 \\
[\text{Expr}] + [\text{Expr}] & \text{prec} = 1 \\
[\text{Expr}] - [\text{Expr}] & \text{prec} = 1 \\
\end{cases} \\
[\text{Term}] &\to
\begin{cases}
\text{int\_lit} \\
\text{ident} \\
([\text{Expr}])
\end{cases}
\end{align*}
$$