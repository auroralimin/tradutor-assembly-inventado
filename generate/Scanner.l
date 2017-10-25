%{
    #include "Scanner.hpp"

    #include "Driver.hpp"
    #include "Parser.hxx"

    using token = sb::Parser::token;
    int yycolumn = 1;

    #undef  YY_DECL
    #define YY_DECL int sb::Scanner::yylex(\
                            sb::Parser::semantic_type *const lval,\
                            sb::Parser::location_type *location)
    #define yyterminate()   static int once = 0;\
                            return once++ ? token::ENDL : token::END;
    #define YY_NO_UNISTD_H
    #define YY_USER_ACTION location->step(); location->columns(yyleng);
%}

%option yyclass="sb::Scanner"
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
\n                          { return token::ENDL;    }
,                           { return token::COMMA;   }
\+[ \t]                     { return token::PLUS;    }
ADD                         { return token::ADD;     }
SUB                         { return token::SUB;     }
MULT                        { return token::MULT;    }
DIV                         { return token::DIV;     }
JMP                         { return token::JMP;     }
JMPP                        { return token::JMPP;    }
JMPN                        { return token::JMPN;    }
JMPZ                        { return token::JMPZ;    }
COPY                        { return token::COPY;    }
LOAD                        { return token::LOAD;    }
STORE                       { return token::STORE;   }
INPUT                       { return token::INPUT;   }
OUTPUT                      { return token::OUTPUT;  }
STOP                        { return token::STOP;    }
SECTION                     { return token::SECTION; }
SPACE                       { return token::SPACE;   }
CONST                       { return token::CONST;   }
[+-]*0x[0-9a-fA-F]+[ \n\t,] { return hexNum(yytext); }
[+-]*[0-9]+[ \n\t,]         { return decNum(yytext); }
[a-z_]+[a-z0-9_]*:          { return label(yytext);  }
[a-z_]+[a-z0-9_]*           { return name(yytext);   }
;.*   ;
[ \t] ;
.     ;
%%

int sb::Scanner::decNum(std::string str) {
    yyless(str.length() - 1);
    int num = std::stoi(yytext);
    yylval->build<int>(num);

    return token::NUM;
}

int sb::Scanner::hexNum(std::string str) {
    yyless(str.length() - 1);
    int num = std::stoi(yytext, 0, 16);
    yylval->build<int>(num);

    return token::NUM;
}

int sb::Scanner::label(std::string str) {
    str = str.substr(0, str.length() - 1);
    yylval->build<std::string>(str);

    return token::LABEL;
}

int sb::Scanner::name(std::string str) {
    yylval->build<std::string>(str);

    return token::NAME;
}

