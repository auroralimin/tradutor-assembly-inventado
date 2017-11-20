#include "Driver.hpp"

#include <fstream>

#define DEBUG false

ldr::Driver::Driver() : acc(0) {}

void ldr::Driver::loader(std::istream &stream){
	ldr::Scanner *scanner = new ldr::Scanner(&stream);
	ldr::Parser *parser = new ldr::Parser(*scanner, *this);

	if (parser->parse()) {
		std::cerr << "Erro imprevisto no carregador" << std::endl;
		exit(EXIT_FAILURE);
	}

	simulator();
}

void ldr::Driver::simulator() {
	for (unsigned long pc = 0; pc < code.size(); pc++) {
		switch (code[pc]) {
			case 1: // ADD:
				acc += code[code[++pc]];
				break;
			case 2: //SUB:
				acc -= code[code[++pc]];
				break;
			case 3: //MULT:
				acc *= code[code[++pc]];
				break;
			case 4: //DIV:
				acc = acc/code[code[++pc]];
				break;
			case 5: //JMP:
				pc = code[++pc] - 1;
				break;
			case 6: //JMPN:
				acc < 0? pc = code[++pc] - 1 : pc++;
				break;
			case 7: //JMPP:
				acc > 0? pc = code[++pc] - 1 : pc++;
				break;
			case 8: //JMPZ:
				acc == 0? pc = code[++pc] - 1 : pc++;
				break;
			case 9: //COPY:
				code[code[pc+2]] = code[code[pc+1]], pc += 2;
				break;
			case 10: //LOAD:
				acc = code[code[++pc]];
				break;
			case 11: //STORE:
				code[code[++pc]] = acc;
				break;
			case 12: //INPUT:
				std::cin >> code[code[++pc]];
				break;
			case 13: //OUTPUT:
				std::cout << code[code[++pc]] << std::endl;
				break;
			case 14: //STOP:	
				return;
		}
	}
}

void ldr::Driver::insertCode(int n) {
	code.emplace(code.begin(), n);
}

void ldr::Driver::insertRealloc(bool b) {
	reallocInfo.emplace(reallocInfo.begin(), b);
}

void ldr::Driver::setModLength(int modLength) {
	this->modLength = modLength;
}

