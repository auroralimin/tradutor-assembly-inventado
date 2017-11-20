#ifndef LDR_DRIVER_HPP
#define LDR_DRIVER_HPP

#include <map>
#include <utility>
#include <vector>

#include "Scanner.hpp"
#include "Parser.hxx"

/**
 * @brief Define o escopo das implementações relacionadas a ligação
 */
namespace ldr {
	/**
	 * @brief Interface entre o Scanner e o Parser
	 * Essa classe visa interfacear as classes Scanner e Parser bem como salvar
	 * as informações necessárias durante o carregamento.
	 */
	class Driver {
		public:
			/**
			 * @brief Método construtor da classe
			 */
			Driver();

			/**
			 * @brief Método destrutor virtual da classe
			 */
			virtual ~Driver() {};

			/**
			 * @brief Método que faz a simulação do executavel
			 */
			void simulator();

			/**
			 * @brief Método que realiza a simulação e o carregamento
			 *
			 * @param stream é um std::istream contendo o stream carregado
			 */
			void loader(std::istream &stream);

			/**
			 * @brief Injeção de amizade com a classe ldr::Parser
			 * @relates ldr::Parser
			 */
			friend Parser;

		private:
			/**
			 * @brief Faz o parser de um arquivo objeto
			 *
			 * @param src é um std::string com o nome do arquivo objeto
			 * @param srcStream é um std::istream com o stream do arquivo\
			 *        objeto
			 */
			void parseObj(std::string src, std::istream &srcStream);
        
			/**
			 *	@brief Insere um elemento do código ligado à uma abstração
			 *
			 *	@param n é um inteiro contendo a parte do código a ser\
			 *				 ligado
			 */
			void insertCode(int n);

			/**
			 *	@brief Insere uma informação de realocação
			 *
			 *	@param b é boleano que informa se a informação é relativa ou não
			 */
			void insertRealloc(bool b);

			/**
			 *	@brief Seta o tamanho do código do módulo passando pelo parser
			 *
			 *	@param modLength é um inteiro que informa o tamanho do módulo
			 */
			void setModLength(int modLength);
			
			int acc; /**< acumulador usado na simulação*/
			int modLength; /**< indica o tamanho do módulo*/
			std::vector<int> code;/**< contém o código carregado */
			std::vector<bool> reallocInfo;
			/**< contém as informações de realocação */
	};
}

#endif

