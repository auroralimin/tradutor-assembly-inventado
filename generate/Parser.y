%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {sb}
%define parser_class_name {Parser}
%define api.value.type variant

%code requires {
     namespace sb {
        class Driver;
        class Scanner;
    }
}

%parse-param {Scanner &scanner}
%parse-param {Driver &driver}

%code{
    #include "Driver.hpp"
    #include "OutFormat.hpp"
   
    #undef yylex
    #define yylex scanner.yylex
}

/******************************************************************************/
/*                                 TOKENS                                     */
/*----------------------------------------------------------------------------*/
%token               ADD
%token               SUB
%token               MULT
%token               DIV
%token               JMP
%token               JMPN
%token               JMPP
%token               JMPZ
%token               COPY
%token               LOAD
%token               STORE
%token               INPUT
%token               OUTPUT
%token               STOP

%token               SECTION
%token               SPACE
%token               CONST

%token <std::string> LABEL
%token <std::string> NAME
%token <int>         NUM

%token               ENDL
%token               COMMA
%token               PLUS
%token               END 0

/******************************************************************************/
/*                                  TYPES                                     */
/*----------------------------------------------------------------------------*/
%type <int>                          command
%type <int>                          instruction
%type <int>                          inst_name
%type <int>                          directive
%type <std::pair<std::string, int> > label
%type <std::pair<std::string, int> > addr

%locations

%%

one_pass
    : line     one_pass
    | line
    | end_line one_pass
    | end_line
    ;

line
    : label   end_line { driver.insertLabel($1.first, $1.second); }
    | command end_line 
    ;

label
    : LABEL end_line command { $$ = std::make_pair($1, $3); }
    | LABEL          command { $$ = std::make_pair($1, $2); }
    ;

command
    : directive   { $$ = $1; }
    | instruction { $$ = $1; }
    ;
   
instruction
    : inst_name {
          $$ = 1;
          driver.assembler($1);
      }
    | inst_name addr {
          $$ = 2;
          driver.assembler($1);
          driver.insertRef($2.first);
          driver.assembler($2.second);
      }
    | COPY addr COMMA addr {
          $$ = 3;
          driver.assembler(9);
          driver.insertRef($2.first);
          driver.assembler($2.second);
          driver.insertRef($4.first);
          driver.assembler($4.second);
      }
    ;

addr
    : NAME PLUS NUM { $$ = std::make_pair($1, $3); }
    | NAME          { $$ = std::make_pair($1,  0); }
    ;

inst_name
    : ADD     { $$ = 1;  }
    | SUB     { $$ = 2;  }
    | MULT    { $$ = 3;  }
    | DIV     { $$ = 4;  }
    | JMP     { $$ = 5;  }
    | JMPN    { $$ = 6;  }
    | JMPP    { $$ = 7;  }
    | JMPZ    { $$ = 8;  }
    | LOAD    { $$ = 10; }
    | STORE   { $$ = 11; }
    | INPUT   { $$ = 12; }
    | OUTPUT  { $$ = 13; }
    | STOP    { $$ = 14; }
    ;

directive
    : SECTION NAME {
          $$ = 0;
      }
    | SPACE {
          $$ = 1;
          driver.assembler(0);
      }
    | SPACE NUM {
          $$ = $2;
          for (int i = 0; i < $2; i++) driver.assembler(0);
      }
    | CONST NUM {
          $$ = 1;
          driver.assembler($2);
      }
    ;

end_line
    : ENDL end_line
    | ENDL
    ;

%%

void sb::Parser::error(const location_type &l, const std::string &errMsg) {
    UNUSED_VAR l;
    UNUSED_VAR errMsg;
}

