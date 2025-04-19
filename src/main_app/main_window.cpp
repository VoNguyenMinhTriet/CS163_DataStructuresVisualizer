// Implementation splitting here is merely a workaround.

#include "main_window.hpp"
#include "./pages/main_menu.hpp"
#include "./themes/dark_simple/dark_simple.hpp"
#include "main_app/pages/page.hpp"
#include <memory>

void ds_viz::MainWindow::Render ()
{
    ref_raylib_window->BeginDrawing();

    if (style) style->RenderWithStyle(this);
    else raywtk::ThemeManager::GetCurrentTheme().GetStyle(typeid(MainWindow).name()).RenderWithStyle(this);
    if (content) content->Render();

    ref_raylib_window->EndDrawing();
}

void ds_viz::MainWindow::ChangePage (std::shared_ptr<Page> to)
{
    content = to;
    to->SetContext(this);
}

int ds_viz::MainWindow::Run ()
{
    ref_raylib_window = std::make_unique<raylib::Window>(initScreenWidth, initScreenHeight, title, raylibFlags);
    ref_raylib_window->SetTargetFPS(fixedFPS);
    ref_raylib_window->SetMinSize(1280, 720);
    
    raywtk::ThemeManager::currentTheme = std::unique_ptr<raywtk::Theme>(ds_viz::themes::dark_simple::DarkSimpleTheme());

    ChangePage(std::make_shared<pages::MainMenuPage>(*this));

    while (!ref_raylib_window->ShouldClose()) // Detect window close button or ESC key
    {
        if (content) content->Update(GetFrameTime());
        Render();
    }

    return 0;
}
