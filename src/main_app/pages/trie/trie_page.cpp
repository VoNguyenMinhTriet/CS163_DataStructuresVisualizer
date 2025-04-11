#include "./trie_page.hpp"
#include "main_app/pages/page.hpp"

ds_viz::pages::TriePage::TriePage(MainWindow &context) : Page(context)
{
    _toggleShowActionsButton.buttonRect =
        raylib::Rectangle(-8, _context->ref_raylib_window->GetHeight() - 172, 16, 136);
    _toggleShowActionsButton.Click.append([this] {
        ToggleShowActions();
    });
    
    _initializeButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 172, 128, 24);
    _initializeButton.Click.append([this] {
        Initialize();
    });
    
    _addButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 144, 128, 24);

    _removeButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 116, 128, 24);

    _searchButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 88, 128, 24);

    _clearButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 60, 128, 24);
}

void ds_viz::pages::TriePage::Render()
{
    _scene.Render();
    _toggleShowActionsButton.Render();
    
    if (actionsShown) {
        _initializeButton.Render();
        _addButton.Render();
        _removeButton.Render();
        _searchButton.Render();
        _clearButton.Render();
    }
    // _showCodeBoxButton.Render();
}
void ds_viz::pages::TriePage::Update(float deltaTime) 
{
    _scene.cam.SetOffset({_context->ref_raylib_window->GetWidth() / 2.0f,
        _context->ref_raylib_window->GetHeight() / 2.0f});
        
    _toggleShowActionsButton.buttonRect =
        raylib::Rectangle(-8, _context->ref_raylib_window->GetHeight() - 172, 16, 136);

    _initializeButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 172, 128, 24);
    
    _addButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 144, 128, 24);

    _removeButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 116, 128, 24);

    _searchButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 88, 128, 24);

    _clearButton.buttonRect =
        raylib::Rectangle(12, _context->ref_raylib_window->GetHeight() - 60, 128, 24);
    
    _toggleShowActionsButton.Update(deltaTime);
    if (actionsShown) {
        _initializeButton.Update(deltaTime);
        _addButton.Update(deltaTime);
        _removeButton.Update(deltaTime);
        _searchButton.Update(deltaTime);
        _clearButton.Update(deltaTime);
    }
    _scene.Update(deltaTime);
}
