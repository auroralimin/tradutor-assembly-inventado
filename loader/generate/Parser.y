%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {ldr}
%define parser_class_name {Parser}
%define api.value.type variant

%code requires {
     namespace ldr {
        class Driver;
        class Scanner;
    }
}

%parse-param {Scanner &scanner}
%parse-param {Driver &driver}

%code{
    #include "Driver.hpp"
  
	#include <vector>

    #undef yylex
    #define yylex scanner.yylex
    #define UNUSED_VAR (void)
}

/******************************************************************************/
/*                                 TOKENS                                     */
/*----------------------------------------------------------------------------*/
%token               HEADER
%token               LENGTH
%token               REALLOC
%token               TEXT

%token <std::string> NAME
%token <int>         NUM

%token               ENDL
%token               END 0

/******************************************************************************/
/*                                  TYPES                                     */
/*----------------------------------------------------------------------------*/

%locations

%%

module
	: header text
	;

header
	: mod_name length realloc
	;

text
	: TEXT text_vec end_line
	;

mod_name
	: HEADER NAME end_line
	;

length
	: HEADER NUM end_line { driver.setModLength($2); } 
	;

realloc 
	: HEADER realloc_vec end_line
	;

text_vec
	: NUM text_vec { driver.insertCode($1); }
	| NUM		   { driver.insertCode($1); }
	;

realloc_vec
	: NUM realloc_vec { driver.insertRealloc($1); }
	| NUM 			  { driver.insertRealloc($1); }
	;

end_line
    : ENDL end_line
    | ENDL
    ;

%%

void ldr::Parser::error(const location_type &l, const std::string &errMsg) {
    UNUSED_VAR l;
    UNUSED_VAR errMsg;
}

