#ifndef __ALIGNMENT_H__
#define __ALIGNMENT_H__

#include "graph.h"

#include <string>
#include <utility>
#include <vector>
#include <filesystem>
#include <algorithm>

class Alignment {
    public:
        int edit_distance;
        std::string str1, str2, match;

        Alignment(const std::string&, const Graph&);
        ~Alignment();

        void dp();
        void print(const std::string&);

    private:
        const std::string& query;
        const Graph& graph;
        int lquery, lgraph;
};

#endif