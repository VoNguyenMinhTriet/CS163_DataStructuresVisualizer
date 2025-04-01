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
        float headX = 100; // Default X-coordinate of head
        float headY = 400; // Default Y-coordinate of head
        float spacing = 100; // Default spacing between nodes
        int size = 0; // Size of Linked List

        // List of buttons
        std::unique_ptr<raywtk::Button> insertAtTail;
        std::unique_ptr<raywtk::Button> returnButton;
        std::unique_ptr<raywtk::Button> randomButton;
        std::unique_ptr<raywtk::Button> clearAllButton;
        std::unique_ptr<raywtk::Button> repositionButton;
        std::unique_ptr<raywtk::Button> loadFileButton;
        std::unique_ptr<raywtk::Button> searchByValueButton;
        std::unique_ptr<raywtk::Button> searchByIndexButton;


        bool showInputBar = false;
        std::string inputValue;

        bool showRandomInput = false;  
        std::string randomInputValue;

        bool showSearchInput = false;
        bool searchByValue = true; // Toggle between value and index search
        std::string searchInput;

        bool animatingSearch = false;
        raywtk::NodeWidget* searchCurrent = nullptr;
        bool found;
        int searchTarget = -1000;
        int searchIndex = -1;
        int currentIndex = 0;
        float searchTimer = 0.0f;

        std::string errorMessage = "";
        float errorTimer = 0.0f;


    public:
    
    LinkedListPage(); 
    void OnReturnButtonClick();
    void OnRandomButtonClick();
    void OnClearButtonClick();
    void InsertAtTail(int value); // Eventually become InsertRandom since only 1 frame
    void SearchByValue(int value);
    void SearchByIndex(int index);
    void ResetColor();
    void RepositionNodes();
    void OnLoadFileButtonClick();
    void Update(float dt) override;
    void Render() override;
    };
}

