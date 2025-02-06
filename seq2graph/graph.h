#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <bitset>
#include <utility>
#include <queue>


class Graph {
    public:
        int totalBases;
        std::vector <std::vector <int> > father;
        std::vector <char> baseVec;
        std::vector <int> topoOrder, generation;

        Graph(const std::string&);
        ~Graph();
        void topo_sort();

    private:
        std::vector <int> inDegree;
};

#endif