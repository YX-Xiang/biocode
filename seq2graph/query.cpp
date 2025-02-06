#include "query.h"


Query::Query(const std::string& inputStringFile) {
    std::cerr << "+++ Load string file from " << inputStringFile << std::endl;

    std::ifstream ifs(inputStringFile);
    if(!ifs.is_open()) {
        std::cerr << "!!! Error: failed to open " << inputStringFile << std::endl;
    }
    while (!ifs.eof()) {
        std::string line;
        std::getline(ifs, line);

        if(line[0] != '>') {
            str.append(line);
        }
    }
}


Query::~Query() {}