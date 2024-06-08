#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "generator.hpp"
#include "parser.hpp"
#include "token.hpp"



int main(int argc , char* argv[] ){
    if(argc !=2)
    {
        std::cerr<<"Missing the source file possitional args"<<std::endl;
        return EXIT_FAILURE;
    }
    std::cout<<"Hello"<<std::endl;
    std::string contents;
    {
        std::fstream input(argv[1], std::ios::in);
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        contents=contents_stream.str();
    }

    // std::cout<<contents<<std::endl;
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();
    for( const auto& it: tokens)
    {
        std::cout<<TokenTypeToString(it.Type)<<" "<<it.Literal.value()<<std::endl;
    }

    Parser parser(std::move(tokens));
    std::optional<NodeProg> prog = parser.parseProg();

    if(!prog.has_value())
    {
        std::cerr << "Invalid Prog"<<std::endl;
        exit(EXIT_FAILURE);
    }


    {
        Generator generator(prog.value());
        std::fstream file("out.asm",std::ios::out);
        file << generator.genProg();
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}
