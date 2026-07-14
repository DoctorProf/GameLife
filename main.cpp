#include "include/SceneGenerator.hpp"
#include "include/GuiController.hpp"
#include "include/ConfigController.hpp"

//g++ -std=c++17 main.cpp -lsfml-graphics -lsfml-window -lsfml-system -o gamelife.exe
//g++ -std=c++20 main.cpp -ltgui -lsfml-graphics -lsfml-window -lsfml-system -O3 -march=native -ffast-math -o gamelife.exe

int main()
{
	uint16_t height = 0;
	uint16_t width = 0;
	int size_cell = 0;
	int tick_rate = 0;
	float procent_life_cells = 0.2f;

	height = utils::writeData("height", 500);
	width = utils::writeData("width", 500);
	size_cell = utils::writeData("size_cell", 5);
	tick_rate = utils::writeData("tick_rate", 60);
	procent_life_cells = utils::writeData("procent_life_cells", 0.2f);
	
	Simulation simulation(height, width, size_cell, procent_life_cells, tick_rate);
	simulation.create();
	
	SceneGenerator scene_generator{height, width, size_cell, {255, 0, 0, 255},
															 {0, 0, 0, 255} };

    sf::RenderWindow window(sf::VideoMode({width, height}),"Life");
	tgui::Gui gui(window);

	sf::Clock clock;
	float tick_per_sec = 1000.f / (float)tick_rate;
	sf::Time accumulator = sf::Time::Zero;

    GuiController gui_controller(&gui, simulation);
    
    bool pause = gui_controller.isPaused();
    bool spawn_block = gui_controller.isSpawnGliderMode();
	bool visible_gui = false;
    gui_controller.setVisiblePanel(visible_gui);
	
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            gui.handleEvent(*event);
            if (event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
            {
                window.close();
            }
			if (event->is<sf::Event::KeyPressed>() &&
                event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::H)
            {
                visible_gui = !visible_gui;
				gui_controller.setVisiblePanel(visible_gui);
            }
        }
        
        pause = gui_controller.isPaused();
        spawn_block = gui_controller.isSpawnGliderMode();

		if(spawn_block)
		 {
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
			{
				auto mouse_pos = sf::Mouse::getPosition(window);
				if (!gui.getWidgetBelowMouseCursor(mouse_pos, true)) 
				{
					auto world_pos = window.mapPixelToCoords(mouse_pos);
					auto cell_pos = world_pos / (float)size_cell;
					simulation.spawnGlaider(cell_pos.x, cell_pos.y);
				}
			}
		}
		else 
		{
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
			{
				auto mouse_pos = sf::Mouse::getPosition(window);
				if (!gui.getWidgetBelowMouseCursor(mouse_pos, true))
				{
					auto world_pos = window.mapPixelToCoords(mouse_pos);
					auto cell_pos = world_pos / (float)size_cell;
					int index = utils::getIndexToCoord(cell_pos.x, cell_pos.y, simulation.getWidthGrid());
					simulation.setStateCell(1, index);
				}
			}
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) 
			{
				auto mouse_pos = sf::Mouse::getPosition(window);
				if (!gui.getWidgetBelowMouseCursor(mouse_pos, true))
				{
					auto world_pos = window.mapPixelToCoords(mouse_pos);
					auto cell_pos = world_pos / (float)size_cell;
					int index = utils::getIndexToCoord(cell_pos.x, cell_pos.y, simulation.getWidthGrid());
					simulation.setStateCell(0, index);
				}
			}
		}
        accumulator += clock.restart();
        float tick_per_sec = 1000.f / gui_controller.getTickRate();
        if(!pause && accumulator.asMilliseconds() >= tick_per_sec) 
        {
            accumulator = sf::Time::Zero;
            simulation.update();
        }
        
        window.clear(sf::Color::White);
        scene_generator.generateScene(simulation.getCurrentState());
        window.draw(scene_generator.getSpriteScene());
        gui.draw();
        window.display();
    }
    
    return 0;
}
