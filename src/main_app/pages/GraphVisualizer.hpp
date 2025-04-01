#pragma once

#include <memory>
#include <vector>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/graph_widgets/node.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "./page.hpp"


namespace ds_viz::pages 
{
    class GraphVisualizer : public Page
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        
        public:
            GraphVisualizer();
            void Update(float dt) override;
            void Render() override;
    };
}