%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {lnkr}
%define parser_class_name {Parser}
%define api.value.type variant

%code requires {
     namespace lnkr {
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
%token               DEF_TABLE
%token               USE_TABLE

%token <std::string> NAME
%token <int>         NUM

%token               ENDL
%token               END 0

/******************************************************************************/
/*                                  TYPES                                     */
/*----------------------------------------------------------------------------*/
%type <std::vector<int> >	use_vec

%locations

%%

module
	: header tables text
	| header text
	;

header
	: mod_name length realloc
	;

tables
	: use_table tables 
	| def_table tables
	| use_table
	| def_table
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

use_table 
	: USE_TABLE NAME use_vec end_line { driver.insertUse($2,$3); }
	;

def_table
	: DEF_TABLE NAME NUM end_line { driver.insertDef($2,$3); }	
	;

text_vec
	: NUM text_vec { driver.insertCode($1); }
	| NUM		   { driver.insertCode($1); }
	;

realloc_vec
	: NUM realloc_vec { driver.insertRealloc($1); }
	| NUM 			  { driver.insertRealloc($1); }
	;

use_vec
	: NUM use_vec {
	      $2.emplace($2.begin(), $1);
		  $$ = $2;
      }
	| NUM { 
          $$.emplace($$.begin(), $1);
      }
	;

end_line
    : ENDL end_line
    | ENDL
    ;

%%

void lnkr::Parser::error(const location_type &l, const std::string &errMsg) {
    UNUSED_VAR l;
    UNUSED_VAR errMsg;
}

