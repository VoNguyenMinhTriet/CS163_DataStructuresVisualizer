#include "main_app/main_window.hpp"
#include "./main_menu.hpp"

void ds_viz::pages::MainMenuPage::Render ()
{
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));

}
