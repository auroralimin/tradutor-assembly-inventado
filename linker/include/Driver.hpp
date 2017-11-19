#ifndef LNKR_DRIVER_HPP
#define LNKR_DRIVER_HPP

#include <map>
#include <string>
#include <vector>

#include "Scanner.hpp"
#include "Parser.hxx"

/**
 * @brief Define o escopo das implementações relacionadas a ligação
 */
namespace lnkr {
	/**
	 * @brief Interface entre o Scanner e o Parser
	 * Essa classe visa interfacear as classes Scanner e Parser bem como salvar
	 * as informações necessárias durante a ligação.
	 */
	class Driver {
		public:
			/**
			 * @brief Métido construtor da classe
			 */
			Driver();

			/**
			 * @brief Método destrutor virtual da classe
			 */
			virtual ~Driver() {};

			/**
			 * @brief Método que realiza a ligação de um ou mais modulos
			 *
			 * @param objs é um std::vector<std::string> contendo os\
			 * 		  nomes dos arquivos dos objetos a serem ligados
			 * @param dst é um std::string contendo o nome do arquivo ao qual\
			 *        o executavel será salvo
			 */
			void linker(std::vector<std::string> objs, std::string dst);

			/**
			 * @brief Injeção de amizade com a classe lnkr::Parser
			 * @relates lnkr::Parser
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
			 * @brief Método que escreve o executável em um arquivo
			 *
			 * @param dst é um std::string contendo o nome do arquivo ao qual\
			 *        o executável será salvo
			 */
			void writeExec(std::string dst);

			/**
			 * @brief Método que resolve as referências de rótulos seguindo a\
			 *        lógica de um montador de passagem única
			 */
			void solveRef();

			/**
			 * @brief Insere infomações de uso em uma abstração de tabela de\
			 * 				usos
			 *
			 * @param label é um std::string contendo o nome do rótulo que foi\
			 * 				ultilizado
			 * @param uses é std::vector<int> contendo os endereçõs onde o\
			 * 				rótulo foi referenciado
			 */
			void insertUse(std::string label, std::vector<int> uses);

			/**
			 * @brief Insere infomações das definições de rótulos publicos em\
			 * 				uma abstração de tabela de definições
			 *
			 * @param label é um std::string contendo o nome do rótulo que foi\
			 * 				definido
			 * @param addr é um inteiro contendo o endereço do rótulo
			 */
			void insertDef(std::string label, int addr);

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
			 *   	   sendo analisado pelo parser no momento
			 */
			void setModLength(int modLength);

			int begin; 
			/**< indica onde o código começa relativo aos outros módulos*/
			int modLength; /**< indica o tamanho do módulo*/
			std::vector<int> code; /**< contém o código ligado */
			std::vector<bool> rllcInfo;
			/**< contém as informações de realocação */
			std::map<std::string, std::vector<int> > useTable; 
			/**< contém a tabela de usos */
			std::map<std::string, int> defTable;
			/**< contém a tabela de definições */
	};
}

#endif

