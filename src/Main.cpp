/**
 * @file
 */

#include <fstream>

#include "Driver.hpp"
#include "OutFormat.hpp"

/**
 * @brief Função main que chama o montador de passagem única
 *
 * Espera como argumentos o nome do arquivo de entrada e o nome do arquivo de
 * saída
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "É necessário especificar: [<programa>.asm]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string src(argv[1]), asmStr = ".asm";
    if (src.rfind(asmStr) != (src.size() - asmStr.size())) {
        std::cerr << "É necessário que o programa de entrada "
                  << "tenha a extensão .asm" << std::endl;
        return EXIT_FAILURE;
    }

    std::string dst = src;
    while (dst.find("/") != std::string::npos) {
        dst = dst.substr(dst.find("/") + 1, dst.size());
    }
    dst = dst.substr(0, dst.find(".")) + ".o";

    std::ifstream stream(src);
    if (!stream.good()) {
        std::cerr << "Não foi possível abrir o arquivo:" << src
                  << "." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (DEBUG) {
        std::cout << "OnePass src = " << src << ", OnePass dst = "
                  << dst << std::endl << std::endl;
    }
    sb::Driver *driver = new sb::Driver();
    driver->onePassProcess(stream, dst);

    stream.close();
    delete driver;

    return EXIT_SUCCESS;
}

