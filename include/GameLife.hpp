#include "Utils.hpp"


using namespace utils;
using matrix = std::vector<std::vector<int>>;

class GameLife
{
	public:
		GameLife(uint16_t height, uint16_t width, int size_cell, float procent_life_cell) :
			height(height), 
			width(width), 
			size_cell(size_cell), 
			procent_life_cell(procent_life_cell),
			gen(std::random_device{}()),
			dist(0.0, 1.0)
		{
			height_grid = height / size_cell;
			width_grid = width / size_cell;
			generateField();
		}
		void generateField()
		{
			int count = 0;
			current_state.clear();
			for(int i = 0; i < height_grid; i++) 
			{
				current_state.push_back(std::vector<int>());
				for(int j = 0; j < width_grid; j++) 
				{
					int value = dist(gen) < procent_life_cell ? 1 : 0;
					if(value) count++;
					current_state[i].push_back(value);
				}
			}
			std::cout << "count - " << count << std::endl;
			previous_state = current_state;
		}
		bool checkLife(int cell, int& count)
		{
			if(cell)
			{
				return (count == 2 || count == 3);
			}
			else 
			{
				return count == 3;
			}
			
		}
		
		void calculateState()
		{
			for(int y = 0; y < height_grid; y++) 
			{
				for(int x = 0; x < width_grid; x++) 
				{
					int count = 0;
					for(int dy = -1; dy < 2; dy++)
					{
						for(int dx = -1; dx < 2; dx++)
						{
							int i = y + dy;
							int j = x + dx;
							if(y == i && x == j) continue;
							if(!checkBounds(i, j, height_grid, width_grid)) count += previous_state[i][j];
						}
					}
					current_state[y][x] = checkLife(previous_state[y][x], count);
				}
			}
		}
		
		void update() 
		{
			calculateState();
			previous_state = current_state;
		}

		void reloadGame() 
		{
			generateField();
		}
		
		void spawnGlaider(int x, int y)
		{
			if(!checkBounds(x, y - 1, height_grid, width_grid))
			{
				previous_state[y - 1][x] = 1;
				current_state[y - 1][x] = 1;
			}
			if(!checkBounds(x + 1, y, height_grid, width_grid))
			{
				previous_state[y][x + 1] = 1;
				current_state[y][x + 1] = 1;
			}
			if(!checkBounds(x - 1, y + 1, height_grid, width_grid))
			{
				previous_state[y + 1][x - 1] = 1;
				current_state[y + 1][x - 1] = 1;
			}
			if(!checkBounds(x, y + 1, height_grid, width_grid))
			{
				previous_state[y + 1][x] = 1;
				current_state[y + 1][x] = 1;
			}
			if(!checkBounds(x + 1, y + 1, height_grid, width_grid))
			{
				previous_state[y + 1][x + 1] = 1;
				current_state[y + 1][x + 1] = 1;
			}
		}

		matrix current_state;
		matrix previous_state;
		uint16_t height;
		uint16_t width;
		float procent_life_cell;
		int size_cell;
		int height_grid;
		int width_grid;
	private:
		std::mt19937 gen;
		std::uniform_real_distribution<double> dist;
};