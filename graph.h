#include <iostream>
#include <vector>

uint32_t get_cube_number(uint32_t len_cube, uint32_t x, uint32_t y, uint32_t z)
{
    return z * len_cube * len_cube + y * len_cube + x;
}


std::vector<std::vector<uint32_t>> build_cube_graph(uint32_t len_cube)
{
    std::vector<std::vector<uint32_t>> edges(len_cube * len_cube * len_cube);
    for (uint32_t z = 0; z < len_cube; z++) 
    {
        for (uint32_t y = 0; y < len_cube; y++) 
        {
            for (uint32_t x = 0; x < len_cube; x++)
            {
                uint32_t id_curr_node = get_cube_number(len_cube, x,  y, z);
                if (x != len_cube - 1) 
                {
                    uint32_t id_x_node = get_cube_number(len_cube, x + 1,  y, z);
                    edges[id_curr_node].push_back(id_x_node);
                    edges[id_x_node].push_back(id_curr_node);
                }
                if (y != len_cube - 1) 
                {
                    uint32_t id_y_node = get_cube_number(len_cube, x,  y + 1, z);
                    edges[id_curr_node].push_back(id_y_node);
                    edges[id_y_node].push_back(id_curr_node);
                }
                if (z != len_cube - 1) 
                {
                    uint32_t id_z_node = get_cube_number(len_cube, x,  y, z + 1);
                    edges[id_curr_node].push_back(id_z_node);
                    edges[id_z_node].push_back(id_curr_node);
                }
            }
        }
    }
    return edges;
}
