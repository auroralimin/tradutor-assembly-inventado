#include "Driver.hpp"

#include <fstream>

#include "OutFormat.hpp"

sb::Driver::~Driver() {
    addr = 0;
    assembly.clear();
    equMap.clear();
    labelMap.clear();
    refMap.clear();
}

void sb::Driver::onePassProcess(std::istream &srcStream, bool unique,
                                std::string src, std::string dst) {
    sb::Scanner *scanner = new sb::Scanner(&srcStream);
    sb::Parser *parser = new sb::Parser(*scanner, *this);

    addr = 0;
    this->unique = unique;
    this->src = src;
    const int accept = 0;
    if (parser->parse() != accept) {
        std::cerr << "Erro imprevisto na montagem." << std::endl;
        exit(EXIT_FAILURE);
    }
   
    solveRef();
    writeBin(dst);

    delete parser;
    delete scanner;
}

void sb::Driver::writeBin(std::string dst) {
    std::ofstream out;
    out.open(dst);
    for (auto n : assembly) {
        out << n << " ";
    }
    out << std::endl;
    out.close();
}

void sb::Driver::insertRef(std::string label) {
    if (DEBUG) {
        const std::string cyan = COLOR(sb::color::cyan);
        std::cout << cyan << "Driver: " << OFF;
        std::cout << "Insere Ref: " << label << " " << addr << std::endl;
    }
    refMap[label].push_back(addr);
}

void sb::Driver::assembler(int value) {
    assembly.push_back(value);
    addr++;
}

void sb::Driver::solveRef() {
    for (auto ref : refMap) {
        std::map<std::string, int>::iterator it = labelMap.find(ref.first);
        if (it != labelMap.end()) {
            for (auto n : ref.second) {
                assembly[n] = it->second + assembly.at(n);
            }
        }
    }
}

void sb::Driver::insertLabel(std::string label, int dec) {
    if (DEBUG) {
        const std::string cyan = COLOR(sb::color::cyan);
        std::cout << cyan << "Driver: " << OFF;
        std::cout << "Insere Label: " << label << " " << addr-dec << std::endl;
    }
    labelMap[label] = addr - dec;
}

void sb::Driver::insertEqu(std::string label, int value) {
    equMap[label] = value;
}

int sb::Driver::getEqu(std::string label) {
    std::map<std::string, int>::iterator it = equMap.find(label);
    return it->second;
}

bool sb::Driver::isUnique() {
    return unique;
}

void sb::Driver::printParseError(std::string msg) {
    const std::string red = COLOR(sb::color::red);
    std::cout << red << "Erro: " << OFF;
    std::cout << BOLD << src << ": " << OFF;
    std::cout << msg << std::endl;
}

