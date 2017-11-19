%{
    #include "Scanner.hpp"

    #include "Driver.hpp"
    #include "Parser.hxx"

    using token = asblr::Parser::token;

    #undef  YY_DECL
    #define YY_DECL int asblr::Scanner::yylex(\
                            asblr::Parser::semantic_type *const lval,\
                            asblr::Parser::location_type *location)
    #define yyterminate() return( token::END )
    #define YY_NO_UNISTD_H
    #define YY_USER_ACTION location->step(); location->columns(yyleng);
%}

%option yyclass="asblr::Scanner"
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
\n                  { return token::ENDL;    }
,                   { return token::COMMA;   }
\+[ \t]             { return token::PLUS;    }
ADD                 { return token::ADD;     }
SUB                 { return token::SUB;     }
MULT                { return token::MULT;    }
DIV                 { return token::DIV;     }
JMP                 { return token::JMP;     }
JMPP                { return token::JMPP;    }
JMPN                { return token::JMPN;    }
JMPZ                { return token::JMPZ;    }
COPY                { return token::COPY;    }
LOAD                { return token::LOAD;    }
STORE               { return token::STORE;   }
INPUT               { return token::INPUT;   }
OUTPUT              { return token::OUTPUT;  }
STOP                { return token::STOP;    }
SECTION             { return token::SECTION; }
SPACE               { return token::SPACE;   }
CONST               { return token::CONST;   }
EQU                 { return token::EQU;     }
IF                  { return token::IF;      }
BEGIN               { return token::BEGINC;  }
END                 { return token::ENDC;    }
EXTERN              { return token::EXTERN;  }
PUBLIC              { return token::PUBLIC;  }
[+-]*0x[0-9a-fA-F]+ { return hexNum(yytext); }
[+-]*[0-9]+         { return decNum(yytext); }
[a-z_]+[a-z0-9_]*:  { return label(yytext);  }
[a-z_]+[a-z0-9_]*   { return name(yytext);   }
;.*   ;
[ \t] ;
.     ;
%%

int asblr::Scanner::decNum(std::string str) {
    int num = std::stoi(str);
    yylval->build<int>(num);

    return token::NUM;
}

int asblr::Scanner::hexNum(std::string str) {
    int num = std::stoi(str, 0, 16);
    yylval->build<int>(num);

    return token::NUM;
}

int asblr::Scanner::label(std::string str) {
    str = str.substr(0, str.length() - 1);
    yylval->build<std::string>(str);

    return token::LABEL;
}

int asblr::Scanner::name(std::string str) {
    yylval->build<std::string>(str);

    return token::NAME;
}

