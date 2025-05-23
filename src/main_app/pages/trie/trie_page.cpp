#include "./trie_page.hpp"

#include "main_app/pages/page.hpp"
#include "raylib-cpp/Color.hpp"
#include "raylib-cpp/Image.hpp"
#include "raylib-cpp/Keyboard.hpp"
#include "raylib-cpp/Rectangle.hpp"
#include "raylib-cpp/Texture.hpp"
#include "raylib-cpp/TextureUnmanaged.hpp"
#include "raylib-cpp/Vector2.hpp"

#include <raylib.h>
#include <algorithm>
#include <sstream>
#include <string>

ds_viz::pages::TriePage::TriePage(MainWindow& context) : Page(context)
{
    titleFont = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128));
    title = raylib::Text("Trie", 128, raylib::Color(255, 255, 255, 48), *titleFont, 0);

    _toggleShowActionsButton.Click.append([this] { ToggleShowActions(); });
    _showCodeBoxButton.Click.append([this] { _codeBoxVisible = !_codeBoxVisible; });
    _initializeButton.Click.append([this] { InitializeRandomly(); });
    _searchButton.Click.append([this] { _searchBoxVisible = !_searchBoxVisible; });
    _addButton.Click.append([this] { _addBoxVisible = !_addBoxVisible; });
    _removeButton.Click.append([this] { _removeBoxVisible = !_removeBoxVisible; });
    _prevStepButton.Click.append([this] { _scene.StepBackward(); });
    _nextStepButton.Click.append([this] { _scene.StepForward(); });
    _playPauseButton.Click.append([this] { _isPlaying = !_isPlaying; });
    _clearButton.Click.append([this] {
        _scene.ClearTimeline();
        _scene.root = std::make_unique<trie::TrieNode>();
    });
    _homeButton.Click.append([this] { GoBackHome(); });
    std::stringstream outSs; outSs << stepsPerSecond;
    _stepsPerSecTextBox.text = outSs.str();
    Update(0);
}

void ds_viz::pages::TriePage::Render()
{
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));

    _scene.Render();

    //_homeButton.Render();
    homeIcon.Draw(raylib::Rectangle(0, 0, homeIcon.GetWidth(), homeIcon.GetHeight()),
                     _homeButton.buttonRect);

    _toggleShowActionsButton.Render();
    _prevStepButton.Render();
    RenderTextInCenter(_prevStepButton.buttonRect, *buttonFont, "Step backward", raylib::Color::Black());
    _playPauseButton.Render();
    RenderTextInCenter(_playPauseButton.buttonRect, *buttonFont,
        _isPlaying ? "Pause" : "Play", raylib::Color::Black());
    _nextStepButton.Render();
    RenderTextInCenter(_nextStepButton.buttonRect, *buttonFont, "Step forward", raylib::Color::Black());

    if (actionsShown)
    {
        _initializeButton.Render();
        RenderTextInCenter(_initializeButton.buttonRect, *buttonFont,
                           "Initialize randomly", raylib::Color::Black());
        _searchButton.Render();
        RenderTextInCenter(_searchButton.buttonRect, *buttonFont,
                           "Search string", raylib::Color::Black());
        if (_searchBoxVisible)
            _searchTextBox.Render();
        _addButton.Render();
        RenderTextInCenter(_addButton.buttonRect, *buttonFont, "Add string",
                           raylib::Color::Black());
        if (_addBoxVisible)
            _addTextBox.Render();
        _removeButton.Render();
        RenderTextInCenter(_removeButton.buttonRect, *buttonFont,
                           "Remove string", raylib::Color::Black());
        if (_removeBoxVisible)
            _removeTextBox.Render();
        _clearButton.Render();
        RenderTextInCenter(_clearButton.buttonRect, *buttonFont, "Clear trie",
                           raylib::Color::Black());
    }

    _showCodeBoxButton.Render();
    if (_codeBoxVisible) _codeBox.Render();

    // Caption
    auto txt = raylib::Text(_scene.GetCaption(), buttonFont->GetBaseSize(), raylib::Color::White(), *buttonFont);
    txt.Draw(raylib::Vector2(8,
        _context->ref_raylib_window->GetRenderHeight() - 27));
    // Status msg
    RenderTextOnRight(
        raylib::Vector2(_context->ref_raylib_window->GetWidth() - 8,
                        _context->ref_raylib_window->GetRenderHeight() - 27),
        *buttonFont, _scene.GetStatusMessage(), raylib::Color::White());
    // Progress bar
    auto progress = _scene.Progress();
    auto progressBarWidth =
        progress.second ? 1.0f * _context->ref_raylib_window->GetWidth() * progress.first / progress.second : _context->ref_raylib_window->GetWidth();
    auto progressBar = raylib::Rectangle(
        0, _context->ref_raylib_window->GetHeight() - 4, progressBarWidth, 4);
    progressBar.Draw(raylib::Color::Green());
    //
    _stepsPerSecTextBox.Render();
    RenderTextInCenter(raylib::Rectangle(_context->ref_raylib_window->GetWidth() / 2.0f + 4, _context->ref_raylib_window->GetHeight() - 60, 42, 24),
        *buttonFont, "steps/s", raylib::Color::White());
}

