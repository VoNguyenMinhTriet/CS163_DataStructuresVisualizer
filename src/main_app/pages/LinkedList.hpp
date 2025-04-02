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
        std::unique_ptr<raywtk::Button> insertAtHeadButton;
        std::unique_ptr<raywtk::Button> insertAtTailButton;
        std::unique_ptr<raywtk::Button> insertAtIndexButton;
        std::unique_ptr<raywtk::Button> returnButton;
        std::unique_ptr<raywtk::Button> randomButton;
        std::unique_ptr<raywtk::Button> clearAllButton;
        std::unique_ptr<raywtk::Button> repositionButton;
        std::unique_ptr<raywtk::Button> loadFileButton;
        std::unique_ptr<raywtk::Button> searchByValueButton;
        std::unique_ptr<raywtk::Button> searchByIndexButton;

        bool showInsertAtHead = false;
        bool showInsertAtTail = false;
        bool showInsertAtIndexInput  = false;
        std::string inputValue; // For value inputs
        std::string inputIndex; // For index inputs
        bool isEnteringValue = true;

        bool showRandomInput = false;  

        bool showSearchInput = false;
        bool searchByValue = true; // Toggle between value and index search
        std::string searchInput;

        // Insert Animation
        bool animatingInsert = false;
        raywtk::NodeWidget* insertCurrent = nullptr;
        int insertIndex = -1;
        int insertValue = -1000;
        int currentInsertIndex = 0;
        float insertTimer = 0.0f;
        int insertState = 0;

        // Search Animation
        bool animatingSearch = false;
        raywtk::NodeWidget* searchCurrent = nullptr;
        int searchTarget = -1000;
        int searchIndex = -1;
        int currentSearchIndex = 0;
        float searchTimer = 0.0f;
        int searchState = 0;

        // Messages and notifications
        std::string errorMessage = "";
        float errorTimer = 0.0f;
        std::string DropMessage = "";
        float DropTimer = 0.0f;
        std::string FindMessage = "";
        float FindTimer = 0.0f;


    public:
    
    LinkedListPage(); 
    void OnReturnButtonClick();
    void OnRandomButtonClick(int numNodes);
    void InsertRandom(int value);
    void OnClearButtonClick();
    void OnLoadFileButtonClick();
    void ResetColor();
    void RepositionNodes();
    void InsertAtHead(int value);
    void InsertAtTail(int value);
    void InsertAtIndex(int value, int index); 
    void SearchByValue(int value);
    void SearchByIndex(int index);
    void CreateNotification(std::string &Message);
    void DrawInputBox(int X, int Y, std::string &input);
    void AnimateSearch(float dt);
    void AnimateInsert(float dt);
    void Update(float dt) override;
    void Render() override;
    };
}

