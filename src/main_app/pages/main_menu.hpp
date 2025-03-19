#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "./page.hpp"
#include <memory>
#include <raylib.h>

namespace ds_viz::pages
{
    class MainMenuPage : public Page
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

        public:

        MainMenuPage () {
            font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
            title = raylib::Text("DATA LA VISTA", 128, raylib::Color::White(), *font, 0);
        }

        void Update (float dt) override
        {

        }

        void Render () override;
    };
}
