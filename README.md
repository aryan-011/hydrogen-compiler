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
<pre>
The grammar supported is:

[Prog] → [Stmt]*

[Stmt] → {
    return[Expr];
    let ident = [Expr];
    {[Stmt]*}
    if(Cond){[Stmt]*}
    if(Cond){[Stmt]*}else{[Stmt]*}
}

[Cond] → {
    [Term] &lt; [Term]
    [Term] &gt; [Term]
    [Term] &lt;= [Term]
    [Term] &gt;= [Term]
    [Term]! = [Term]
    [Term] == [Term]
}

[Expr] → {
    [Term]
    [BinExpr]
}

[BinExpr] → {
    [Expr] * [Expr]    prec = 2
    [Expr]/[Expr]      prec = 2
    [Expr] + [Expr]    prec = 1
    [Expr] - [Expr]    prec = 1
}

[Term] → {
    int_lit
    ident
    ([Expr])
}
</pre>
