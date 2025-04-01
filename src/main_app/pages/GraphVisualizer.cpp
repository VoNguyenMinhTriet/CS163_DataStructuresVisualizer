#include "./GraphVisualizer.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>

using namespace ds_viz::pages;

GraphVisualizer::GraphVisualizer() 
{
    font = std::make_unique<raylib::Font>("./ttf/InterDisplay-Black.ttf", 128, nullptr, 250);
    title = raylib::Text("Graph Visualizer", 64, raylib::Color::White(), *font, 0);
}

void GraphVisualizer::Update(float dt) 
{
    // Placeholder update logic
}

void GraphVisualizer::Render() 
{
    title.Draw(200, 100);
}