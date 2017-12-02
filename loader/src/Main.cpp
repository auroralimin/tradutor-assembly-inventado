/**
 * @file
 */

#include <fstream>

#include "Driver.hpp"
#include "OutFormat.hpp"

bool isInt(std::string str);

/**
 * @brief Função main que chama o carregador
 *
 * Espera como argumentos o nome de um executavel a ser carrregado, a quantidade
 * de chunks disponível na memória, o tamanho de cada chunk e o endereço inicial
 * de cada chunk.
 */
int main(int argc, char** argv) {
    if ((argc < 5) || ((isInt(argv[2])) && (argc < atoi(argv[2])*2 + 3))) {
        std::cerr << ERROR_PRINT << "Invalid arguments." << std::endl
                  << ERROR_SPACE << "Expected: <executable> <number of chunks> "
                  << "[<size of each chunk>] "
                  << "[<initial address of each chunk>]" << std::endl;
        return EXIT_FAILURE;
    } 

    //checa se o arquivo de entrada pode ser aberto e lido
    std::ifstream stream;
    stream.open(argv[1]);
    if (!stream.good()) {
        std::cerr << ERROR_PRINT << "Unable to open \"" << argv[1] << "\"."
                  << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 3; i < argc; i++) {
        if (!isInt(argv[i])) {
            std::cerr << ERROR_PRINT << "Invalid arguments." << std::endl
                      << ERROR_SPACE << "All chunk related arguments should "
                      << "be integers." << std::endl;
        return EXIT_FAILURE;
        }
    }

    ldr::Driver driver = ldr::Driver();

    int numChunk = atoi(argv[2]);
    for (int i = 3; i < numChunk + 3; i++) {
        driver.insertChunk(std::make_pair(atoi(argv[i]),
                                          atoi(argv[i + numChunk])));
    }

    std::string dst = argv[1];
    dst += ".im";

    driver.loader(stream, dst);

    stream.close();
    return EXIT_SUCCESS;
}

/**
 * @brief Função que checa se uma string contém somente digitos
 *
 * @param str é um std::string com a string a ser analisada
 *
 * @retval true  < se a string contém somente dígitos
 * @retval false < se a string não contém somente dígitos
 */
bool isInt(std::string str) {
    for (auto c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

