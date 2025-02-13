#include "graph.h"


Graph::Graph(const std::string& inputGfaFile) {
    std::cerr << "+++ Load gfa file from " << inputGfaFile << std::endl;

    int baseID = 0;
    totalBases = 0;
    topoOrder = std::vector <int> ();

    std::ifstream ifs0(inputGfaFile);
    if(!ifs0.is_open()) {
        std::cerr << "!!! Error: failed to open " << inputGfaFile << std::endl;
    }
    while (!ifs0.eof()) {
        std::string line;
        std::getline(ifs0, line);

        if (line.empty() || line[0] != 'S') continue;

        std::istringstream iss(line);
        std::vector <std::string> tokens;
        std::string token;

        while (std::getline(iss, token, '\t')) {
            tokens.push_back(token);
        }

        totalBases += tokens[2].size();
    }
    ifs0.close();

    father = std::vector <std::vector <int> > (totalBases, std::vector <int>());
    edge = std::vector <std::vector <int> > (totalBases, std::vector <int>());
    baseVec = std::vector <char> (totalBases);
    inDegree = std::vector <int> (totalBases);
    // generation = std::vector <int> (totalBases, totalBases);
    std::map <std::string, std::pair<int, int> > segmentBE;

    std::ifstream ifs1(inputGfaFile);
    if(!ifs1.is_open()) {
        std::cerr << "!!! Error: failed to open " << inputGfaFile << std::endl;
    }
    while (!ifs1.eof()) {
        std::string line;
        std::getline(ifs1, line);

        if (line[0] == 'S') {
            std::istringstream iss(line);
            std::vector <std::string> tokens;
            std::string token;
            int BID = baseID;

            while (std::getline(iss, token, '\t')) {
                tokens.push_back(token);
            }

            for (auto s: tokens[2]) {
                baseVec[baseID] = s;
                if (baseID != BID) {
                    father[baseID].push_back(baseID - 1);
                    edge[baseID - 1].push_back(baseID);
                    inDegree[baseID] ++;
                }
                baseID ++;
            }

            segmentBE[tokens[1]] = {BID, baseID - 1};
        }
    }
    ifs1.close();

    std::ifstream ifs2(inputGfaFile);
    if(!ifs2.is_open()) {
        std::cerr << "!!! Error: failed to open " << inputGfaFile << std::endl;
    }
    while (!ifs2.eof()) {
        std::string line;
        std::getline(ifs2, line);

        if (line[0] == 'L') {
            std::istringstream iss(line);
            std::vector <std::string> tokens;
            std::string token;

            while (std::getline(iss, token, '\t')) {
                tokens.push_back(token);
            }

            if (tokens[2] == "+") {
                if (tokens[4] == "+") {
                    father[segmentBE[tokens[3]].first].push_back(segmentBE[tokens[1]].second);
                    edge[segmentBE[tokens[1]].second].push_back(segmentBE[tokens[3]].first);
                    inDegree[segmentBE[tokens[3]].first] ++;
                } else {
                    father[segmentBE[tokens[3]].second].push_back(segmentBE[tokens[1]].second);
                    edge[segmentBE[tokens[1]].second].push_back(segmentBE[tokens[3]].second);
                    inDegree[segmentBE[tokens[3]].second] ++;
                }
            } else {
                if (tokens[4] == "+") {
                    father[segmentBE[tokens[3]].first].push_back(segmentBE[tokens[1]].first);
                    edge[segmentBE[tokens[1]].first].push_back(segmentBE[tokens[3]].first);
                    inDegree[segmentBE[tokens[3]].first] ++;
                } else {
                    father[segmentBE[tokens[3]].second].push_back(segmentBE[tokens[1]].first);
                    edge[segmentBE[tokens[1]].first].push_back(segmentBE[tokens[3]].second);
                    inDegree[segmentBE[tokens[3]].second] ++;
                }
            }
        }
    }
    ifs2.close();

    for (int i = 0; i < totalBases; i ++) {
        if (! inDegree[i]) {
            father[i].push_back(-1);
        }
    }
}


Graph::~Graph() {}


void Graph::topo_sort() {
    std::queue <int> que;

    for (int i = 0; i < totalBases; i ++) {
        // for (auto f: father[i]) {
        //     generation[i] = std::min(generation[f] + 1, generation[i]);
        // }
        if (! inDegree[i]) {
            que.push(i);
            topoOrder.push_back(i + 1);
            // generation[i] = 1;
        }
    }

    while (! que.empty()) {
        int queFront = que.front();
        que.pop();

        for (auto nxt: edge[queFront]) {
            inDegree[nxt] --;
            if (! inDegree[nxt]) {
                que.push(nxt);
                topoOrder.push_back(nxt + 1);
            }
        }
    }
}