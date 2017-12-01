#include "Driver.hpp"

#include <fstream>

#define DEBUG false

ldr::Driver::Driver() : acc(0) {}

void ldr::Driver::loader(std::istream &stream, std::string dst){
    ldr::Scanner *scanner = new ldr::Scanner(&stream);
    ldr::Parser *parser = new ldr::Parser(*scanner, *this);
    
    if (parser->parse()) {
        std::cerr << "Erro imprevisto no carregador" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    simulator();
    reallocCode();
    writeOutput(dst);
}

void ldr::Driver::insertChunk(std::pair<int,int> chunk) {
    chunkInfo.push_back(chunk);
}

bool ldr::Driver::compareChunk(std::pair<int,int> &x, std::pair<int,int> &y) {
    return x.first > y.first;
}

void ldr::Driver::reallocCode() {
    
    std::sort (chunkInfo.begin(), chunkInfo.end(), compareChunk);
    std::vector<int>::iterator c_it;
    std::vector<bool>::iterator r_it;
    
    for (c_it = code.begin(), r_it = reallocInfo.begin();
         c_it != code.end(); c_it++, r_it++) {
        
        if (*r_it == false) {
            continue;
        }
        
        int acc = 0;
        
        std::vector<std::pair<int,int>>::iterator chunk;
        
        for (chunk = chunkInfo.begin(); chunk != chunkInfo.end(); chunk++) {
            if (*c_it < chunk->first + acc) {
                *c_it += chunk->second - acc;
                break;
            }else {
                acc += chunk->first;
            }
        }
        
        if (chunk == chunkInfo.end()) {
            std::cout << "OUT OF MEMORY - YOUR PROGRAM WILL NOT BE LOADED" << std::endl;
            exit(EXIT_FAILURE);
        }
        
    }
}

void ldr::Driver::writeOutput(std::string dst) {
    
    std::ofstream out;
    out.open(dst);
    
    for (int c : code) {
        out << c << " ";
    }
    
    out.close();
}

void ldr::Driver::simulator() {
    
    std::vector<int> codeAux(code);
    
    for (unsigned long pc = 0; pc < codeAux.size(); pc++) {
        switch (codeAux[pc]) {
            case 1: // ADD:
                acc += codeAux[codeAux[++pc]];
                break;
            case 2: //SUB:
                acc -= codeAux[codeAux[++pc]];
                break;
            case 3: //MULT:
                acc *= codeAux[codeAux[++pc]];
                break;
            case 4: //DIV:
                acc = acc/codeAux[codeAux[++pc]];
                break;
            case 5: //JMP:
                pc = codeAux[++pc] - 1;
                break;
            case 6: //JMPN:
                acc < 0? pc = codeAux[++pc] - 1 : pc++;
                break;
            case 7: //JMPP:
                acc > 0? pc = codeAux[++pc] - 1 : pc++;
                break;
            case 8: //JMPZ:
                acc == 0? pc = codeAux[++pc] - 1 : pc++;
                break;
            case 9: //COPY:
                codeAux[codeAux[pc+2]] = codeAux[codeAux[pc+1]], pc += 2;
                break;
            case 10: //LOAD:
                acc = codeAux[codeAux[++pc]];
                break;
            case 11: //STORE:
                codeAux[codeAux[++pc]] = acc;
                break;
            case 12: //INPUT:
                std::cin >> codeAux[codeAux[++pc]];
                break;
            case 13: //OUTPUT:
                std::cout << codeAux[codeAux[++pc]] << std::endl;
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
    reallocInfo.insert(reallocInfo.begin(), b);
}

void ldr::Driver::setModLength(int modLength) {
    this->modLength = modLength;
}

