#pragma once

#include <iostream>
#include <vector>

#define TILE_HEIGHT 32
#define TILE_WIDTH 32
#define TILE_BYTES TILE_HEIGHT * TILE_WIDTH //1024
#define TILE_UINT32_COUNT (TILE_HEIGHT * TILE_WIDTH / sizeof(uint32_t)) //256
#define TILE_BFLOAT16_COUNT TILE_UINT32_COUNT * 2 //512

class BRgrid {

    public:

    std::vector<std::vector<uint32_t>> grid;

    BRgrid(int blocks_x, int blocks_y) : blocks_x(blocks_x), blocks_y(blocks_y), blocks_count(blocks_x * blocks_y) {
        m_buffer = (uint32_t*) malloc(blocks_x * blocks_y * TILE_BYTES);
        
        std::vector<std::vector<uint32_t>> h_scalar_mat(blocks_x * blocks_y, std::vector<uint32_t>(TILE_UINT32_COUNT, 0));
        
    }

    ~BRgrid() {
        grid.clear();
        free(m_buffer);
    }

    inline uint32_t* Buffer(){
        return m_buffer;
    }

    void generateFromBuffer(uint32_t* buff) {

        // THE GENERATION IS BASED ON A PRE-DEFINED BLOCK SIZE, IF YOU DO IT WRONG, YOU'LL HAVE A SEGFAULT
        // HOW CAN I CHECK THIS?

        for(int i = 0; i<blocks_x*blocks_y; i++){
            for(int j = 0; j<TILE_UINT32_COUNT; j++){
                grid[i][j] =  buff[i*TILE_UINT32_COUNT + j];
            }
        }

        is_initialized = true;
    }

    void printGrid(){

        if(!is_initialized) {
            std::cerr << "Grid not initialized!" << std::endl;
            return;
        }

        // TO IMPLEMENT
    }

    void saveGridCSV(const std::string& filename) {

        if(!is_initialized) {
            std::cerr << "Grid not initialized!" << std::endl;
            return;
        }

        // TO IMPLEMENT
    }

    private:

    int blocks_x;
    int blocks_y;
    int blocks_count;
    bool is_initialized = false;
    uint32_t* m_buffer;

};
