#pragma once

#include <vector>
#include <cstdint>
#include <queue>

inline std::vector<int32_t> seq_bfs(
        uint32_t nodes_count, uint32_t start_node,
        std::vector<std::vector<uint32_t>> const& edges)
{
    std::vector<int32_t> dist(nodes_count, -1);
    dist[start_node] = 0;
    std::queue<uint32_t> q;
    q.push(start_node);
    while (!q.empty())
    {
        uint32_t from_node = q.front();
        q.pop();
        for (uint32_t to_node : edges[from_node])
        {
            if (dist[to_node] == -1)
            {
                dist[to_node] = dist[from_node] + 1;
                q.push(to_node);
            }
        }
    }
    return dist;
}
