#ifndef SB_DRIVER_HPP
#define SB_DRIVER_HPP

#include <map>

#include "Scanner.hpp"
#include "Parser.hxx"

/**
 * @brief Define o escopo das implementações relacionadas ao trabalho de sb
 */
namespace sb {
    /**
     * @brief  Interface entre o Scanner e o Parser
     * Essa classe visa interfacear as classes Scanner e Parser bem como salvar
     * as informações necessárias durante a montagem.
     */
    class Driver {
        public:
            /**
             * @brief Destrutor virtual da classe
             */
            virtual ~Driver() {};

            /**
             * @brief Método que realiza a montagem de um código pré-processado
             *
             * @param srcStream é um std::istream contendo o stream carregado\
             *        com o código pré-processado
             * @param dst é um std::string contendo o nome do arquivo ao qual\
             *        o binátio montado será salvo
             */
            void onePassProcess(std::istream &srcStream, std::string dst);

            /**
             * @brief Injeção de amizade com a classe sb::Parser
             * @relates sb::Parser
             */
            friend Parser;

        private:
            /**
             * @brief Método que salva o binário montado em um arquivo
             *
             * @param dst é um std::string contendo o nome do arquivo ao qual\
             *        o binátio montado será salvo
             */
            void writeBin(std::string dst);

            /**
             * @brief Método que salva uma referência a um rótulo em um mapa
             *
             * @param label é um srd::vector contendo o nome do rótulo que foi\
             *         referenciado
             */
            void insertRef(std::string label);

            /**
             * @brief Método que salva em um vetor o valor numérico relacionado\
             *        a montagem de algum atributo terminal do código-fonte
             *
             * @param value é um int que contém o valor numérico da montagem
             */
            void assembler(int value);

            /**
             * @brief Método que resolve as referências de rótulos seguindo a\
             *        lógica de um montador de passagem única
             */
            void solveRef();

            /**
             * @brief Salva o endereço de um rótulo e informações relacionadas
             *
             * @param label é um srd::string contendo o nome do rótulo
             * @param dec é um int contendo um offset negativo para o endereço\
             *        do rótulo
             */
            void insertLabel(std::string label, int dec);

            int addr; /**<  contador de endereços do montador */ 
            std::vector<int> assembly 
            /**< contém os valores após a montagem*/;
            std::map<std::string, int> labelMap;
            /**< contém  informações a respeito de rótulos */
            std::map<std::string, std::vector<int> > refMap;
            /**< mapa de referências */
    };
}

#endif

