#include "Driver.hpp"

#include <fstream>

#include "OutFormat.hpp"

void sb::Driver::onePassProcess(std::istream &srcStream, std::string dst) {
    sb::Scanner *scanner = new sb::Scanner(&srcStream);
    sb::Parser *parser = new sb::Parser(*scanner, *this);

    addr = 0;
    const int accept(0);
    if (parser->parse() != accept) {
        std::cerr << "Erro imprevisto na montagem." << std::endl;
        exit(EXIT_FAILURE);
    }
   
    solveRef();
    writeBin(dst);
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

