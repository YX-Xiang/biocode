#include "alignment.h"


Alignment::Alignment(const std::string& query, const Graph& graph):
    query(query), graph(graph) {
        lquery = query.size();
        lgraph = graph.totalBases;
}


Alignment::~Alignment() {}


void Alignment::dp() {
    std::vector< std::vector <int> > min_matrix = std::vector< std::vector <int> > (lquery + 1, std::vector <int> (lgraph + 1));
    std::vector< std::vector <std::pair<int, int> > > front_matrix = std::vector< std::vector <std::pair<int, int> > > (lquery + 1, std::vector < std::pair<int, int> > (lgraph + 1));
    
    for (int i = 0; i <= lquery; ++i) {
        min_matrix[i][0] = i;  // 初始化删除开销
    }
    for (auto j: graph.topoOrder) {
        min_matrix[0][j] = graph.generation[j - 1];  // 初始化增开销
    }

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
    edit_distance = min_matrix[lquery][*graph.topoOrder.rbegin()];
    min_matrix = std::vector< std::vector <int> > ();

    int idxi = lquery, idxj = *graph.topoOrder.rbegin();
    while (idxi || idxj) {
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


// void Alignment::hirschberg_path() {
//     std::vector <std::vector <int> > xy = std::vector <std::vector <int> > (64, std::vector <int> (4));
//     std::vector <std::vector <int> > row = std::vector <std::vector <int> > (2, std::vector <int> (lgraph));
//     std::vector <int> path = std::vector <int> (lquery);
//     int p = 0, x1, x2, y1, y2, x3, y3, smax = -0x7FFFFFFF;
//     xy[p][0] = 0; xy[p][1] = lgraph; xy[p][2] = 0; xy[p][3] = lquery;

//     while(p >= 0) {
//         x1 = xy[p][0]; x2 = xy[p][1]; y1 = xy[p][2]; y2 = xy[p][3]; p --;
//         y3 = ((y1 + y2) >> 1);
//         x3 = hirschberg_core(x1, x2, y1, y2, row);
//         path[y3] = x3;
//         if (smax == -0x7FFFFFFF) {
//             smax = row[0][x3];
//         }

//         if (y1 == y3) {
//             x3 ++;
//         }
//         if (x1 == x3) {
//             y3 ++;
//         }
//         if (x1 < x3 && y1 < y3) {
//             p ++;
//             xy[p][0] = x1; xy[p][1] = x3;
//             xy[p][2] = y1; xy[p][3] = y3;
//         }
//         if (x2 > x3 && y2 > y3) {
//             p ++;
//             xy[p][0] = x3; xy[p][1] = x2;
//             xy[p][2] = y3; xy[p][3] = y2;
//         }
//     }
//     edit_distance = smax;
// }


// // 初始时 graphb = 0， graphe = lgraph，seqb = 0，seqe = lquery
// int Alignment::hirschberg_core(int graphb, int graphe, int seqb, int seqe, std::vector <std::vector <int> >& row) {
//     int seq_mid = (seqb + seqe) >> 1;
//     // if (graphb + 1 >= graphe || seqb + 1 >= seqe) {
//     //     return (seqb + seqe) >> 1;
//     // }

//     // 初始化增的开销
//     for (int i = graphb; i < graphe; i ++) {
//         row[0][graph.topoOrder[i]] = graph.generation[graph.topoOrder[i]] - graph.generation[graph.topoOrder[graphb]] + 1;
//     }
//     for (int i = graphb; i < graphe; i ++) {
//         row[1][graph.topoOrder[i]] = graph.generation[graph.topoOrder[graphe - 1]] - graph.generation[graph.topoOrder[i]] + 1;
//     }

//     fwd_calc_align_matirx(graphb, graphe, seqb, seqb + seq_mid, row[0]);
//     rev_calc_align_matrix(graphb, graphe, seqb + seq_mid, seqe, row[1]);

//     int query_x;
//     for (int i = graphb; i < graphe; i ++) {
//         row[0][graph.topoOrder[i]] += row[1][graph.topoOrder[i]];
//         if (row[0][graph.topoOrder[i]] > row[0][graph.topoOrder[query_x]]) {
//             query_x = i;
//         }
//     }
//     return query_x;
// }


// // 改和增是通过 father 状态转移的
// void Alignment::fwd_calc_align_matirx (int graphb, int graphe, int seqb, int seqe, std::vector <int>& row) {
//     std::vector <int> score = std::vector <int> (lgraph);
//     for (int y = seqb; y < seqe; y ++) {
//         for (int x = graphb; x < graphe; x ++) {
//             int node = graph.topoOrder[x];
//             for (auto fID: graph.father[node]) {
//                 if (fID >= 0 && graph.generation[fID] >= graph.generation[graph.topoOrder[graphb]]) {
//                     // 改
//                     score[x] = row[fID] + (query[y] == graph.baseVec[node] ? 0 : 1);
//                     // 增
//                     score[x] = std::min(score[x], score[fID] + 1);
//                 } else {
//                     // 改
//                     score[x] = (query[y] == graph.baseVec[node] ? 0 : 1);
//                     // 增
//                     score[x] = std::min(score[x], y - seqb + 1);
//                 }
//             }
//             // 删
//             score[x] = std::min(score[x], row[x] + 1);
//         }
//         for (int x = graphb; x < graphe; x ++) {
//             row[x] = score[x];
//         }
//     }
// }


// // 改和增是通过 edge 状态转移的
// void Alignment::rev_calc_align_matrix (int graphb, int graphe, int seqb, int seqe, std::vector <int>& row) {
//     std::vector <int> score = std::vector <int> (lgraph);
//     for (int y = seqe - 1; y >= seqb; y --) {
//         for (int x = graphe - 1; x >= graphb; x --) {
//             int node = graph.topoOrder[x];
//             if (graph.edge[node].empty()) {
//                 // 改
//                 score[x] = (query[y] == graph.baseVec[node] ? 0 : 1);
//                 // 增
//                 score[x] = std::min(score[x], y - seqe + 2);
//             }
//             for (auto sID: graph.edge[node]) {
//                 if (graph.generation[sID] <= graph.generation[graph.topoOrder[graphe - 1]]) {
//                     // 改
//                     score[x] = row[sID] + (query[y] == graph.baseVec[node] ? 0 : 1);
//                     // 增
//                     score[x] = std::min(score[x], score[sID] + 1);
//                 } else {
//                     // 改
//                     score[x] = seqe - y - 1 + (query[y] == graph.baseVec[node] ? 0 : 1);
//                     // 增
//                     score[x] = std::min(score[x], seqe - y);
//                 }
//             }
//             // 删
//             score[x] = std::min(score[x], row[x] + 1);
//         }
//         for (int x = graphb; x < graphe; x ++) {
//             row[x] = score[x];
//         }
//     }
// }


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