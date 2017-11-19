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

    out << "H: " << dst.substr(0, dst.find(".")) << std::endl;
    out << "H: " << addr << std::endl;

    out << "H: ";
    for (auto r : realocInfo) {
        out << r << " ";
    }
    out << std::endl;

    out << "T: ";
    for (auto n : assembly) {
        out << n << " ";
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

void sb::Driver::assembler(int value, bool relative) {
    assembly.push_back(value);
    realocInfo.push_back(relative);
    addr++;
}

void sb::Driver::solveRef() {
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

void sb::Driver::insertPublicLabel(std::string label) {
    publicLabel.emplace_back(label);
}

