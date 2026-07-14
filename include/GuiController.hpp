#pragma once
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Tabs.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <TGUI/Widgets/RadioButton.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <iomanip>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Simulation.hpp"

class GuiController
{
    public:
        GuiController(tgui::Gui* gui, 
                      Simulation& simulation) : gui(gui), simulation(simulation)
        {
            createTabControl();
        }
         
        bool isSpawnGliderMode() const 
        { 
            return spawn_glider_mode; 
        }
    
        bool isPaused() const 
        { 
            return paused;
        }
        
        int getTickRate() const 
        { 
            return simulation.getTickRate();
        }
        void setVisiblePanel(bool& visible_gui) 
        {
            gui->getWidgets().at(2)->setVisible(visible_gui);
            if(current_tab) 
            {
                gui->getWidgets().at(0)->setVisible(visible_gui);
            }
            else 
            {
                gui->getWidgets().at(1)->setVisible(visible_gui);
            }
        }
    private:

        void createTabControl()
        {
            auto tabs = tgui::Tabs::create();
            tabs->setPosition(10, 10);
            tabs->setSize(400, 30);
            tabs->add("Simulation");
            tabs->add("Rules");
            
            auto simPanel = tgui::Panel::create();
            simPanel->setPosition(10, 50);
            simPanel->setSize(400, 300);
            simPanel->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 200));
            
            auto rulesPanel = tgui::Panel::create();
            rulesPanel->setPosition(10, 50);
            rulesPanel->setSize(400, 300);
            rulesPanel->setVisible(false);
            rulesPanel->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 200));
            
            createSimulationTab(simPanel);
            createRulesTab(rulesPanel);
            
            gui->add(simPanel);
            gui->add(rulesPanel);
            gui->add(tabs);
            
            tabs->onTabSelect([this, simPanel, rulesPanel](const tgui::String& tabName) 
            {
                if (tabName == "Simulation")
                {
                    simPanel->setVisible(true);
                    rulesPanel->setVisible(false);
                    current_tab = true;
                } 
                else 
                {
                    simPanel->setVisible(false);
                    rulesPanel->setVisible(true);
                    current_tab = false;
                }
            });
        }

        void createSimulationTab(tgui::Panel::Ptr panel)
        {
            auto speedLabel = tgui::Label::create("Tick rate (1-500):");
            speedLabel->setPosition(10, 10);
            speedLabel->getRenderer()->setTextColor(tgui::Color::White);
            panel->add(speedLabel);
            
            speed_slider = tgui::Slider::create();
            speed_slider->setPosition(10, 35);
            speed_slider->setSize(380, 20);
            speed_slider->setMinimum(1);
            speed_slider->setMaximum(500);
            speed_slider->setValue(simulation.getTickRate());
            speed_slider->onValueChange([this](int value) {
                simulation.setTickRate(value);
            });
            panel->add(speed_slider);
            
            auto speedValueLabel = tgui::Label::create(std::to_string(simulation.getTickRate()));
            speedValueLabel->setPosition(10, 60);
            speedValueLabel->getRenderer()->setTextColor(tgui::Color::White);
            panel->add(speedValueLabel);
            speed_slider->onValueChange([speedValueLabel](int value) {
                speedValueLabel->setText(std::to_string(value));
            });
            
            auto percentLabel = tgui::Label::create("Live cells percent (0-100):");
            percentLabel->setPosition(10, 90);
            percentLabel->getRenderer()->setTextColor(tgui::Color::White);
            panel->add(percentLabel);
            
            percent_slider = tgui::Slider::create();
            percent_slider->setPosition(10, 115);
            percent_slider->setSize(380, 20);
            percent_slider->setMinimum(0);
            percent_slider->setMaximum(100);
            percent_slider->setValue(static_cast<int>(simulation.getProcentLifeCells() * 100));
            percent_slider->onValueChange([this](int value) {
                float newPercent = value / 100.0f;
                simulation.setProcentLifeCells(newPercent);
            });
            panel->add(percent_slider);
            
            auto percentValueLabel = tgui::Label::create(std::to_string(static_cast<int>(simulation.getProcentLifeCells() * 100)) + "%");
            percentValueLabel->setPosition(10, 140);
            percentValueLabel->getRenderer()->setTextColor(tgui::Color::White);
            panel->add(percentValueLabel);
            percent_slider->onValueChange([percentValueLabel](int value) {
                percentValueLabel->setText(std::to_string(value) + "%");
            });
            
            auto pauseButton = tgui::Button::create("Pause");
            pauseButton->setPosition(10, 170);
            pauseButton->setSize(100, 30);
            pauseButton->onPress([this]() {
                paused = !paused;
            });
            panel->add(pauseButton);
            
            auto reloadButton = tgui::Button::create("Regenerate");
            reloadButton->setPosition(120, 170);
            reloadButton->setSize(100, 30);
            reloadButton->onPress([this]() {
                simulation.reload();
            });
            panel->add(reloadButton);
            
            auto spawnLabel = tgui::Label::create("Spawn mode:");
            spawnLabel->setPosition(10, 220);
            spawnLabel->getRenderer()->setTextColor(tgui::Color::White);
            panel->add(spawnLabel);
            
            draw_cell_radio = tgui::RadioButton::create();
            draw_cell_radio->setText("Draw cell");
            draw_cell_radio->getRenderer()->setTextColor(tgui::Color::White);
            draw_cell_radio->setPosition(10, 250);
            draw_cell_radio->setChecked(true);
            draw_cell_radio->onCheck([this]() { spawn_glider_mode = false; });
            panel->add(draw_cell_radio);

            spawn_glider_radio = tgui::RadioButton::create();
            spawn_glider_radio->setText("Spawn Glider");
            spawn_glider_radio->getRenderer()->setTextColor(tgui::Color::White);
            spawn_glider_radio->setPosition(10, 280);
            spawn_glider_radio->onCheck([this]() { spawn_glider_mode = true; });
            panel->add(spawn_glider_radio);
        }

        void createRulesTab(tgui::Panel::Ptr panel)
        {
            auto birthLabel = tgui::Label::create("Birth rules (dead cell becomes alive)");
            birthLabel->setPosition(10, 10);
            birthLabel->getRenderer()->setTextColor(tgui::Color::White);
            panel->add(birthLabel);
            
            auto surviveLabel = tgui::Label::create("Survival rules (live cell stays alive)");
            surviveLabel->setPosition(10, 150);
            surviveLabel->getRenderer()->setTextColor(tgui::Color::White);
            panel->add(surviveLabel);
            
            int x_start = 10;
            int y_start_birth = 40;
            int y_start_survive = 180;
            int spacing_x = 45;
            int spacing_y = 30;
            
            for (int i = 0; i < 8; ++i) 
            {
                auto cbBirth = tgui::CheckBox::create(std::to_string(i));
                cbBirth->setPosition(x_start + (i % 4) * spacing_x, y_start_birth + (i / 4) * spacing_y);
                cbBirth->setChecked(simulation.getBirthRules().count(i));
                cbBirth->onChange([this, i](bool checked) {
                    if (checked)
                        simulation.addBirthRule(i);
                    else
                        simulation.removeBirthRule(i);
                });
                cbBirth->getRenderer()->setTextColor(tgui::Color::White);
                panel->add(cbBirth);
                birth_checkboxes.push_back(cbBirth);
                
                auto cbSurvive = tgui::CheckBox::create(std::to_string(i));
                cbSurvive->setPosition(x_start + (i % 4) * spacing_x, y_start_survive + (i / 4) * spacing_y);
                cbSurvive->setChecked(simulation.getSurvivalRules().count(i));
                cbSurvive->onChange([this, i](bool checked) {
                    if (checked)
                        simulation.addSurvivalRule(i);
                    else
                        simulation.removeSurvivalRule(i);
                });
                cbSurvive->getRenderer()->setTextColor(tgui::Color::White);
                panel->add(cbSurvive);
                survival_checkboxes.push_back(cbSurvive);
            }
            
            auto resetButton = tgui::Button::create("Reset to Classic");
            resetButton->setPosition(10, 260);
            resetButton->setSize(150, 25);
            resetButton->onPress([this]() {
                simulation.clearBirthRules();
                simulation.clearSurvivalRules();
                simulation.addBirthRule(3);
                simulation.addSurvivalRule(2);
                simulation.addSurvivalRule(3);
                for (int i = 0; i < 8; ++i) {
                    birth_checkboxes[i]->setChecked(simulation.getBirthRules().count(i));
                    survival_checkboxes[i]->setChecked(simulation.getSurvivalRules().count(i));
                }
            });
            panel->add(resetButton);
        }
        // template <class T>
        // void addButton(T& object, std::string name_btn, int pos_x, int pos_y, int size_w, int size_h)
        // {
        //     auto button = tgui::Button::create(name_btn);
        //     button->setPosition(pos_x, pos_y);
        //     button->setSize(size_w, size_h);

        //     button->onPress([&object](){
        //         object.reload();
        //     });

        //     gui->add(button);
        // }

    tgui::Gui* gui;
    Simulation& simulation;

    bool current_tab = true;
    bool spawn_glider_mode = false;
    bool paused = true;
    
    tgui::Slider::Ptr speed_slider;
    tgui::Slider::Ptr percent_slider;
    tgui::RadioButton::Ptr draw_cell_radio;
    tgui::RadioButton::Ptr spawn_glider_radio;
    
    std::vector<tgui::CheckBox::Ptr> birth_checkboxes;
    std::vector<tgui::CheckBox::Ptr> survival_checkboxes;
};