#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.hpp"

using pixels = std::vector<std::uint8_t>;
using color = std::vector<std::uint8_t>;
using cells = std::vector<int>;

class SceneGenerator 
{
    public:
        SceneGenerator(uint16_t& height,
                       uint16_t& width,
                       int& size_cell,
                       color color_cell,
                       color color_grid) :
                       height(height),
                       width(width),
                       size_cell(size_cell),
                       height_grid(height / size_cell),
                       width_grid(width / size_cell),
                       color_cell(color_cell),
                       color_grid(color_grid),
                       scene({width, height}),
                       scene_sprite(scene)
        {
            pixels_grid.resize(height * width * 4);
            pixels_scene.resize(height * width * 4);
            generateGrid();
        }
        void generateGrid() 
        {
            for(int y = 0; y < height; ++y) 
            {
                for(int x = 0; x < width; ++x) 
                {
                    int index = utils::getIndexToCoord(x, y, width) * 4;

                    pixels_grid[index + 0] = 255;
                    pixels_grid[index + 1] = 255;
                    pixels_grid[index + 2] = 255;
                    pixels_grid[index + 3] = 255;

                    if (x % size_cell == 0 || y % size_cell == 0)
                    {
                        pixels_grid[index + 0] = color_grid[0];
                        pixels_grid[index + 1] = color_grid[1];
                        pixels_grid[index + 2] = color_grid[2];
                        pixels_grid[index + 3] = color_grid[3];
                    }
                }	
            }
        }
        void fillRect(int& y, int& x)
        {
            int start_x = x * size_cell;
            int start_y = y * size_cell;
            for(int i = 0; i < size_cell; ++i) 
            {
                for(int j = 0; j < size_cell; ++j) 
                {
                    int current_x = start_x + j;
                    int current_y = start_y + i;
                    int fill_index = (current_y * width + current_x) * 4;
                    pixels_scene[fill_index + 0] = color_cell[0];
                    pixels_scene[fill_index + 1] = color_cell[1];
                    pixels_scene[fill_index + 2] = color_cell[2];
                    pixels_scene[fill_index + 3] = color_cell[3];
                }
            }
        }  
        void generateScene(const cells& current_state) 
        {
            pixels_scene = pixels_grid;
            for(int i = 0; i < height_grid; ++i) 
            {
                for(int j = 0; j < width_grid; ++j) 
                {
                    if(current_state[utils::getIndexToCoord(j, i, width_grid)])
                    {
                        fillRect(i, j);
                    }
                }
            }
            scene.update(pixels_scene.data());
        }
        const sf::Sprite& getSpriteScene() const 
        {
            return scene_sprite;
        }
    private:
    	sf::Texture scene;
	    sf::Sprite scene_sprite;
        uint16_t height;
        uint16_t width;
        uint16_t height_grid;
        uint16_t width_grid;
        int size_cell;

        color color_grid;
        color color_cell;

	    pixels pixels_grid;
	    pixels pixels_scene;
};