void ds_viz::pages::TriePage::Update(float deltaTime)
{
    LoadFileIfDropped();

    if ((!_searchBoxVisible || !_searchTextBox.focused) && (!_addBoxVisible || !_addTextBox.focused) &&
        (!_removeBoxVisible || !_removeTextBox.focused) && (!_stepsPerSecTextBox.focused)) // Very ugly workaround, have to rework focusing behaviour
    {
        if (raylib::Keyboard::IsKeyPressed(KEY_RIGHT) ||
            raylib::Keyboard::IsKeyPressedRepeat(KEY_RIGHT))
            _scene.StepForward();
        if (raylib::Keyboard::IsKeyPressed(KEY_LEFT) ||
            raylib::Keyboard::IsKeyPressedRepeat(KEY_LEFT))
            _scene.StepBackward();
    }

    _scene.cam.SetOffset({_context->ref_raylib_window->GetWidth() / 2.0f,
                          _context->ref_raylib_window->GetHeight() / 2.0f});

    _toggleShowActionsButton.buttonRect = raylib::Rectangle(
        -8, _context->ref_raylib_window->GetHeight() - 172, 16, 136);

    _initializeButton.buttonRect = raylib::Rectangle(
        12, _context->ref_raylib_window->GetHeight() - 172, 128, 24);

    _searchButton.buttonRect = raylib::Rectangle(
        12, _context->ref_raylib_window->GetHeight() - 144, 128, 24);
    _searchTextBox.textBoxRect = raylib::Rectangle(
        144, _context->ref_raylib_window->GetHeight() - 144, 192, 24);

    _addButton.buttonRect = raylib::Rectangle(
        12, _context->ref_raylib_window->GetHeight() - 116, 128, 24);
    _addTextBox.textBoxRect = raylib::Rectangle(
        144, _context->ref_raylib_window->GetHeight() - 116, 192, 24);

    _removeButton.buttonRect = raylib::Rectangle(
        12, _context->ref_raylib_window->GetHeight() - 88, 128, 24);
    _removeTextBox.textBoxRect = raylib::Rectangle(
        144, _context->ref_raylib_window->GetHeight() - 88, 192, 24);

    _clearButton.buttonRect = raylib::Rectangle(
        12, _context->ref_raylib_window->GetHeight() - 60, 128, 24);

    _prevStepButton.buttonRect = raylib::Rectangle(
        _context->ref_raylib_window->GetWidth() / 2.0 - 128,
        _context->ref_raylib_window->GetHeight() - 32, 100, 24);
    _playPauseButton.buttonRect = raylib::Rectangle(
        _context->ref_raylib_window->GetWidth() / 2.0 - 24,
        _context->ref_raylib_window->GetHeight() - 32, 48, 24);
    _nextStepButton.buttonRect = raylib::Rectangle(
        _context->ref_raylib_window->GetWidth() / 2.0 + 28,
        _context->ref_raylib_window->GetHeight() - 32, 100, 24);
    _stepsPerSecTextBox.textBoxRect = raylib::Rectangle(
        _context->ref_raylib_window->GetWidth() / 2.0 - 46,
        _context->ref_raylib_window->GetHeight() - 60, 46, 24);

    _showCodeBoxButton.buttonRect = raylib::Rectangle(
        _context->ref_raylib_window->GetWidth() - 8, _context->ref_raylib_window->GetHeight() - 172, 16, 136);
    _codeBox.boxRect = raylib::Rectangle(_context->ref_raylib_window->GetWidth() - 268,_context->ref_raylib_window->GetHeight() - 172,256,136);
    _codeBox.code = _scene.GetCode();
    _codeBox.highlightedLine = _scene.GetCurrentLine();

    _homeButton.buttonRect = raylib::Rectangle(4, 4, 28, 28);

    _homeButton.Update(deltaTime);
    _toggleShowActionsButton.Update(deltaTime);
    _showCodeBoxButton.Update(deltaTime);
    _prevStepButton.Update(deltaTime);
    _playPauseButton.Update(deltaTime);
    _nextStepButton.Update(deltaTime);

    if (actionsShown)
    {
        _initializeButton.Update(deltaTime);
        _searchButton.Update(deltaTime);
        if (_searchBoxVisible)
        {
            if (_searchTextBox.focused &&
                raylib::Keyboard::IsKeyPressed(KEY_ENTER) &&
                _searchTextBox.text.size() > 0)
            {
                if (raylib::Keyboard::IsKeyDown(KEY_LEFT_SHIFT) ||
                    raylib::Keyboard::IsKeyDown(KEY_RIGHT_SHIFT))
                {
                    _scene.root->Search(_searchTextBox.text);
                    _searchTextBox.text = "";
                }
                else
                {
                    _isPlaying = true;
                    _scene.BuildSearchTimeline(_searchTextBox.text);
                    _searchTextBox.text = "";
                    _searchBoxVisible = false;
                }
            }

            _searchTextBox.Update(deltaTime);
        }

        _addButton.Update(deltaTime);
        if (_addBoxVisible)
        {
            if (_addTextBox.focused &&
                raylib::Keyboard::IsKeyPressed(KEY_ENTER) &&
                _addTextBox.text.size())
            {
                if (raylib::Keyboard::IsKeyDown(KEY_LEFT_SHIFT) ||
                    raylib::Keyboard::IsKeyDown(KEY_RIGHT_SHIFT))
                {
                    _scene.root->Add(_addTextBox.text);
                    _addTextBox.text = "";
                }
                else
                {
                    _isPlaying = true;
                    _scene.BuildAddTimeline(_addTextBox.text);
                    _addTextBox.text = "";
                    _addBoxVisible = false;
                }
            }
            _addTextBox.Update(deltaTime);
        }

        _removeButton.Update(deltaTime);
        if (_removeBoxVisible)
        {
            if (_removeTextBox.focused &&
                raylib::Keyboard::IsKeyPressed(KEY_ENTER) &&
                _removeTextBox.text.size())
            {
                if (raylib::Keyboard::IsKeyDown(KEY_LEFT_SHIFT) ||
                    raylib::Keyboard::IsKeyDown(KEY_RIGHT_SHIFT))
                {
                    _scene.root->Remove(_removeTextBox.text);
                    _removeTextBox.text = "";
                }
                else
                {
                    _isPlaying = true;
                    _scene.BuildRemoveTimeline(_removeTextBox.text);
                    _removeTextBox.text = "";
                    _removeBoxVisible = false;
                }
            }
            _removeTextBox.Update(deltaTime);
        }

        _clearButton.Update(deltaTime);
    }

    // Steps per second control
    _stepsPerSecTextBox.Update(deltaTime);
    std::stringstream ss(_stepsPerSecTextBox.text);
    float parsedStepsPerSec; char eofTest;

    if (_stepsPerSecTextBox.focused &&
        raylib::Keyboard::IsKeyPressed(KEY_ENTER) &&
        _stepsPerSecTextBox.text.size())
    {
        if (ss >> parsedStepsPerSec && !(ss >> eofTest))
            stepsPerSecond = std::clamp(parsedStepsPerSec, 1.5f, 10.0f);

        std::stringstream outSs; outSs << stepsPerSecond;
        _stepsPerSecTextBox.text = outSs.str();
        _stepsPerSecTextBox.focused = false;
    }

    // Playing mechanism
    if (_isPlaying)
    {
        if (_scene.Progress().first == _scene.Progress().second)
            _isPlaying = false;
        else 
        {
            _currentTime += deltaTime;

            while (_currentTime > 1 / stepsPerSecond)
            {
                _scene.StepForward();
                _currentTime -= 1 / stepsPerSecond;
            }
        }
    }
    else
        _currentTime = 0;

    _scene.Update(deltaTime);
}

void ds_viz::pages::TriePage::GoBackHome()
{
    _context->ChangePage(
        std::make_shared<ds_viz::pages::MainMenuPage>(*_context));
}
