#define NDEBUG

#include "par_bfs.h"
#include "seq_bfs.h"
#include "graph.h"
#include "parray.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <functional>
#include <array>
#include <string>


int main()
{
    std::vector<std::vector<uint32_t>> edges_three = {
            {1, 3, 9},
            {0, 2, 4, 10},
            {1, 5, 11},
            {0, 4, 6, 12},
            {1, 3, 5, 7, 13},
            {2, 4, 8, 14},
            {3, 7, 15},
            {4, 6, 8, 16},
            {5, 7, 17},
            {0, 10, 12, 18},
            {1, 9, 11, 13, 19},
            {2, 10, 14, 20},
            {3, 9, 13, 15, 21},
            {4, 10, 12, 14, 16, 22},
            {5, 11, 13, 17, 23},
            {6, 12, 16, 24},
            {7, 13, 15, 17, 25},
            {8, 14, 16, 26},
            {9, 19, 21},
            {10, 18, 20, 22},
            {11, 19, 23},
            {12, 18, 22, 24},
            {13, 19, 21, 23, 25},
            {14, 20, 22, 26},
            {15, 21, 25},
            {16, 22, 24, 26},
            {17, 23, 25}
    };
    std::vector<std::vector<uint32_t>> edges_test = build_cube_graph(3);
    if (edges_three != edges_test){
        std::cout << "Wrong cube" << std::endl;
        return 0;
    }
    else {
        std::cout << "Right cube" << std::endl;
    }

    std::vector<int32_t> seq_bfs_res = seq_bfs(27, 0, edges_test);
    pasl::pctl::parray<int32_t> par_bfs_res = par_bfs(27, 0, edges_test);
 

    if (seq_bfs_res.size() != par_bfs_res.size()) {
        std::cout << "Wrong parallel size" << std::endl;
        return 0;
    }
    for (int32_t i; i < seq_bfs_res.size(); i++){
        if (seq_bfs_res[i] != par_bfs_res[i]) {
            std::cout << "Wrong parallel version" << std::endl;
            return 0;
        }
    }

    std::cout << "Right parallel version" << std::endl;
    uint32_t len_cube = 500;
    uint32_t nodes_count = len_cube * len_cube * len_cube;
    auto edges = build_cube_graph(len_cube);

    uint64_t par_sum = 0;
    uint64_t seq_sum = 0;
    for (int32_t i = 0; i < 5; i++) {
        std::chrono::steady_clock::time_point begin_seq = std::chrono::steady_clock::now();
        seq_bfs(nodes_count, 0, edges);
        std::chrono::steady_clock::time_point end_seq = std::chrono::steady_clock::now();
        uint64_t seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - begin_seq).count();
        seq_sum += seq_time;
        std::cout << "Iteration " << i << " seq time: " << seq_time << std::endl;
        std::chrono::steady_clock::time_point begin_par = std::chrono::steady_clock::now();
        par_bfs(nodes_count, 0, edges);
        std::chrono::steady_clock::time_point end_par = std::chrono::steady_clock::now();
        uint64_t par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - begin_par).count();
        par_sum += par_time;
        std::cout << "Iteration " << i << " par time: " << par_time << std::endl;
        std::cout << "______________________________________________________________" << std::endl;

    }

    uint64_t mean_seq = seq_sum / 5;
    std::cout << "Mean seq time: " << mean_seq << std::endl;


    uint64_t mean_par = par_sum / 5;
    std::cout << "Mean par time: " << mean_par << std::endl;

    double better_bfs = static_cast<double>(mean_seq) / mean_par;
    std::cout << "Par bfs is better than seq in: " << better_bfs << std::endl;

    return 0;
}

