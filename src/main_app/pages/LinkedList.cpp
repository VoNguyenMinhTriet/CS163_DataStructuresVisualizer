#include "LinkedList.hpp"
#include "main_app/main_window.hpp"

void ds_viz::pages::LinkedListPage::Render() {
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));
}



