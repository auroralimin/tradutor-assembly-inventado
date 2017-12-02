/**
 * @file
 */

#include "Driver.hpp"
#include "OutFormat.hpp"

/**
 * @brief Função main que chama o ligador
 *
 * Espera como argumentos o nome de um ou mais obejotos a serem\
 * ligados
 */
int main(int argc, char** argv) {
	if (argc < 2) {
        std::cerr << ERROR_PRINT << "Invalid arguments." << std::endl
                  << ERROR_SPACE << "Expected: [<object name>.o]" << std::endl;
		return EXIT_FAILURE;
	}
	
	std::vector<std::string> objs;
	for (int i = 1; i < argc; i++)
		objs.emplace_back(argv[i]);

	std::string exec(argv[1]);
	exec = exec.substr(0, exec.find('.'));

	lnkr::Driver driver = lnkr::Driver();
	driver.linker(objs, exec);

	return EXIT_SUCCESS;
}

