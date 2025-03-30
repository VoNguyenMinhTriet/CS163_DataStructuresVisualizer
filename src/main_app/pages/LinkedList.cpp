#include "./LinkedList.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include <iostream>

ds_viz::pages::LinkedListPage::LinkedListPage() 
{
    font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
    title = raylib::Text("Singly-Linked List", 128, raylib::Color::White(), *font, 0);
        
    auto returnButton = std::make_unique<raywtk::Button>();
    returnButton->buttonRect = raylib::Rectangle(10, 110, 40, 40); // Position and size
    returnButton->buttonText = "<-";
    returnButton->Click.append([this]() { OnReturnButtonClick(); });
    returnButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    Buttons.push_back(std::move(returnButton)); 

    
}


// Handle return button click event
void ds_viz::pages::LinkedListPage::OnReturnButtonClick()
{
    _context->ChangePage(std::make_shared<ds_viz::pages::MainMenuPage>());
}


void ds_viz::pages::LinkedListPage::Update(float dt)
{
    for (auto& button: Buttons)
        button->Update(dt);
}

void ds_viz::pages::LinkedListPage::Render()
{
    // Draw title
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));

    for (auto& button: Buttons)
    {
        button->Render();
    }

}



