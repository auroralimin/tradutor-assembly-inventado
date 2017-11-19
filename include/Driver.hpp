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
             * @brief Método destrutor da classe
             */
            ~Driver();

            /**
             * @brief Método que realiza a montagem de um código pré-processado
             *
             * @param srcStream é um std::istream contendo o stream carregado\
             *        com o código pré-processado
             * @param dst é um std::string contendo o nome do arquivo ao qual\
             *        o binátio montado será salvo
             * @param unique é um bool que infoma caso o arquivo sendo montado\
             * seja único ou parte de um projeto
             */
            void onePassProcess(std::istream &srcStream, bool unique,
                                std::string src, std::string dst);

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
             * @param relative é um booleano que informa se o valor montado é
             * relativo a um endereço ou a um código fixo
             */
            void assembler(int value, bool relative);

            /**
             * @brief Método que resolve as referências de rótulos seguindo a\
             *        lógica de um montador de passagem única
             */
            void solveRef();

            /**
             * @brief Método que salva o endereço de um rótulo e informações\
             * relacionadas
             *
             * @param label é um std::string contendo o nome do rótulo
             * @param dec é um int contendo um offset negativo para o endereço\
             *        do rótulo
             */
            void insertLabel(std::string label, int dec);

            /**
             * @brief Método que insere rótulo EQU
             *
             * @param label é um std::string contendo o nome do rótulo
             * @param value é um int contendo o valor do EQU
             */
            void insertEqu(std::string label, int value);

            /**
             * @brief Método que retorna o valor numérico de um EQU dado o seu\
             * rótulo
             *
             * @param label é um std::string contendo o nome do rótulo
             *
             * @retval num < em todos os casos retorna o valor do EQU\
             * (assume-se que todos os EQU foram declarados previamente,\
             * sem erros)
             */
            int getEqu(std::string label);

            /**
             * @brief Método que retorna se um módulo é único
             *
             * @retval true  < se o módulo for único
             * @retval false < se o módulo não for único
             */
            bool isUnique();

            /**
             * @brief Imprime um erro de montagem no parser
             *
             * @param msg é um std::string com o corpo da mensagem de erro
             */
            void printParseError(std::string msg);

            /**
             * @brief Insere o nome de um rótulo público em um vetor
             *
             * @param label é um std::string com o nome do rótulo declarado\
             * como público
             */
            void insertPublicLabel(std::string label);

            int addr;    /**< contador de endereços do montador */ 
            bool unique; /**< informa se o módulo é único ou não */ 
            std::string src; /**< nome do arquivo fonte */ 
            std::vector<int> assembly 
                /**< contém os valores após a montagem*/;
            std::vector<int> realocInfo 
                /**< contém as informações de realocação*/;
            std::map<std::string, int> equMap;
            /**< contém os valores equ relacionados aos seus rótulos*/;
            std::map<std::string, int> labelMap;
            /**< contém informações a respeito de rótulos */
            std::vector<std::string> publicLabel;
            /**< contém os nomes dos rótulos que são públicos */
            std::map<std::string, std::vector<int> > refMap;
            /**< mapa de referências */
    };
}

#endif

