#include <cstring>
#include <ostream>

#include "graph.h"
#include "query.h"
#include "alignment.h"


void showUsage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)>\n"
        << "Options:\n"
        << "\t-h\t\tDisplay this help message\n"
        << "\t-s <file>\tSpecify the input sequence file path\n"
        << "\t-g <file>\tSpecify the input sequence graph file path\n"
        << "\t-o <file>\tSpecify the output file path\n"
        << std::endl;
}


int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	std::cerr.tie(NULL);

    if(argc < 1) {
		showUsage(argv[0]);
		return 1;
	}

    std::string inputStringFile = "";
    std::string inputGfaFile = "";
	std::string outputPath = "";

    for (int i = 1; i < argc; ++ i) {
        if(strcmp(argv[i], "-h") == 0) {
			showUsage(argv[0]);
			return 0;
		} else if (strcmp(argv[i], "-s") == 0) {
			if (i + 1 < argc) {
				inputStringFile = argv[++i];
			} else {
				std::cerr << "-s option requires one argument." << std::endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-g") == 0) {
			if (i + 1 < argc) {
				inputGfaFile = argv[++i];
			} else {
				std::cerr << "-g option requires one argument." << std::endl;
				return 1;
			}  
		} else if (strcmp(argv[i], "-o") == 0) {
			if (i + 1 < argc) {
				outputPath = argv[++i];
			} else {
				std::cerr << "-o option requires one argument." << std::endl;
				return 1;
			}  
		}
    }

	Graph graph(inputGfaFile);
	graph.topo_sort();

	Query query(inputStringFile);

	Alignment alignment(query.str, graph);
	alignment.dp();
	std::cout << alignment.edit_distance << std::endl;
	alignment.print(outputPath);

    return 0;
}