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
	const std::string red = COLOR(color::red);

    if (argc < 2) {
        std::cerr << ERROR_PRINT << "Invalid arguments." << std::endl
                  << ERROR_SPACE << "Expected: [<file name>.asm]" << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        asblr::Driver *driver = new asblr::Driver();
        std::string src, asmStr = ".asm";
        src = argv[i];

        //confere se é um arquivo asm de entrada
        if (src.rfind(asmStr) != (src.size() - asmStr.size())) {
            std::cerr << ERROR_PRINT 
                      << "Argument files should be \".asm\"." << std::endl;
            return EXIT_FAILURE;
        }

        //retira o caminho e substitui a extensão do arquivo de destino para .o
        std::string dst = src;
        while (dst.find("/") != std::string::npos) {
            dst = dst.substr(dst.find("/") + 1, dst.size());
        }
        dst = dst.substr(0, dst.find(".")) + ".o";

        //checa se o arquivo de entrada pode ser aberto e lido
        std::ifstream stream;
        stream.open(src);
        if (!stream.good()) {
            std::cerr << ERROR_PRINT << "Unable to open \"" << src
                      << "\"." << std::endl;
            return EXIT_FAILURE;
        }

        //monta arquivo
        driver->onePassProcess(stream, argc == 2, src, dst);

        stream.close();
        delete driver;
    }

    return EXIT_SUCCESS;
}

