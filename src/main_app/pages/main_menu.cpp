
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"

void ds_viz::pages::MainMenuPage::Render ()
{
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));

    _trieButton.Render();
    _graphButton.Render();
    _heapButton.Render();
    _linkedListButton.Render();
}

ds_viz::pages::MainMenuPage::MainMenuPage(ds_viz::MainWindow &context) : Page(context)
{
    font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128));
    title = raylib::Text("DATA LA VISTA", 128, raylib::Color::White(), *font, 0);

    _trieButton.buttonRect = raylib::Rectangle {250, 250, 300, 100};
    _graphButton.buttonRect = raylib::Rectangle {750, 250, 300, 100};
    _heapButton.buttonRect = raylib::Rectangle {250, 450, 300, 100};
    _linkedListButton.buttonRect = raylib::Rectangle {750, 450, 300, 100};

    _trieButton.buttonText = "Trie";
    _graphButton.buttonText = "Graph";
    _heapButton.buttonText = "Heap";
    _linkedListButton.buttonText = "Linked List";

    _trieButton.Click.append([this]() {
        _deferredStateChange = [this]() {
            //_context->ChangePage(std::make_shared<TriePage>(*_context));
        };
    });
    _graphButton.Click.append([this]() {
        _deferredStateChange = [this]() {
            _context->ChangePage(std::make_shared<GraphVisualizer>(*_context));
        };
    });
    _heapButton.Click.append([this]() {
        _deferredStateChange = [this]() {
            _context->ChangePage(std::make_shared<HeapVisualizer>(*_context));
        };
    });
    _linkedListButton.Click.append([this]() {
        _deferredStateChange = [this]() {
            _context->ChangePage(std::make_shared<LinkedListPage>(*_context));
        };
    });
}
void ds_viz::pages::MainMenuPage::Update(float dt)
{
    // Very ugly workaround for heap-use-after-free bug where after the page was switched, the next lines would continue running.
    _trieButton.Update(dt);
    _graphButton.Update(dt);
    _heapButton.Update(dt);
    _linkedListButton.Update(dt);

    if (_deferredStateChange) _deferredStateChange();
}




