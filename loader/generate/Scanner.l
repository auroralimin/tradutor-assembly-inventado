%{
    #include "Scanner.hpp"

    #include "Driver.hpp"
    #include "Parser.hxx"

    using token = ldr::Parser::token;

    #undef  YY_DECL
    #define YY_DECL int ldr::Scanner::yylex(\
                            ldr::Parser::semantic_type *const lval,\
                            ldr::Parser::location_type *location)
    #define yyterminate() return( token::END )
    #define YY_NO_UNISTD_H
    #define YY_USER_ACTION location->step(); location->columns(yyleng);
%}

%option yyclass="ldr::Scanner"
%option debug
%option yylineno
%option nodefault
%option noyywrap
%option c++
%option caseless

%%
%{
    yylval = lval;
%}
\n                { return token::ENDL;      }
H:		          { return token::HEADER;    }
T:		          { return token::TEXT;	     }
[0-9]+[ \n\t]     { return decNum(yytext); 	 }
[a-z_]+[a-z0-9_]* { return name(yytext);   	 }
;.*   ;
[ \t] ;
.     ;
%%

int ldr::Scanner::decNum(std::string str) {
    yyless(str.length() - 1);
    int num = std::stoi(yytext);
    yylval->build<int>(num);

    return token::NUM;
}

int ldr::Scanner::name(std::string str) {
    yylval->build<std::string>(str);

    return token::NAME;
}

