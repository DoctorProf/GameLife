#pragma once
#include "RandGenerator.hpp"
#include "Utils.hpp"
#include <unordered_set>

class Simulation 
{
    public:
        Simulation(uint16_t& height,
                   uint16_t& width,
                   int& size_cell,
                   float& procent_life_cells,
                   int& tick_rate) :
                   height(height),
                   width(width),
                   size_cell(size_cell),
                   procent_life_cells(procent_life_cells),
                   tick_rate(tick_rate),
                   rand_generator(RandGenerator{0.f, 1.f})
        {
            height_grid = height / size_cell;
            width_grid = width / size_cell;
            survival_rules = {2, 3};
            birth_rules = {3};
        }
        void create() 
        {
            generateField();
        }
        void reload() 
        {
            generateField();
        }
        void close() 
        {

        }
        void update() 
        {
            calculateState();
			previous_state = current_state;
        }
        void generateField()
		{
			int count = 0;
			current_state.clear();
			for(int i = 0; i < height_grid * width_grid; i++) 
			{
                int value = rand_generator.generateValue() < procent_life_cells ? 1 : 0;
                if(value) count++;
                current_state.push_back(value);
			}
            std::cout << "count - " << count << std::endl;
			previous_state = current_state;
		}
        bool checkLife(int cell, int& count)
		{
			if(cell)
			{
				return survival_rules.contains(count);
			}
			else 
			{
				return birth_rules.contains(count);
			}
			
		}
        void calculateState()
		{
			for (int idx = 0; idx < height_grid * width_grid; idx++) 
            {
                int y = idx / width_grid;
                int x = idx % width_grid;
                
                int count = 0;
                
                for (int dy = -1; dy <= 1; dy++) 
                {
                    for (int dx = -1; dx <= 1; dx++) 
                    {
                        if (dy == 0 && dx == 0) continue;
                        
                        int ny = y + dy;
                        int nx = x + dx;
                        
                        if (utils::checkBounds(ny, nx, height_grid, width_grid)) 
                        {
                            int nidx = utils::getIndexToCoord(nx, ny, width_grid);
                            count += previous_state[nidx];
                        }
                    }
                }
                
                current_state[idx] = checkLife(previous_state[idx], count);
            }
		}
        void spawnGlaider(int x, int y)
		{
			if(utils::checkBounds(y - 1, x, height_grid, width_grid))
			{
                int index = utils::getIndexToCoord(x, y - 1, width_grid);
				setStateCell(1, index);
			}
			if(utils::checkBounds(y, x + 1, height_grid, width_grid))
			{
				int index = utils::getIndexToCoord(x + 1, y, width_grid);
				setStateCell(1, index);
			}
			if(utils::checkBounds(y + 1, x - 1, height_grid, width_grid))
			{
				int index = utils::getIndexToCoord(x - 1, y + 1, width_grid);
				setStateCell(1, index);
			}
			if(utils::checkBounds(y + 1, x, height_grid, width_grid))
			{
				int index = utils::getIndexToCoord(x, y + 1, width_grid);
				setStateCell(1, index);
			}
			if(utils::checkBounds(y + 1, x + 1, height_grid, width_grid))
			{
				int index = utils::getIndexToCoord(x + 1, y + 1, width_grid);
				setStateCell(1, index);
			}
		}
        cells& getCurrentState() 
        {
            return current_state;
        }
        cells& getPreviousState()
        {
            return previous_state;
        }
        uint16_t& getHeight()
        {
            return height;
        }
        uint16_t& getWidth()
        {
            return width;
        }
        uint16_t& getHeightGrid()
        {
            return height_grid;
        }
        uint16_t& getWidthGrid()
        {
            return width_grid;
        }
        int& getSizeCell()
        {
            return size_cell;
        }
        float& getProcentLifeCell()
        {
            return procent_life_cells;
        }
        void setStateCell(int state, int index) 
        {
            current_state[index] = state;
            previous_state[index] = state;
        }
        const std::unordered_set<int>& getBirthRules() const 
        { 
            return birth_rules; 
        }
        const std::unordered_set<int>& getSurvivalRules() const 
        { 
            return survival_rules; 
        }
        void addBirthRule(int n) 
        { 
            birth_rules.insert(n); 
        }
        void removeBirthRule(int n) 
        { 
            birth_rules.erase(n);
        }
        void clearBirthRules() 
        { 
            birth_rules.clear();
        }
        
        void addSurvivalRule(int n) { survival_rules.insert(n); }
        void removeSurvivalRule(int n) { survival_rules.erase(n); }
        void clearSurvivalRules() { survival_rules.clear(); }
        
        void setBirthRules(const std::unordered_set<int>& rules) 
        { 
            birth_rules = rules;
        }
        void setSurvivalRules(const std::unordered_set<int>& rules) 
        { 
            survival_rules = rules;
        }
        
        void setTickRate(int tps) 
        { 
            tick_rate = tps;
        }
        int getTickRate() const 
        { 
            return tick_rate;
        }
        
        void setProcentLifeCells(float p) 
        { 
            procent_life_cells = p;
        }
        float getProcentLifeCells() const 
        { 
            return procent_life_cells;
        }
        
    private:
        RandGenerator<float> rand_generator;

        uint16_t height;
        uint16_t width;
        uint16_t height_grid;
		uint16_t width_grid;
        int tick_rate;

        int size_cell;
        float procent_life_cells;

        cells previous_state;
        cells current_state;

        std::unordered_set<int> survival_rules;
        std::unordered_set<int> birth_rules;
};