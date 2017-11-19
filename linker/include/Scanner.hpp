#ifndef LNKR_PRE_SCANNER_HPP
#define LNKR_PRE_SCANNER_HPP

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "Parser.hxx"
#include "location.hh"

namespace lnkr {
	/**
	 * @brief Classe responisável pelo scanner do ligador
	 * Essa classe utiliza-se de recursos do Flex (por vezes redefinindo-os)
	 * para implementar a análise léxica do ligador
	 */
	class Scanner : public yyFlexLexer {
		public:
			using FlexLexer::yylex;

			/**
			 * @brief Construtor da classe
			 *
			 * @param src é um std::string contendo o nome do arquivo de entrada
			 */
			Scanner(std::istream *in) : yyFlexLexer(in) {};

			/**
			 * @brief Destrutor virtual da classe
			 */
			virtual ~Scanner() {};

			/**
			 * @brief Redefinição de yylex
			 */
			virtual int yylex(lnkr::Parser::semantic_type * const lval,
					lnkr::Parser::location_type *location);

		private:
			/**
			 * @brief Processa um token de número decimal
			 *
			 * @retval token::NUM
			 */
			int decNum(std::string str);

			/**
			 * @brief Processa um token de nome válido
			 *
			 * @retval token::NAME
			 */
			int name(std::string str);

			lnkr::Parser::semantic_type *yylval = nullptr;
			/**< salva informações do token */
	};
}

#endif

