/**
 * @file
 */

#include "Driver.hpp"

#include <fstream>

/**
 * @brief Função main que chama o carregador
 *
 * Espera como argumentos o nome de um executavel a ser carregado\
 * e simulado
 */
int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Deve ser informado um executavel"
			<< "a ser carregado" << std::endl;
		return EXIT_FAILURE;
	}

	//checa se o arquivo de entrada pode ser aberto e lido
	std::ifstream stream;
	stream.open(argv[1]);
	if (!stream.good()) {
		std::cerr << "Não foi possível abrir o arquivo:" << argv[1]
			<< "." << std::endl;
		return EXIT_FAILURE;
	}

	ldr::Driver driver = ldr::Driver();
	driver.loader(stream);

	stream.close();
	return EXIT_SUCCESS;
}

