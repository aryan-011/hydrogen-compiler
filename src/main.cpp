#include <fstream>
# include <iostream>
#include <sstream>
#include <vector>
#include "token.h"


std::vector<Token> tokenize(const std::string& str)
{
    std::vector<Token> Tokens;
    std::string buf;
    for(int i=0;i<str.size();i++)
    {
        char ch= str.at(i);
        if(isalpha(ch))
        {
            buf.push_back(ch);
            i++;
            while (isalnum(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            TokenType type = LookupIdentifier(buf);
            Tokens.emplace_back(type,type==TokenType::IDENT ?buf:TokenTypeToString(type));
            buf.clear();
        }
        else if(isdigit(ch))
        {
            buf.push_back(ch);
            i++;
            while(isdigit(str.at(i)))
            {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            Tokens.emplace_back(TokenType::INT,buf);
            buf.clear();
        }
        else if(ch=='=')
        {
            buf.push_back(ch);
            if(str.at(i+1) == '=')
            {
                buf.push_back(str.at(++i));
                Tokens.emplace_back(TokenType::EQ,buf);
                continue;
            }
            Tokens.emplace_back(TokenType::ASSIGN,buf);
            buf.clear();
        }
        else if (ch == '!')
        {
            buf.push_back(ch);
            if(str.at(i+1) == '=')
            {
                buf.push_back(str.at(++i));
                Tokens.emplace_back(TokenType::NOT_EQ,buf);
                continue;;
            }
            Tokens.emplace_back(TokenType::BANG,buf);
            buf.clear();
        }
        else if (ch == '+')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::PLUS, buf);
            buf.clear();
        }
        else if (ch == '-')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::MINUS, buf);
            buf.clear();
        }
        else if (ch == '*')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::ASTERISK, buf);
            buf.clear();
        }
        else if (ch == '/')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::SLASH, buf);
            buf.clear();
        }
        else if (ch == '<')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::LT, buf);
            buf.clear();
        }
        else if (ch == '>')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::GT, buf);
            buf.clear();
        }
        else if (ch == ',')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::COMMA, buf);
            buf.clear();
        }
        else if (ch == ';')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::SEMICOLON, buf);
            buf.clear();
        }
        else if (ch == '(')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::LPAREN, buf);
            buf.clear();
        }
        else if (ch == ')')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::RPAREN, buf);
            buf.clear();
        }
        else if (ch == '{')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::LBRACE, buf);
            buf.clear();
        }
        else if (ch == '}')
        {
            buf.push_back(ch);
            Tokens.emplace_back(TokenType::RBRACE, buf);
            buf.clear();
        }
        else if(isspace(ch))
        {
            continue;
        }
    }
    return Tokens;
}

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
