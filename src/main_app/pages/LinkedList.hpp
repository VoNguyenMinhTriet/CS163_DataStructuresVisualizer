#pragma once

#include <memory>
#include <vector>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/graph_widget/node.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "./page.hpp"


namespace ds_viz::pages 
{
    class LinkedListPage : public Page 
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        std::unique_ptr<raywtk::NodeWidget> head = nullptr; // Head node
        int size = 0; // Size of Linked List

        std::unique_ptr<raywtk::Button> insertAtTail;
        std::unique_ptr<raywtk::Button> returnButton;

        bool showInputBar = false;
        std::string inputValue;

        std::string errorMessage = "";
        float errorTimer = 0.0f;

    public:
    
    LinkedListPage(); 
    void OnReturnButtonClick();
    void InsertNode(int value);
    void Update(float dt) override;
    void Render() override;
    };
}

