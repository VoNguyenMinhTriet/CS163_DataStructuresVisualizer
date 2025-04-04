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
        std::shared_ptr<raylib::Font> sharedFont = std::make_shared<raylib::Font>(LoadFont("./ttf/InterDisplay-ExtraBold.ttf")); // Node font
        float headX = 80; // Default X-coordinate of head
        float headY = 400; // Default Y-coordinate of head
        float spacing = 100; // Default spacing between nodes
        int size = 0; // Size of Linked List

        // List of buttons
        std::unique_ptr<raywtk::Button> insertAtHeadButton;
        std::unique_ptr<raywtk::Button> insertAtTailButton;
        std::unique_ptr<raywtk::Button> insertAtIndexButton;
        std::unique_ptr<raywtk::Button> deleteAtHeadButton;
        std::unique_ptr<raywtk::Button> deleteAtTailButton;
        std::unique_ptr<raywtk::Button> deleteAtIndexButton;
        std::unique_ptr<raywtk::Button> returnButton;
        std::unique_ptr<raywtk::Button> randomButton;
        std::unique_ptr<raywtk::Button> clearAllButton;
        std::unique_ptr<raywtk::Button> repositionButton;
        std::unique_ptr<raywtk::Button> loadFileButton;
        std::unique_ptr<raywtk::Button> searchByValueButton;
        std::unique_ptr<raywtk::Button> searchByIndexButton;

        bool showInsertAtHead = false;
        std::string inputInsertAtHead;

        bool showInsertAtTail = false;
        std::string inputInsertAtTail;

        bool showInsertAtIndexInput  = false;
        std::string inputValue; // For value inputs
        std::string inputIndex; // For index inputs
        bool isEnteringValue = true;

        bool showRandomInput = false;  
        std::string inputRandom;

        bool showDeleteAtHead = false;
        bool showDeleteAtTail = false;

        bool showDeleteAtIndexInput = false;
        std::string inputDeleteIndex;

        bool showSearchInput = false;
        bool searchByValue = true; // Toggle between value and index search
        std::string searchByValInput;
        std::string searchByIndInput;

        float animatingTimer = 0.0f; // Timer for animations

        // Insert Animation
        bool animatingInsert = false;
        raywtk::NodeWidget* insertCurrent = nullptr;
        int insertIndex = -1;
        int insertValue = -1000;
        int currentInsertIndex = 0;
        int insertState = 0;

        // Delete Animation
        bool animatingDelete = false;
        raywtk::NodeWidget* deleteCurrent = nullptr;
        raywtk::NodeWidget* ToDelPrev = nullptr;
        raywtk::NodeWidget* deleteShift = nullptr;
        int deleteIndex = -1;
        int currentdeleteIndex = 0;
        int deleteState = 0;

        // Search Animation
        bool animatingSearch = false;
        raywtk::NodeWidget* searchCurrent = nullptr;
        int searchTarget = -1000;
        int searchIndex = -1;
        int currentSearchIndex = 0;
        int searchState = 0;

        // Speed bar
        float speedBarX = 100;  // Bar start position
        float speedBarY = 50;
        float speedBarWidth = 300;
        float speedBarHeight = 10;
        float speedKnobRadius = 8;

        float speedMin = 0.25f;  // Minimum speed factor
        float speedMax = 2.0f;  // Maximum speed factor
        float speedKnobX = speedBarX;  // Start at the lowest speed
        bool isDraggingSpeedKnob = false;

        // Current speed value
        float animationSpeed = 1.0f;  // Default speed (1x)
        std::vector<float> speedSteps = {0.25f, 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 1.75f, 2.0f};

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
    void DeleteAtHead();
    void DeleteAtTail();
    void DeleteAtIndex(int index);
    void SearchByValue(int value);
    void SearchByIndex(int index);
    void CreateNotification(std::string &Message);
    void DrawInputBox(int X, int Y, std::string &input);
    void DrawSpeedBar();
    void AnimateSearch(float dt);
    void AnimateInsert(float dt);
    void AnimateDelete(float dt);
    void Update(float dt) override;
    void Render() override;
    };
}

