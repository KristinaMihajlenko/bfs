#pragma once


#include "parray.hpp"
#include "datapar.hpp"
#include <vector>
#include <cstdint>
#include <atomic>
#include <functional>

inline pasl::pctl::parray<int32_t> par_bfs(
        uint32_t nodes_numbers, uint32_t start_node,
        std::vector<std::vector<uint32_t>> const& edges)
{
    pasl::pctl::parray<int32_t> dist(nodes_numbers, -1);
    dist[start_node] = 0;
    pasl::pctl::parray<int32_t> frontier = {static_cast<int32_t>(start_node)};
    while (frontier.size() != 0)
    {
        pasl::pctl::parray<uint32_t> curr_nodes_edges_size(frontier.size(), [&frontier, &edges](uint32_t node_idx)
                {
                    return edges[frontier[node_idx]].size();
                }
        );
        pasl::pctl::parray<uint32_t> pref_sum = pasl::pctl::scan(curr_nodes_edges_size.begin(), curr_nodes_edges_size.end(), static_cast<uint32_t>(0),
                [](uint32_t a, uint32_t b)
                {
                    return a + b;
                },
                pasl::pctl::scan_type::forward_exclusive_scan
        );
        uint32_t new_frontier_size = pref_sum[pref_sum.size() - 1] + curr_nodes_edges_size[curr_nodes_edges_size.size() - 1];
        pasl::pctl::parray<int32_t> new_frontier(new_frontier_size, -1);
        pasl::pctl::parallel_for(static_cast<uint32_t>(0), static_cast<uint32_t>(frontier.size()), [&edges, &frontier, &dist, &new_frontier, &pref_sum](uint32_t node_idx)
                {
                    uint32_t from_node = frontier[node_idx];
                    uint32_t start_idx = pref_sum[node_idx];

                    std::vector<uint32_t> const& to_nodes = edges[from_node];
                    for (uint32_t edge_num = 0; edge_num < to_nodes.size(); ++edge_num)
                    {
                        int32_t inf_dist = -1;
                        uint32_t to_node = to_nodes[edge_num];
                        int32_t new_dist = __atomic_load_n(&dist[from_node], __ATOMIC_SEQ_CST) + 1;
                        if (__atomic_compare_exchange_n( &dist[to_node], &inf_dist, new_dist, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
                        {
                            new_frontier[start_idx + edge_num] = to_node;
                        }
                    }
                }
        );
        frontier = pasl::pctl::filter(new_frontier.begin(), new_frontier.end(), [](int32_t cur_node)
                {
                    return cur_node != -1;
                }
        );
    }
    return dist;
}
