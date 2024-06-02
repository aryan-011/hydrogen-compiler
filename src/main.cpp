#include <fstream>
# include <iostream>
#include <sstream>
#include <vector>
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

    std::cout<<contents<<std::endl;

    for(std::vector<Token> tokens = tokenize(contents); const auto& it: tokens)
    {
        std::cout<<TokenTypeToString(it.Type)<<" "<<it.Literal<<std::endl;
    }
    return EXIT_SUCCESS;
}
