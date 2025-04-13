#include "main_app/main_window.hpp"
#include "./main_menu.hpp"

void ds_viz::pages::MainMenuPage::Render ()
{
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));

    _trieButton.Render();
}

ds_viz::pages::MainMenuPage::MainMenuPage(ds_viz::MainWindow &context)
    : Page(context)
{
    font = std::unique_ptr<raylib::Font>(
        new raylib::Font("./ttf/InterDisplay-Black.ttf", 128));
    title =
        raylib::Text("DATA LA VISTA", 128, raylib::Color::White(), *font, 0);

    _trieButton.buttonRect = raylib::Rectangle {128, 128, 128, 64};
    _trieButton.Click.append([this]() {
        _context->ChangePage(std::make_shared<TriePage>(*_context));
    });
}
void ds_viz::pages::MainMenuPage::Update(float dt)
{
    _trieButton.Update(dt);
}
