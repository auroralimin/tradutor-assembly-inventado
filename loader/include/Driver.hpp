#ifndef LDR_DRIVER_HPP
#define LDR_DRIVER_HPP

#include <map>
#include <utility>
#include <vector>
#include <queue>

#include "Scanner.hpp"
#include "Parser.hxx"

/**
 * @brief Define o escopo das implementações relacionadas ao carregamento
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
         * @param dst nome do arquivo que vai ser gravado a saída
         */
        void loader(std::istream &stream, std::string dst);
        
        /**
         * @brief Injeção de amizade com a classe ldr::Parser
         * @relates ldr::Parser
         */
        friend Parser;
        
        /**
         * @brief Método que insere uma tupla na abstração do valor de chunk
         * @param chunk é um std::pair<int,int> contendo o tamanho do chunk\
         * e o endereço do primeiro elemento
         */
        void insertChunk(std::pair<int,int> chunk);
        
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
         * @brief Insere um elemento do código ligado à uma abstração
         *
         * @param n é um inteiro contendo a parte do código a ser\
         *        ligado
         */
        void insertCode(int n);
        
        /**
         * @brief Insere uma informação de realocação
         *
         * @param b é boleano que informa se a informação é relativa ou não
         */
        void insertRealloc(bool b);
        
        /**
         * @brief Seta o tamanho do código do módulo passando pelo parser
         *
         * @param modLength é um inteiro que informa o tamanho do módulo
         */
        void setModLength(int modLength);
        
        /**
         * @brief Realoca os endereços de memória relativos de acordo com\
         *        o chunk
         */
        void reallocCode();
        
        /**
         * @brief Escreve o código com os endereços de memória\
         *        realocados em um arquivo
         * @param dst é ums std::string com o nome do arquivo de saída
         */
        void writeOutput(std::string dst);
        
        /**
         * @brief Auxilia na ordenação da abstração de chunk
         * @param x é um std::pair<int, int> contendo o primeiro elemento
         * @param y é um std::pair<int, int> contendo o segundo elemento
         */
        static bool compareChunk(std::pair<int,int> &x, std::pair<int,int> &y);
        
        int acc; /**< acumulador usado na simulação*/
        int modLength; /**< indica o tamanho do módulo*/
        std::vector<int> code;/**< contém o código carregado*/
        std::vector<bool> reallocInfo;/**< contém as informações de realocação*/
        std::vector<std::pair<int, int>> chunkInfo; /**< informação de chunk*/
    };
}

#endif

