#include "alignment.h"


Alignment::Alignment(const std::string& query, const Graph& graph):
    query(query), graph(graph) {
        lquery = query.size();
        lgraph = graph.totalBases;
        edit_distance = lquery + lgraph;
}


Alignment::~Alignment() {}


void Alignment::poa() {
    std::vector< std::vector <int> > min_matrix = std::vector< std::vector <int> > (lquery + 1, std::vector <int> (lgraph + 1));
    std::vector< std::vector <std::pair<int, int> > > front_matrix = std::vector< std::vector <std::pair<int, int> > > (lquery + 1, std::vector < std::pair<int, int> > (lgraph + 1));
    
    for (int i = 0; i <= lquery; ++i) {
        min_matrix[i][0] = i;  // 初始化删除开销
    }
    // for (auto j: graph.topoOrder) {
    //     min_matrix[0][j] = graph.generation[j - 1];  // 初始化增开销
    // }

    for (int i = 1; i <= lquery; i ++) {
        for (auto j: graph.topoOrder) {
            min_matrix[i][j] = lquery + lgraph;
            // std::cout << i << " " << query[i - 1] << " " << j << " " << graph.baseVec[j - 1] << std::endl;
            for (auto fID: graph.father[j - 1]) {
                // 改
                if (min_matrix[i - 1][fID + 1] + (query[i - 1] == graph.baseVec[j - 1] ? 0 : 1) < min_matrix[i][j]) {
                    min_matrix[i][j] = min_matrix[i - 1][fID + 1] + (query[i - 1] == graph.baseVec[j - 1] ? 0 : 1);
                    front_matrix[i][j] = {i - 1, fID + 1};
                }

                // 增
                if (min_matrix[i][fID + 1] + 1 < min_matrix[i][j]) {
                    min_matrix[i][j] = min_matrix[i][fID + 1] + 1;
                    front_matrix[i][j] = {i, fID + 1};
                }
            }

            // 删
            if (min_matrix[i - 1][j] + 1 < min_matrix[i][j]) {
                min_matrix[i][j] = min_matrix[i - 1][j] + 1;
                front_matrix[i][j] = {i - 1, j};
            }
            // std::cout << i << " " << query[i - 1] << " " << j << " " << graph.baseVec[j - 1] << " " << min_matrix[i][j] << std::endl;
        }
    }

    int idxi = lquery, idxj;
    for (int j = 0; j <= lgraph; j ++) {
        if (min_matrix[idxi][j] < edit_distance) {
            edit_distance = min_matrix[idxi][j];
            idxj = j;
        }
    }
    min_matrix = std::vector< std::vector <int> > ();

    while (idxi) {
        int fi = front_matrix[idxi][idxj].first, fj = front_matrix[idxi][idxj].second;
        if (fi + 1 == idxi && idxj != fj) {
            if (query[idxi - 1] == graph.baseVec[idxj - 1]) {
                match.push_back('|');
            } else {
                match.push_back(' ');
            }
            str1.push_back(query[idxi - 1]);
            str2.push_back(graph.baseVec[idxj - 1]);
        } else if (fi + 1 == idxi) {
            str1.push_back(query[idxi - 1]);
            str2.push_back('-');
            match.push_back(' ');
        } else {
            str1.push_back('-');
            str2.push_back(graph.baseVec[idxj - 1]);
            match.push_back(' ');
        }
        idxi = fi, idxj = fj;
    }
}


void Alignment::navarro() {
    std::vector< std::vector <int> > min_matrix = std::vector< std::vector <int> > (lquery + 1, std::vector <int> (lgraph + 1));
    std::vector< std::vector <std::pair<int, int> > > front_matrix = std::vector< std::vector <std::pair<int, int> > > (lquery + 1, std::vector < std::pair<int, int> > (lgraph + 1));
    
    for (int i = 0; i <= lquery; ++i) {
        min_matrix[i][0] = i;  // 初始化删除开销
    }

    for (int i = 1; i <= lquery; i ++) {
        for (int j = 1; j <= lgraph; j ++) {
            min_matrix[i][j] = min_matrix[i - 1][j] + 1;
            front_matrix[i][j] = {i - 1, j};

            for (auto fa: graph.father[j - 1]) {
                if (min_matrix[i - 1][fa + 1] + (query[i - 1] == graph.baseVec[j - 1] ? 0 : 1) < min_matrix[i][j]) {
                    min_matrix[i][j] = min_matrix[i - 1][fa + 1] + (query[i - 1] == graph.baseVec[j - 1] ? 0 : 1);
                    front_matrix[i][j] = {i - 1, fa + 1};
                }
            }
        }
        for (int j = 1; j <= lgraph; j ++) {
            for (auto son: graph.edge[j - 1]) {
                propagate(i, j, son, min_matrix, front_matrix);
            }
        }
        // for (int j = 1; j <= lgraph; j ++) {
        //     std::cout << min_matrix[i][j] << "\t";
        // }
        // std::cout << std::endl;
    }

    int idxi = lquery, idxj;
    for (int j = 0; j <= lgraph; j ++) {
        if (min_matrix[idxi][j] < edit_distance) {
            edit_distance = min_matrix[idxi][j];
            idxj = j;
        }
    }
    min_matrix = std::vector< std::vector <int> > ();

    while (idxi) {
        int fi = front_matrix[idxi][idxj].first, fj = front_matrix[idxi][idxj].second;
        if (fi + 1 == idxi && idxj != fj) {
            if (query[idxi - 1] == graph.baseVec[idxj - 1]) {
                match.push_back('|');
            } else {
                match.push_back(' ');
            }
            str1.push_back(query[idxi - 1]);
            str2.push_back(graph.baseVec[idxj - 1]);
        } else if (fi + 1 == idxi) {
            str1.push_back(query[idxi - 1]);
            str2.push_back('-');
            match.push_back(' ');
        } else {
            str1.push_back('-');
            str2.push_back(graph.baseVec[idxj - 1]);
            match.push_back(' ');
        }
        idxi = fi, idxj = fj;
    }
}


void Alignment::propagate(int i, int u, int v, std::vector< std::vector <int> >& min_matrix, std::vector< std::vector <std::pair<int, int> > >& front_matrix) {
    if (min_matrix[i][v + 1] > min_matrix[i][u] + 1) {
        min_matrix[i][v + 1] = min_matrix[i][u] + 1;
        front_matrix[i][v + 1] = {i, u};
        for (auto son: graph.edge[v]) {
            propagate(i, v + 1, son, min_matrix, front_matrix);
        }
    }
}


void Alignment::print(const std::string& outputPath) {
    std::filesystem::create_directories(outputPath);
    std::string outputFile;

    if (outputPath.back() != '/') {
        outputFile = outputPath + "/match.txt";
    } else {
        outputFile = outputPath + "match.txt";
    }
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    size_t length = std::max({str1.length(), match.length(), str2.length()});

    for (size_t i = 0; i < length; i += 80) {
        for (size_t j = 0; j < 80 && i + j < length; ++j) {
            output << str1[str1.length() - 1 - (i + j)];
        }
        output << "\n";
        for (size_t j = 0; j < 80 && i + j < length; ++j) {
            output << match[match.length() - 1 - (i + j)];
        }
        output << "\n";
        for (size_t j = 0; j < 80 && i + j < length; ++j) {
            output << str2[str2.length() - 1 - (i + j)];
        }
        output << "\n";
    }
}