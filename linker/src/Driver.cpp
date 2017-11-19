#include "Driver.hpp"

#include <fstream>

lnkr::Driver::Driver() : begin(0), modLength(0) {}

void lnkr::Driver::linker(std::vector<std::string> objs,
				       	  std::string dst) {
	for (auto src : objs) {
		std::ifstream stream;
		stream.open(src);
		if (!stream.good()) {
			std::cerr << "Não foi possível abrir o arquivo:" << src
				<< "." << std::endl;
			exit(EXIT_FAILURE);
		}
		parseObj(src, stream);
		stream.close();
	}
	//TODO: solveUses();
	writeExec(dst);
}

void lnkr::Driver::parseObj(std::string src, std::istream &srcStream) {
	lnkr::Scanner *scanner = new lnkr::Scanner(&srcStream);
	lnkr::Parser *parser = new lnkr::Parser(*scanner, *this);

	std::cout << src << " len: " << modLength << std::endl;
	begin += modLength;
	const int accept = 0;
	if (parser->parse() != accept) {
		std::cerr << "Erro imprevisto no parse do objeto "
		     	  << src << "." << std::endl;
		exit(EXIT_FAILURE);
	}

	delete parser;
	delete scanner;
}

void lnkr::Driver::writeExec(std::string dst) {
	std::cout << "realloc info:" << std::endl;
	for (auto r : rllcInfo)
		std::cout << r << " ";
	std::cout << std::endl;

	std::cout << "Código ligado sem corrigir usos:" << std::endl;
	for (auto c : code) {
		std::cout << c << "  ";
	}
	std::cout << std::endl;
}

void lnkr::Driver::insertUse(std::string label, std::vector<int> uses) {
	for (int i; i < uses.size(); i++)
		uses[i] += begin;

	useTable[label] = uses;
}

void lnkr::Driver::insertDef(std::string label, int addr) {
	defTable[label] = addr + begin;
}

void lnkr::Driver::insertCode(int n) {
	//Como é resolido de baixo para cima na arvore de derivação
	//é precisso percorrer a informação de realocação de tras 
	//para frente para cada modulo, dessa forma:
	// begin + modLength : tamanho da informação de realocação em 
	// 					   cada modulo
	// begin - code.size : percorrer de tras pra frente em cada 
	// 					   modulo  
	if (rllcInfo[(begin + modLength) + (begin - code.size()) - 1])
		code.emplace(code.begin() + begin, n + begin);
	else 
		code.emplace(code.begin() + begin, n);
}

void lnkr::Driver::insertRealloc(bool b) {
	rllcInfo.emplace(rllcInfo.begin() + begin, b);
}

void lnkr::Driver::setModLength(int modLength) {
	this->modLength = modLength;
}

