//
// Created by leanne on 6/12/21.
//

#ifndef HYDRAULICNETWORK_GRAPH_H
#define HYDRAULICNETWORK_GRAPH_H
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <iterator>
#include <cassert>
#include <fstream>

namespace HydraulicNetwork {


    class graph {
        int r;
        std::vector<std::vector<int>> adjMatrix;
        std::vector<std::vector<int>> treeAdjMat;

    public:
        // Initialize the matrix to zero
        graph(int r) : r(r), adjMatrix(r, std::vector<int>(r, 0)),
                       treeAdjMat(r, std::vector<int>(r)) {}
        void addEdge(int i, int j) {
            assert(i >= 0 && i < r && j >= 0 && j < r);
            ++adjMatrix[i][j];
            ++adjMatrix[j][i];
        }

        int size() const { return r; };

        template<class OutputIterator>
        void DFSCheckCycle(std::vector<std::vector<int>> &adjMat, size_t u, size_t par, std::vector<bool> &visited,
                           std::vector<int> &parents, size_t source, OutputIterator foundCycle) {
            if (visited[u]) {// if u is visited
                if (u == source) {
                    // mark u as seen by setting its parents
                    parents[u] = par;
                    while (true) {
                        // cycle found,
                        *foundCycle++ = u;
                        u = parents[u];
                        if (u == source) {
                            *foundCycle++ = u;// foundCycle.push_back(u);
                            break;
                        }
                    }
                }
                return;
            }
            visited[u] = true;
            parents[u] = par;
            for (size_t v = 0; v < adjMat.size(); ++v) {
                if (adjMat[u][v] == 1 && int(v) != parents[u]) {
                    DFSCheckCycle(adjMat, v, u, visited, parents, source, foundCycle);
                }
            }
        }

        // Block1: Partition the set of n nodes w.r.t. chosen edges to form a set of disjoint trees with
        // adjacency matrix B. For if row i of adjMatrix, set treeAdjMatrix[i][j] = 1 if
        // adjMatrix[i][j] is the 1st superdiagonal element of ith row of adjMatrix
        template<typename OutputIterator>
        OutputIterator Gotlieb123(OutputIterator cycles) {
            const auto r = adjMatrix.size();
            // Initialize adjacency matrix for spanning tree
            treeAdjMat = std::vector<std::vector<int>>(r, std::vector<int>(r, 0));
            for (size_t i = 0; i < r; ++i) {
                for (size_t j = i; j < r; ++j) {
                    if (adjMatrix[i][j] == 1) {
                        treeAdjMat[i][j] = 1;
                        treeAdjMat[j][i] = 1;
                        break;
                    }
                }
            }

            // BLOCK 2: Find all connected components
            /* Example: C = [ [ 1 1 1 0 0 1 ],
         *                [ 0 0 0 1 0 0 ],
         *                [ 0 0 0 0 1 0 ]]
         * It is like walking through a maze. Similar to DFS,
         * we need to know which way to go.
         * When we hit a dead end, we take the first thing off
         * the stack and go back there and try */

            std::vector<std::vector<int>> connComponents;
            std::vector<bool> visited(r, false);
            for (size_t u = 0; u < r; ++u) {
                if (visited[u])
                    continue;
                std::vector<int> component(r, 0);
                std::stack<int> s;
                s.push(u);
                while (!s.empty()) {
                    int v = s.top();
                    visited[v] = true;
                    component[v] = 1;
                    s.pop();
                    for (size_t w = 0; w < r; ++w) {
                        if (treeAdjMat[v][w] && !visited[w]) {
                            s.push(w);// put one branch and try onto stack
                        }
                    }
                }
                connComponents.push_back(component);
            }
            // Now focus on finding cycle base
            /* Block 3: Here the connected components are amalgamated by
        adding appropriate edges to the adjacency matrix B (treeAdjMat)
        Example: edge(1, 4) and (1, 5) are added back to B
        */
            //NOTE:
            //  some utility functions for connected components
            auto connComponentOrder = [](auto &c) { return std::count(std::begin(c), std::end(c), true); };
            auto toComponent =
                    [&](auto n) -> auto & {//NOTE: return auto &, since we get a copy by default
                return *std::find_if(std::begin(connComponents), std::end(connComponents), [&](auto &c) { return c[n]; });
            };

            while (connComponents.size() > 1) {
                //NOTE:
                //  sort the components in descending order (node count). descending so that we can easily remove the smallest with a pop_back.
                std::sort(std::begin(connComponents), std::end(connComponents), [&](auto &c0, auto &c1) { return connComponentOrder(c0) > connComponentOrder(c1); });
                auto &small = connComponents.back();
                auto &big =
                        [&]() -> auto & {//NOTE: find cross edge to bigger component, auto & again to get reference
                    for (int i = 0; i < r; ++i) {
                        if (!small[i])
                            continue;
                        for (int j = 0; j < r; ++j) {
                            if (adjMatrix[i][j] == 1 && !small[j]) {
                                treeAdjMat[i][j] = 1;
                                treeAdjMat[j][i] = 1;
                                return toComponent(j);
                            }
                        }
                    }
                    __builtin_unreachable();//NOTE: we should never reach this
                }
                ();

                //NOTE: move the nodes from small component to big component
                for (int i = 0; i < r; ++i)
                    if (small[i])
                        big[i] = true;
                connComponents.pop_back();
            }
            // BLOCK 4
            /* Collect all edges eliminated from the original adjacency matrix to
        build the spanning tree matrix
        */
            std::vector<std::pair<int, int>> eliminatedEdges;
            for (size_t i = 0; i < r; ++i)
                for (size_t j = i; j < r; ++j)
                    if (treeAdjMat[i][j] != adjMatrix[i][j])
                        eliminatedEdges.emplace_back(i, j);

            // count how many sides been eliminated to get the spanning tree
            // FINAL: Iterate through each eliminated edge, try adding it into mat B(treeAdjMat)
            // The use DFS to check the cycle, the source node is the first node of the edge
            // temporarily modifying a data structure.
            for (auto edge : eliminatedEdges) {
                visited = std::vector<bool>(r, false);
                std::vector<int> parents(r, -1);
                treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 1;  // adds an edge
                DFSCheckCycle(treeAdjMat, edge.first, -1, visited, parents, edge.first, cycles);// check for cycles
                treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 0;  // remove the edge
            }
            return cycles;
        }
    };

    // Requires a sequence of closed cycles.
    template<class ForwardIterator, class OutputStream>
    void print_cycles(ForwardIterator first, ForwardIterator last, OutputStream &os) {
        using T = typename std::iterator_traits<ForwardIterator>::value_type;
        while (first != last) {
            auto const cycle_start = first++;
            first = ++find(first, last, *cycle_start);
            copy(cycle_start, first, std::ostream_iterator<T>(os, " "));
            os << "\n";
        }
    }
}
#endif//HYDRAULICNETWORK_GRAPH_H
