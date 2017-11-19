%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {asblr}
%define parser_class_name {Parser}
%define api.value.type variant

%code requires {
     namespace asblr {
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

    bool validLine = true;
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
%token               EQU
%token               IF
%token               BEGINC
%token               ENDC
%token               EXTERN
%token               PUBLIC

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

module
    : begin_code code end
    | end_line begin_code code end
    | code {
           if (!driver.isUnique()) {
              driver.printParseError("Se 2 ou 3 programas sao definidos pelo "\
                                     "usuário como entrada entao as diretivas "\
                                     "BEGIN e END são obrigatorias.");
             exit(EXIT_FAILURE);
          }
      }
    ;

begin_code
    : LABEL BEGINC end_line {
          if (driver.isUnique()) {
              driver.printParseError("Se um ́unico programa foi colocado o "\
                                     "mesmo NÃO deve ter as diretivas "\
                                     "BEGIN e END.");
             exit(EXIT_FAILURE);
          }
      }
    ;

end
    : ENDC
    | ENDC end_line
    ;

code
    : line     code
    | line
    | end_line code
    | end_line
    ;

line
    : equ end_line
    | if end_line
    | extern end_line
    | public end_line
    | label   end_line { driver.insertLabel($1.first, $1.second); }
    | command end_line 
    ;

equ
    : LABEL EQU NUM          { driver.insertEqu($1, $3); }
    | LABEL end_line EQU NUM { driver.insertEqu($1, $4); }
    ;

if
    : IF NAME {
          if (driver.getEqu($2) != 1) {
              validLine = false;
          }
      }
    ;

extern
    : LABEL EXTERN
    ;

public
    : PUBLIC NAME {
          driver.insertPublicLabel($2);
      }
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
          if (validLine) {
              $$ = 1;
              driver.assembler($1, false);
          } else {
              validLine = true;
          }
      }
    | inst_name addr {
          if (validLine) {
              $$ = 2;
              driver.assembler($1, false);
              driver.insertRef($2.first);
              driver.assembler($2.second, true);
          } else {
              validLine = true;
          }
      }
    | COPY addr COMMA addr {
          if (validLine) {
              $$ = 3;
              driver.assembler(9, false);
              driver.insertRef($2.first);
              driver.assembler($2.second, true);
              driver.insertRef($4.first);
              driver.assembler($4.second, true);
          } else {
              validLine = true;
          }
      }
    ;

addr
    : NAME PLUS NUM  { $$ = std::make_pair($1, $3); }
    | NAME PLUS NAME { $$ = std::make_pair($1, driver.getEqu($3)); }
    | NAME           { $$ = std::make_pair($1,  0); }
    ;

inst_name
    : ADD    { $$ = 1;  }
    | SUB    { $$ = 2;  }
    | MULT   { $$ = 3;  }
    | DIV    { $$ = 4;  }
    | JMP    { $$ = 5;  }
    | JMPN   { $$ = 6;  }
    | JMPP   { $$ = 7;  }
    | JMPZ   { $$ = 8;  }
    | LOAD   { $$ = 10; }
    | STORE  { $$ = 11; }
    | INPUT  { $$ = 12; }
		| OUTPUT { $$ = 13; }
    | STOP   { $$ = 14; }
    ;

directive
    : SECTION NAME {
          $$ = 0;
      }
    | SPACE {
          $$ = 1;
          driver.assembler(0, true);
      }
    | SPACE NUM {
          $$ = $2;
          for (int i = 0; i < $2; i++) driver.assembler(0, true);
      }
    | CONST NUM {
          $$ = 1;
          driver.assembler($2, true);
      }
    ;

end_line
    : ENDL end_line
    | ENDL
    ;

%%

void asblr::Parser::error(const location_type &l, const std::string &errMsg) {
    UNUSED_VAR l;
    UNUSED_VAR errMsg;
}

