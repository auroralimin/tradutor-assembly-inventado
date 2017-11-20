#include "Driver.hpp"

#include <fstream>

#define DEBUG false

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
	solveUses();
	writeExec(dst);
}

void lnkr::Driver::parseObj(std::string src, std::istream &srcStream) {
	lnkr::Scanner *scanner = new lnkr::Scanner(&srcStream);
	lnkr::Parser *parser = new lnkr::Parser(*scanner, *this);

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

void lnkr::Driver::solveUses() {
    for (auto uses : useTable) {
        int addr = defTable.at(uses.first);
#if DEBUG
        std::cout << "Definições: " << uses.first << " " << addr << std::endl;
        std::cout << "Usos: " << uses.first << " ";
#endif
        for (auto u : uses.second) {
#if DEBUG
            std::cout << u << " ";
#endif
            code[u].first += addr - code[u].second;
        }
#if DEBUG
        std::cout << std::endl;
#endif
    }
}

void lnkr::Driver::writeExec(std::string dst) {
	std::ofstream out;
	out.open(dst);

	out << "H: " << dst << std::endl;
	out << "H: " << begin + modLength << std::endl;
    
	out << "H: ";
	for (auto r : reallocInfo)
		out << r << " ";
	out << std::endl;

	out << "T: ";
	for (auto c : code) {
		out << c.first << " ";
	}
	out << std::endl;

	out.close();
}

void lnkr::Driver::insertUse(std::string label, std::vector<int> uses) {
	for (unsigned long i = 0; i < uses.size(); i++)
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
	if (reallocInfo[(begin + modLength) + (begin - code.size()) - 1])
		code.emplace(code.begin() + begin, std::make_pair(n + begin, begin));
	else 
		code.emplace(code.begin() + begin, std::make_pair(n, begin));
}

void lnkr::Driver::insertRealloc(bool b) {
	reallocInfo.emplace(reallocInfo.begin() + begin, b);
}

void lnkr::Driver::setModLength(int modLength) {
	this->modLength = modLength;
}

