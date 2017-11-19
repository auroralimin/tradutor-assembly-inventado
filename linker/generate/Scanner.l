%{
    #include "Scanner.hpp"

    #include "Driver.hpp"
    #include "Parser.hxx"

    using token = lnkr::Parser::token;

    #undef  YY_DECL
    #define YY_DECL int lnkr::Scanner::yylex(\
                            lnkr::Parser::semantic_type *const lval,\
                            lnkr::Parser::location_type *location)
    #define yyterminate() return( token::END )
    #define YY_NO_UNISTD_H
    #define YY_USER_ACTION location->step(); location->columns(yyleng);
%}

%option yyclass="lnkr::Scanner"
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
TD:		          { return token::DEF_TABLE; }
TU:		          { return token::USE_TABLE; }
[0-9]+[ \n\t]     { return decNum(yytext); 	 }
[a-z_]+[a-z0-9_]* { return name(yytext);   	 }
;.*   ;
[ \t] ;
.     ;
%%

int lnkr::Scanner::decNum(std::string str) {
    yyless(str.length() - 1);
    int num = std::stoi(yytext);
    yylval->build<int>(num);

    return token::NUM;
}

int lnkr::Scanner::name(std::string str) {
    yylval->build<std::string>(str);

    return token::NAME;
}

