#pragma once

#include <memory>
#include <vector>
#include <stack>
#include "main_app/main_window.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/graph_widgets/listnode.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "./page.hpp"
#include "main_app/main_window.hpp"


namespace ds_viz::pages 
{
    class LinkedListPage : public Page 
    {
        // Font and title of page
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        
        // For scaling
        const int baseWidth = 1280;  
        const int baseHeight = 720;
        float scaleX; 
        float scaleY;     

        std::unique_ptr<raywtk::ListNodeWidget> head = nullptr; // Head node
        std::shared_ptr<raylib::Font> nodeFont; // Node font
        std::shared_ptr<raylib::Font> textFont; // Text font
        std::shared_ptr<raylib::Font> codeFont;

        float headX = 80; // Default X-coordinate of head
        float headY = 400; // Default Y-coordinate of head
        float spacing = 100; // Default spacing between nodes
        int size = 0; // Size of Linked List
        
        // For undo and redo functions
        std::stack<std::vector<int>> UndoStack; 
        std::stack<std::vector<int>> RedoStack;

        // List of buttons
        std::unique_ptr<raywtk::Button> repositionButton;
        std::unique_ptr<raywtk::Button> undoButton;
        std::unique_ptr<raywtk::Button> redoButton;
        std::unique_ptr<raywtk::Button> returnButton;
        std::unique_ptr<raywtk::Button> actionBarButton;

        bool actionBarVisible = false; 
        std::unique_ptr<raywtk::Button> insertButton;
        std::vector<std::unique_ptr<raywtk::Button>> insertDropdownButtons;
        std::vector<std::string> insertOptions = { "At Head", "At Tail", "At Index" };
        bool insertDropdownOpen = false;

        std::unique_ptr<raywtk::Button> deleteButton;
        std::vector<std::unique_ptr<raywtk::Button>> deleteDropdownButtons;
        std::vector<std::string> deleteOptions = { "At Head", "At Tail", "At Index" };
        bool deleteDropdownOpen = false;

        std::unique_ptr<raywtk::Button> searchButton;
        std::vector<std::unique_ptr<raywtk::Button>> searchDropdownButtons;
        std::vector<std::string> searchOptions = { "By Value", "By Index" };
        bool searchDropdownOpen = false;

        std::unique_ptr<raywtk::Button> createButton;
        std::vector<std::unique_ptr<raywtk::Button>> createDropdownButtons;
        std::vector<std::string> createOptions = { "Random", "Clear", "Load file" };
        bool createDropdownOpen = false;
        
        
        // Step-by-step running buttons
        std::unique_ptr<raywtk::Button> stepForwardButton;
        std::unique_ptr<raywtk::Button> stepBackwardButton;
        
        struct ListState 
        {
            std::vector<int> values; // Node values
            std::vector<raylib::Color> colors; // Node colors
            int currentStep; // Current step in the pseudo-code

            bool hasNewNode = false; // Check if a new node exists in the state       
            int newNodeValue = 0;              
            raylib::Color newNodeColor;        
            raylib::Vector2 newNodePos;  
        };
        
        std::vector<ListState> animationStates; 
        int currentAnimationState = -1; 
        
        // pseudo-code box for each operation
        std::unique_ptr<raywtk::Button> pseudoToggleButton;
        std::vector<std::string> pseudoCodeSteps; 
        int currentStep = -1;                    
        bool showPseudoCode = false;    
        
        // For image buttons texture
        raylib::Texture undoButtonTex;
        raylib::Texture redoButtonTex;
        raylib::Texture returnButtonTex;
        raylib::Texture stepforwardTex;
        raylib::Texture stepbackTex;
        
        // flags to show input bars
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

        bool showDeleteAtIndexInput = false;
        std::string inputDeleteIndex;

        bool showSearchInput = false;
        bool searchByValue = true; // Toggle between value and index search
        std::string searchByValInput;
        std::string searchByIndInput;

        float animatingTimer = 0.0f; // Timer for animations

        // Insert Animation
        bool animatingInsert = false;
        raywtk::ListNodeWidget* insertCurrent = nullptr;
        int insertIndex = -1;
        int insertValue = -1000;
        int currentInsertIndex = 0;
        int insertState = 0;
        std::unique_ptr<raywtk::ListNodeWidget> newNode = nullptr; //node for insertion animation
        bool IAH = false;
        bool IAT = false;
        bool IAI = false;

        // Delete Animation
        bool animatingDelete = false;
        raywtk::ListNodeWidget* deleteCurrent = nullptr;
        raywtk::ListNodeWidget* ToDelPrev = nullptr;
        raywtk::ListNodeWidget* deleteShift = nullptr;
        int deleteIndex = -1;
        int currentdeleteIndex = 0;
        int deleteState = 0;
        bool DAH = false;
        bool DAT = false;
        bool DAI = false;

        // Search Animation
        bool animatingSearch = false;
        raywtk::ListNodeWidget* searchCurrent = nullptr;
        int searchTarget = -1000;
        int searchIndex = -1;
        int currentSearchIndex = 0;
        int searchState = 0;

        // Speed bar
        float speedBarX = 100;  
        float speedBarY = 50;
        float speedBarWidth = 300;
        float speedBarHeight = 10;
        float speedKnobRadius = 8;

        float speedMin = 0.25f;  // Minimum speed factor
        float speedMax = 2.0f;  // Maximum speed factor
        float speedKnobX = speedBarX;  // Start at the lowest speed
        bool isDraggingSpeedKnob = false;

        float animationSpeed = 1.0f;  // Default speed (1.0x)
        std::vector<float> speedSteps = {0.25f, 0.5f, 1.0f, 1.5f, 2.0f};

        // Messages and notifications
        std::string errorMessage = "";
        float errorTimer = 0.0f;
        std::string DropMessage = "";
        float DropTimer = 0.0f;
        std::string FindMessage = "";
        float FindTimer = 0.0f;

                 
        
                                    
    public:

    LinkedListPage(ds_viz::MainWindow &context); 
    ~LinkedListPage();

    void OnRandomButtonClick(int numNodes);
    void InsertRandom(int value);
    void OnLoadFileButtonClick();
    void OnClearButtonClick();
    void ResetColor();
    void RepositionNodes();
    void GetListState(std::vector<int> &ListState);
    void Append();
    void OnUndoButtonClick();
    void OnRedoButtonClick();
    void OnReturnButtonClick();
    void InsertAtHead(int value);
    void InsertAtTail(int value);
    void InsertAtIndex(int value, int index); 
    void DeleteAtHead();
    void DeleteAtTail();
    void DeleteAtIndex(int index);
    void SearchByValue(int value);
    void SearchByIndex(int index);
    void AnimateInsert(float dt);
    void AnimateDelete(float dt);
    void AnimateSearch(float dt);
    void OnStepForwardClick();
    void OnStepBackwardClick();
    void SaveListState();
    void LoadListState(const ListState& state);
    void CreateNotification(std::string &Message);
    void DrawInputBox(int X, int Y, int width, int height, std::string &input);
    void DrawSpeedBar();
    void SetPseudoCodeSteps(const std::vector<std::string>& steps); 
    void DrawPseudoCodeBlock();  
    void Update(float dt) override;
    void Render() override;
    };
}
