#include "Driver.hpp"

#include <fstream>

#include "OutFormat.hpp"

#define DRIVER_DEBUG false

asblr::Driver::~Driver() {
	addr = 0;
	assembly.clear();
	equMap.clear();
	labelMap.clear();
	refMap.clear();
}

void asblr::Driver::onePassProcess(std::istream &srcStream, bool unique,
								   std::string src, std::string dst) {
	asblr::Scanner *scanner = new asblr::Scanner(&srcStream);
	asblr::Parser *parser = new asblr::Parser(*scanner, *this);

	addr = 0;
	this->unique = unique;
	this->src = src;
	const int accept = 0;
	if (parser->parse() != accept) {
		std::cerr << ERROR_PRINT << "Unexpected assembler error." << std::endl
                  << ERROR_SPACE << "This assembler program only handles "
                  << "BEGIN/END directives related errors." << std::endl;
		exit(EXIT_FAILURE);
	}

	solveRef();
	writeBin(dst);

	delete parser;
	delete scanner;
}

void asblr::Driver::writeBin(std::string dst) {
	std::ofstream out;
	out.open(dst);

	out << "H: " << dst.substr(0, dst.find(".")) << std::endl;
	out << "H: " << addr << std::endl;

	out << "H: ";
	for (auto r : realocInfo) {
		out << r << " ";
	}
	out << std::endl;

	for (auto r : refMap) {
		out << "TU: " << r.first << " ";
		for (auto n : r.second) {
			out << n << " ";
		}    
		out << std::endl;
	}

	for (auto p : publicLabel) {
		out << "TD: " << p << " " << labelMap.at(p) << std::endl;
	}

	out << "T: ";
	for (auto n : assembly) {
		out << n << " ";
	}
	out << std::endl;

	out.close();
}

void asblr::Driver::insertRef(std::string label) {
#if DRIVER_DEBUG
    const std::string cyan = COLOR(asblr::color::cyan);
    std::cout << cyan << "Driver: " << OFF;
    std::cout << "Insert reference: " << label << " " << addr << std::endl;
#endif
	refMap[label].push_back(addr);
}

void asblr::Driver::assembler(int value, bool relative) {
	assembly.push_back(value);
	realocInfo.push_back(relative);
	addr++;
}

void asblr::Driver::solveRef() {
	std::map<std::string, std::vector<int> >::iterator it1;

	for (it1 = refMap.begin(); it1 != refMap.end(); /*faz nada*/) {
		std::map<std::string, int>::iterator it2 = labelMap.find(it1->first);
		if (it2 != labelMap.end()) {
			for (auto n : it1->second) {
				assembly[n] = it2->second + assembly.at(n);
			}
			refMap.erase(it1++);
		} else {
			it1++;
		}
	}
}

void asblr::Driver::insertLabel(std::string label, int dec) {
#if DRIVER_DEBUG
    const std::string cyan = COLOR(asblr::color::cyan);
    std::cout << cyan << "Driver: " << OFF;
    std::cout << "Insert label: " << label << " " << addr-dec << std::endl;
#endif
	labelMap[label] = addr - dec;
}

void asblr::Driver::insertEqu(std::string label, int value) {
	equMap[label] = value;
}

int asblr::Driver::getEqu(std::string label) {
	std::map<std::string, int>::iterator it = equMap.find(label);
	return it->second;
}

bool asblr::Driver::isUnique() {
	return unique;
}

void asblr::Driver::printParseError(std::string msg) {
	std::cerr << ERROR_PRINT << BOLD << src << ": " << OFF
	          << msg << std::endl;
}

void asblr::Driver::insertPublicLabel(std::string label) {
	publicLabel.emplace_back(label);
}

