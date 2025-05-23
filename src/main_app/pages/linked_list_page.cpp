#include "./linked_list_page.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/image_button.hpp"
#include "main_app/main_window.hpp"
#include "./page.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib> // For rand()
#include <ctime>

ds_viz::pages::LinkedListPage::LinkedListPage(ds_viz::MainWindow &context) : ds_viz::Page(context) 
{
    font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
    title = raylib::Text("Linked List", 60, raylib::Color(255, 255, 255, 128), *font, 0);
    
    nodeFont = std::make_shared<raylib::Font>("./ttf/InterDisplay-ExtraBold.ttf", 128);
    textFont = std::make_shared<raylib::Font>("./ttf/Inter-Regular.ttf", 18);
    codeFont = std::make_shared<raylib::Font>("./ttf/Cascadia.ttf", 18);  

    // repositioning nodes button
    repositionButton = std::make_unique<raywtk::Button>();
    repositionButton->buttonText = "Reposition";
    repositionButton->buttonRect = raylib::Rectangle(12, 660, 128, 24);
    repositionButton->Click.append([this]() { RepositionNodes(); });
    repositionButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // input from keyboard button
    inputButton = std::make_unique<raywtk::Button>();
    inputButton->buttonText = "Input values";
    inputButton->buttonRect = raylib::Rectangle(12, 520, 128, 24);
    inputButton->Click.append([this]() { 
        showInputValues = true;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput = false;
        showRandomInput = false;
        insertDropdownOpen = false;
        deleteDropdownOpen = false;
        searchDropdownOpen = false;
        createDropdownOpen = false;
    });
    inputButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // action bar toggle
    actionBarButton = std::make_unique<raywtk::Button>();
    actionBarButton->buttonText = ">";
    actionBarButton->buttonRect = raylib::Rectangle(0, 524, 8, 160); 
    actionBarButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);
    actionBarButton->Click.append([this]() { 
        actionBarVisible = !actionBarVisible; // Toggle visibility
        actionBarButton->buttonText = actionBarVisible ? "<" : ">";
        insertDropdownOpen = false;
        deleteDropdownOpen = false; 
        searchDropdownOpen = false;
        createDropdownOpen = false;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
        showInputValues = false;
    });

    // Pseudo-code toggle button
    pseudoToggleButton = std::make_unique<raywtk::Button>();
    pseudoToggleButton->buttonText = "<";
    pseudoToggleButton->buttonRect = raylib::Rectangle(1265, 393, 8, 280);
    pseudoToggleButton->Click.append([this]() {
        showPseudoCode = !showPseudoCode; // Toggle pseudo-code visibility
        pseudoToggleButton->buttonText = showPseudoCode ? "<" : ">";
    });
    pseudoToggleButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // Insert buttons (Main button)
    insertButton = std::make_unique<raywtk::Button>();
    insertButton->buttonText = "Insert";
    insertButton->buttonRect = raylib::Rectangle(12, 548, 128, 24);
    insertButton->Click.append([this]() { 
        insertDropdownOpen = !insertDropdownOpen;
        deleteDropdownOpen = false; 
        searchDropdownOpen = false;
        createDropdownOpen = false;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
        showInputValues = false;
    });
    insertButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // Dropdown insert buttons
    for (size_t i = 0; i < insertOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = insertOptions[i];
        btn->buttonRect = raylib::Rectangle((12 + (i + 1) * 128), 548, 128, 24);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

        if (insertOptions[i] == "At Head")
        {
            btn->Click.append([this]() {
                showInsertAtHead = true;
                showInsertAtTail = false;
                showInsertAtIndexInput = false;
                showInputValues = false;
            });
        }

        else if (insertOptions[i] == "At Tail")
        {
            btn->Click.append([this]() {
                showInsertAtHead = false;
                showInsertAtTail = true;
                showInsertAtIndexInput = false;
                showInputValues = false;
            });
        }

        else if (insertOptions[i] == "At Index")
        {
            btn->Click.append([this]() {
                showInsertAtHead = false;
                showInsertAtTail = false;
                showInsertAtIndexInput = true;
                showInputValues = false;
            });
        }

        insertDropdownButtons.push_back(std::move(btn));
    }

    // delete buttons (Main button)
    deleteButton = std::make_unique<raywtk::Button>();
    deleteButton->buttonText = "Delete";
    deleteButton->buttonRect = raylib::Rectangle(12, 576, 128, 24);
    deleteButton->Click.append([this]() { 
        insertDropdownOpen = false;
        deleteDropdownOpen = !deleteDropdownOpen; 
        searchDropdownOpen = false;
        createDropdownOpen = false;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
        showInputValues = false;
    });
    deleteButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // Dropdown buttons
    for (size_t i = 0; i < deleteOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = deleteOptions[i];
        btn->buttonRect = raylib::Rectangle((12 + (i + 1) * 128), 576 , 128, 24);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

        if (deleteOptions[i] == "At Head")
        {
            btn->Click.append([this]() {
                showDeleteAtIndexInput = false;
                showInputValues = false;
                DeleteAtHead();
            });
        }

        else if (deleteOptions[i] == "At Tail")
        {
            btn->Click.append([this]() {
                showDeleteAtIndexInput = false;
                showInputValues = false;
                DeleteAtTail();
            });
        }

        else if (deleteOptions[i] == "At Index")
        {
            btn->Click.append([this]() {
                showDeleteAtIndexInput = true;
                showInputValues = false;
            });
        }

        deleteDropdownButtons.push_back(std::move(btn));
    }

    // Create buttons (Main button)
    createButton = std::make_unique<raywtk::Button>();
    createButton->buttonText = "Create";
    createButton->buttonRect = raylib::Rectangle(12, 604, 128, 24);
    createButton->Click.append([this]() { 
        insertDropdownOpen = false;
        deleteDropdownOpen = false; 
        searchDropdownOpen = false;
        createDropdownOpen = !createDropdownOpen;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
        showInputValues = false;
    });
    createButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // search buttons (Main button)
    searchButton = std::make_unique<raywtk::Button>();
    searchButton->buttonText = "Search";
    searchButton->buttonRect = raylib::Rectangle(12, 632 , 128, 24);
    searchButton->Click.append([this]() { 
        insertDropdownOpen = false;
        deleteDropdownOpen = false; 
        searchDropdownOpen = !searchDropdownOpen;
        createDropdownOpen = false;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
        showInputValues = false;
    });
    searchButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // Dropdown buttons
    for (size_t i = 0; i < searchOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = searchOptions[i];
        btn->buttonRect = raylib::Rectangle((12 + (i + 1) * 128), 632, 128, 24);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

        if (searchOptions[i] == "By Value")
        {
            btn->Click.append([this]() {
                showSearchInput = true;
                searchByValue = true;
                showInputValues = false;
            });
        }

        else if (searchOptions[i] == "By Index")
        {
            btn->Click.append([this]() {
                showSearchInput = true;
                searchByValue = false;
                showInputValues = false;
            });
        }

        searchDropdownButtons.push_back(std::move(btn));
    }

    // Dropdown buttons
    for (size_t i = 0; i < createOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = createOptions[i];
        btn->buttonRect = raylib::Rectangle((12 + (i + 1) * 128), 604, 128, 24);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

        if (createOptions[i] == "Random")
        {
            btn->Click.append([this]() {
                showRandomInput = true;
                showInputValues = false;
                showInputValues = false;
            });
        }

        else if (createOptions[i] == "Clear")
        {
            btn->Click.append([this]() {
                showRandomInput = false;
                showInputValues = false;
                showInputValues = false;
                OnClearButtonClick();
            });
        }

        else if (createOptions[i] == "Load file")
        {
            btn->Click.append([this]() {
                showRandomInput = false;
                showInputValues = false;
                showInputValues = false;
                OnLoadFileButtonClick();
            });
        }

        createDropdownButtons.push_back(std::move(btn));
    }

    // Step-forward button
    stepForwardButton = std::make_unique<raywtk::Button>();
    stepForwardButton->buttonRect = raylib::Rectangle(650, 660, 128, 24);
    stepForwardButton->buttonText = "Step Forward";
    stepForwardButton->Click.append([this]() { OnStepForwardClick(); });
    stepForwardButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);
    // Step-back button
    stepBackwardButton = std::make_unique<raywtk::Button>();
    stepBackwardButton->buttonRect = raylib::Rectangle(515, 660, 128, 24);
    stepBackwardButton->buttonText = "Step Back";
    stepBackwardButton->Click.append([this]() { OnStepBackwardClick(); });
    stepBackwardButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>(textFont);

    // undo button
    undoButtonTex = raylib::Texture(raylib::Image("./images/undo_button.png"));
    undoButton = std::make_unique<raywtk::Button>();
    undoButton->buttonRect = raylib::Rectangle(85, 10, 50, 50);
    undoButton->Click.append([this]() { OnUndoButtonClick(); });
    undoButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&undoButtonTex);

    // redo button
    redoButtonTex = raylib::Texture(raylib::Image("./images/redo_button.png"));
    redoButton = std::make_unique<raywtk::Button>();
    redoButton->buttonRect = raylib::Rectangle(160, 10, 50, 50);
    redoButton->Click.append([this]() { OnRedoButtonClick(); });
    redoButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&redoButtonTex);

    // return button
    returnButtonTex = raylib::Texture(raylib::Image("./images/return_button.png"));
    returnButton = std::make_unique<raywtk::Button>();
    returnButton->buttonRect = raylib::Rectangle(10, 10, 50, 50);
    returnButton->Click.append([this]() { OnReturnButtonClick(); });
    returnButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&returnButtonTex);
}

// destructor of page
ds_viz::pages::LinkedListPage::~LinkedListPage()
{
    returnButtonTex.Unload();
    undoButtonTex.Unload();
    redoButtonTex.Unload();
}

// Generate Random List
void ds_viz::pages::LinkedListPage::OnRandomButtonClick(int numNodes)
{ 
    animationStates.clear();
    currentAnimationState = -1;
    head.reset();
    size = 0;

    srand(time(nullptr)); // Start Generator

    for (int i = 0; i < numNodes; i++)
    {
        int randomValue = rand() % 1999 - 999; 
        InsertRandom(randomValue);
    } 

    Append();
}

// Generate List from keyboard
void ds_viz::pages::LinkedListPage::OnInputButtonClick(std::vector<int> &values)
{
    animationStates.clear();
    currentAnimationState = -1;
    head.reset(); 
    size = 0;

    for (int value : values) {
        InsertRandom(value);
    }

    Append();
}

// A.k.a insert at tail in 1 frame
void ds_viz::pages::LinkedListPage::InsertRandom(int value)
{
    animationStates.clear();
    currentAnimationState = -1;
    ResetColor();

    auto newNode = std::make_unique<raywtk::ListNodeWidget>(value, nodeFont);
    
    // Compute position
    if (!head) 
    {
        newNode->position = raylib::Vector2(headX, headY);
        head = std::move(newNode);
    } 
    
    else 
    {
        raywtk::ListNodeWidget* current = head.get();
        int index = 0;
        while (current->next) 
        {
            current = current->next.get();
            index++;
        }

        // Compute new node position
        float posX = headX + (index + 1) * spacing;
        newNode->position = raylib::Vector2(posX, headY);

        current->next = std::move(newNode);
    }

    size++;
    RepositionNodes();
}

// Load list from textfile, must save before uploading
void ds_viz::pages::LinkedListPage::OnLoadFileButtonClick()
{
    animationStates.clear();
    currentAnimationState = -1;
    DropMessage = "Drop file to load!";
    DropTimer = 2.0f;

    if (IsFileDropped()) 
    {
        FilePathList droppedFiles = LoadDroppedFiles();
        std::ifstream fin(droppedFiles.paths[0]);
        DropTimer = 0.0f;

        if (!fin) 
        {
            errorMessage = "Cannot open file!";
            errorTimer = 2.0f;
            UnloadDroppedFiles(droppedFiles);
            return;
        }

        int n;
        fin >> n; // read number of elements
       
        head.reset();
        size = 0;

        for (int i = 0; i < std::min(n, 12); i++) 
        {
            int x;
            fin >> x;
            InsertRandom(x);
        }

        if (n > 12)
        {
            errorMessage = "Maximum number of nodes is 12!";
            errorTimer = 2.0f;
        }

        fin.close();
        UnloadDroppedFiles(droppedFiles);
        Append();
    }
}

// Clear All List
void ds_viz::pages::LinkedListPage::OnClearButtonClick()
{
    animationStates.clear();
    currentAnimationState = -1;
    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }

    head.reset(); 
    size = 0;
    Append();
}

void ds_viz::pages::LinkedListPage::ResetColor()
{
    raywtk::ListNodeWidget* current = head.get();
    while (current)
    {
        current->color = raylib::Color::Black(); // Reset to default
        current = current->next.get();
    }
}

// Repositioning the nodes if dragged
void ds_viz::pages::LinkedListPage::RepositionNodes()
{
    raywtk::ListNodeWidget* current = head.get();
    int index = 0;

    while (current)
    {
        current->position = raylib::Vector2(headX + index * spacing, headY);
        current = current->next.get();
        index++;
    }
}

// Function to get the current state of linked list
void ds_viz::pages::LinkedListPage::GetListState(std::vector<int> &ListState)
{   
    raywtk::ListNodeWidget* temp = head.get();

    while (temp)
    {
        ListState.push_back(temp->value);
        temp = temp->next.get();
    }
}

// Add the current state of list at each operation
void ds_viz::pages::LinkedListPage::Append()
{
    std::vector<int> ListState;
    GetListState(ListState);
    UndoStack.push(ListState);
    
    while(!RedoStack.empty())
        RedoStack.pop();
}

// Undo
void ds_viz::pages::LinkedListPage::OnUndoButtonClick()
{
    animationStates.clear();
    currentAnimationState = -1;
    if (UndoStack.empty())
        return;
    
    RedoStack.push(UndoStack.top());
    UndoStack.pop();
    std::vector<int> PrevListState;

    if (!UndoStack.empty())
        PrevListState = UndoStack.top();

    head.reset();
    size = 0;
    for (int i: PrevListState)
        InsertRandom(i);
}

// Redo
void ds_viz::pages::LinkedListPage::OnRedoButtonClick()
{
    animationStates.clear();
    currentAnimationState = -1;
    if (RedoStack.empty())
        return;
    
    UndoStack.push(RedoStack.top());
    RedoStack.pop();
    
    head.reset();
    size = 0;
    for (int i: UndoStack.top())
        InsertRandom(i);
}

// Handle return button click event
void ds_viz::pages::LinkedListPage::OnReturnButtonClick()
{
    animationStates.clear();
    currentAnimationState = -1;
    if (_context)
        _context->ChangePage(std::make_shared<ds_viz::pages::MainMenuPage>(*_context));
    else
        std::cerr<<"Error"<<std::endl;
}

// Insert a new node at head
void ds_viz::pages::LinkedListPage::InsertAtHead(int value)
{
    animationStates.clear();
    currentAnimationState = -1;

    ResetColor();
    
    SetPseudoCodeSteps({
        "Node insert = new Node(value)",
        "insert->next = head, head = insert"
    });
    
    animatingInsert = true;
    insertCurrent = head.get();
    insertValue = value;
    insertIndex = 0;
    currentInsertIndex = 0;
    animatingTimer = 0.3f;
    insertState = 0;
    IAH = true;
    IAT = false;
    IAI = false;
    
    RepositionNodes();
}

// Insert a new node at tail
void ds_viz::pages::LinkedListPage::InsertAtTail(int value)
{
    animationStates.clear();
    currentAnimationState = -1;

    ResetColor();
    
    SetPseudoCodeSteps({
        "If list is empty",
        "   Node insert = new Node(value)",
        "   head = insert, return",
        "Node temp = head",
        "while (temp->next)",
        "   temp = temp->next",
        "Node insert = new Node(value)",
        "temp->next = insert"
    });
    
    animatingInsert = true;
    insertCurrent = head.get();
    insertValue = value;
    insertIndex = size;
    currentInsertIndex = 0;
    animatingTimer = 0.3f;
    insertState = 0;
    currentStep = 0;
    IAH = false;
    IAT = true;
    IAI = false;
    
    RepositionNodes();
}

// Insert a new node at an index
void ds_viz::pages::LinkedListPage::InsertAtIndex(int value, int index)
{
    animationStates.clear();
    currentAnimationState = -1;

    ResetColor();
    
    if (index == 0)
    {
        SetPseudoCodeSteps({
            "Node insert = new Node(value)",
            "insert->next = head, head = insert"
        });
    }

    else
    {
        SetPseudoCodeSteps({
            "Node temp = head",
            "for (k = 0; k < index - 1; k++)",
            "   temp = temp->next",
            "Node insert = new Node(value)", 
            "insert->next = temp->next, temp->next = insert"
        });
    }

    animatingInsert = true;
    insertCurrent = head.get();
    insertValue = value;
    insertIndex = index;
    currentInsertIndex = 0;
    animatingTimer = 0.3f;
    insertState = 0;
    currentStep = 0;
    IAH = false;
    IAT = false;
    IAI = true;

    RepositionNodes();
}

// Delete at head
void ds_viz::pages::LinkedListPage::DeleteAtHead()
{
    animationStates.clear();
    currentAnimationState = -1;
    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }

    SetPseudoCodeSteps({
        "If list is empty, return.",
        "Node ToDel = head",
        "head = head->next, delete ToDel"
    });

    ResetColor();
    animatingDelete = true;
    deleteCurrent = head.get();
    deleteIndex = 0;
    currentdeleteIndex = 0;
    animatingTimer = 0.3f;
    deleteState = 0;
    DAH = true;
    DAT = false;
    DAI = false;
    currentStep = 1;
    
    RepositionNodes();
}

// Delete at tail
void ds_viz::pages::LinkedListPage::DeleteAtTail()
{
    animationStates.clear();
    currentAnimationState = -1;
    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }

    SetPseudoCodeSteps({
        "If list is empty, return.",
        "Node temp = head",
        "If head->next = NULL",
        "   Node ToDel = head",
        "   head = NULL, delete ToDel",
        "else",
        "   while (temp->next->next)",
        "       temp = temp->next",
        "   Node ToDel = temp->next",
        "   temp->next = ToDel->next, delete ToDel"
    });

    ResetColor();
    animatingDelete = true;
    deleteCurrent = head.get();
    deleteIndex = size - 1;
    currentdeleteIndex = 0;
    animatingTimer = 0.3f;
    deleteState = 0;
    DAH = false;
    DAT = true;
    DAI = false;
    currentStep = 1;
    
    RepositionNodes();
}

// Delete at an index
void ds_viz::pages::LinkedListPage::DeleteAtIndex(int index)
{
    animationStates.clear();
    currentAnimationState = -1;
    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }

    SetPseudoCodeSteps({
        "If list is empty, return.",
        "Node temp = head",
        "if index == 0",
        "   ToDel = head",
        "   head = head->next, delete ToDel",
        "else",
        "   for (k = 0; k < index; k++)",
        "       temp = temp->next",
        "   Node ToDel = temp->next",
        "   temp->next = ToDel->next, delete ToDel"
    });

    ResetColor();
    animatingDelete = true;
    deleteCurrent = head.get();
    deleteIndex = index;
    currentdeleteIndex = 0;
    animatingTimer = 0.3f;
    deleteState = 0;
    DAH = false;
    DAT = false;
    DAI = true;
    currentStep = 1;
    
    RepositionNodes();
}

// Search nodes by value
void ds_viz::pages::LinkedListPage::SearchByValue(int val)
{
    animationStates.clear();
    currentAnimationState = -1;
    SetPseudoCodeSteps({
        "If list is empty, return.",
        "Node temp = head",
        "while (temp->val != target)",
        "   temp = temp->next",
        "if found, highlight the node",
        "if (temp==NULL), cout<<Node not found"
    });

    ResetColor();
    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }
    
    animatingSearch = true;
    searchCurrent = head.get();
    searchTarget = val;
    animatingTimer = 0.3f; // Delay per step
    searchState = 0;
}

// Search nodes by Index
void ds_viz::pages::LinkedListPage::SearchByIndex(int index)
{   
    animationStates.clear();
    currentAnimationState = -1;
    SetPseudoCodeSteps({
        "If list is empty, return.",
        "Node temp = head, index = 0",
        "while (index != targetindex && temp)",
        "   temp = temp->next, index++",
        "if found, highlight the node",
    });
    
    ResetColor();
    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }

    if (index < 0 || index >= size)
    {
        errorMessage = "Index is out of bounds!";
        errorTimer = 2.0f;
        return;
    }

    animatingSearch = true;
    searchCurrent = head.get();
    searchIndex = index;
    currentSearchIndex = 0;
    animatingTimer = 0.3f;
    searchState = 0;
}

void ds_viz::pages::LinkedListPage::AnimateInsert(float dt)
{
    if (animatingInsert)
    {
        animatingTimer -= dt * animationSpeed;
        if (animatingTimer <= 0)
        {
            SaveListState();

            if (insertState == 0) 
            {
                if (insertIndex == 0)
                {
                    if (IAT)
                        currentStep = 1;
                    else
                        currentStep = 0;

                    newNode = std::make_unique<raywtk::ListNodeWidget>(insertValue, nodeFont);
                    float posX = headX;
                    float posY = headY - 100; 
                    newNode->position = raylib::Vector2(posX, posY);
                    newNode->color = raylib::Color::Maroon(); 

                    insertState = 2; // Move to the next state
                    animatingTimer = 0.3f / animationSpeed; 
                }
                
                else
                {
                    if (IAT)
                        currentStep  = 4;
                    else
                        currentStep = 1;
                    insertCurrent->color = raylib::Color::Orange();
                    insertState = 1;
                    animatingTimer = 0.3f / animationSpeed; 
                }
            } 
            else if (insertState == 1) 
            {
                if (currentInsertIndex == insertIndex - 1 || insertIndex == 0)
                {
                    if (IAT)
                        currentStep = 6;
                    else
                        currentStep = 3;
                    newNode = std::make_unique<raywtk::ListNodeWidget>(insertValue, nodeFont);
                    float posX = headX + (insertIndex) * spacing;
                    float posY = headY - 100;
                    newNode->position = raylib::Vector2(posX, posY);
                    insertCurrent->color = raylib::Color::Black();
                    newNode->color = raylib::Color::Maroon();
                    

                    insertState = 2;
                    animatingTimer = 0.3f / animationSpeed; 
                }
                
                else
                {
                    if (IAT)
                        currentStep = 5;
                    else
                        currentStep = 2;
                    insertCurrent->color = raylib::Color::Black();
                    insertCurrent = insertCurrent->next.get();
                    currentInsertIndex++;
                    insertState = 0;
                    animatingTimer = 0.3f / animationSpeed; 
                }
            }

            else if (insertState == 2)
            {
                if (IAT && insertIndex == 0)
                    currentStep = 2;
                else if (IAT && insertIndex != 0)
                    currentStep = 7;
                else if (IAH)
                    currentStep = 1;
                else
                {
                    if (IAI && insertIndex == 0)
                        currentStep = 1;
                    else
                        currentStep = 5;
                }

                if (insertIndex == 0 && head)
                {
                    newNode->next = std::move(head);
                    head = std::move(newNode);
                }

                else if (insertIndex == 0 && !head)
                    head = std::move(newNode);
                
                else
                {
                    newNode->next = std::move(insertCurrent->next);
                    insertCurrent->next = std::move(newNode);
                }
               
                newNode = nullptr;
                RepositionNodes();
                animatingInsert = false; 
                size++;
                Append();
                SaveListState();
                currentAnimationState = static_cast<int>(animationStates.size()) - 1;
                return;
            }
        }
    }
}

void ds_viz::pages::LinkedListPage::AnimateDelete(float dt)
{
    if (animatingDelete)
    {
        animatingTimer -= dt * animationSpeed;
        if (animatingTimer <= 0)
        {
            SaveListState();
            if (deleteState == 0)
            {
                if (deleteCurrent)
                {
                    if (currentdeleteIndex == deleteIndex)
                    {
                        deleteCurrent->color = raylib::Color::Maroon();   
                        if (deleteIndex == 0)
                        {
                            if (DAI || DAT)
                                currentStep = 3;
                            else if (DAH)
                                currentStep = 1;
                        }
                        else
                            currentStep = 8;
                    }   

                    else
                    {
                        deleteCurrent->color = raylib::Color::Orange();
                        currentStep = 6;
                    }
                }   
                
                if (currentdeleteIndex == deleteIndex - 1)
                    ToDelPrev = deleteCurrent;
                
                deleteState = 1;
                animatingTimer = 0.3f / animationSpeed;
            }

            else if (deleteState == 1)
            {
                if (currentdeleteIndex == deleteIndex)
                {
                    if (deleteIndex != 0)
                    {
                        if (deleteCurrent && deleteCurrent->next)
                            ToDelPrev->next = std::move(deleteCurrent->next);
                        else
                            ToDelPrev->next = nullptr;
                        currentStep = 9;
                    }

                    else
                    {
                        if (deleteCurrent)
                            head = std::move(deleteCurrent->next);

                        if (DAI || DAT)
                            currentStep = 4;
                        else if (DAH)
                            currentStep = 2;
                    }


                    RepositionNodes();

                    animatingDelete = false;
                    size--;
                    Append();
                    SaveListState();
                    currentAnimationState = static_cast<int>(animationStates.size()) - 1;
                    return; 
                }
                
                currentStep = 7;
                deleteCurrent->color = raylib::Color::Black();
                deleteCurrent = deleteCurrent->next.get();
                currentdeleteIndex++;
                deleteState = 0;
                animatingTimer = 0.3f / animationSpeed;
            }
        }
    }
}

void ds_viz::pages::LinkedListPage::AnimateSearch(float dt)
{
    if (animatingSearch && searchCurrent)
    {
        animatingTimer -= dt * animationSpeed;
        if (animatingTimer <= 0)
        {
            SaveListState();

            if (searchState == 0) 
            {
                currentStep = 2; //Highlight the current node
                searchCurrent->color = raylib::Color::Orange();
                searchState = 1;
                animatingTimer = 0.3f / animationSpeed; // Delay between each node
            } 
            else if (searchState == 1) 
            {
                if (searchByValue && searchCurrent->value == searchTarget)
                {
                    currentStep = 4; // Highlight if found
                    searchCurrent->color = raylib::Color::DarkGreen();
                    FindMessage = "Node found!";
                    FindTimer = 2.0f;
                    SaveListState();
                    animatingSearch = false;
                    currentAnimationState = static_cast<int>(animationStates.size()) - 1;
                    searchCurrent = nullptr;
                    return;
                }
                else if (!searchByValue && currentSearchIndex == searchIndex)
                {
                    currentStep = 4; // Highlight if found
                    searchCurrent->color = raylib::Color::DarkGreen();
                    FindMessage = "Node found!";
                    FindTimer = 2.0f;
                    SaveListState();
                    animatingSearch = false;
                    currentAnimationState = static_cast<int>(animationStates.size()) - 1;
                    searchCurrent = nullptr;
                    return;
                }

                currentStep = 3;
                searchCurrent->color = raylib::Color::Black();
                searchCurrent = searchCurrent->next.get();
                currentSearchIndex++;

                if (!searchCurrent)
                {
                    currentStep = 5; // If null, not found
                    animatingSearch = false;
                    errorMessage = "Value not found";
                    errorTimer = 2.0f;
                    SaveListState();
                    currentAnimationState = static_cast<int>(animationStates.size()) - 1;
                    return;
                }

                searchState = 0;
                animatingTimer = 0.3f / animationSpeed; // Delay between each node
            }

            if (!animatingSearch)
            {
                currentAnimationState = -1;
                animationStates.clear(); 
            }
        }
    }
}

void ds_viz::pages::LinkedListPage::OnStepForwardClick()
{
    if (animatingSearch || currentAnimationState >= static_cast<int>(animationStates.size()) - 1)
        return; 

    currentAnimationState++;
    LoadListState(animationStates[currentAnimationState]);
}

void ds_viz::pages::LinkedListPage::OnStepBackwardClick()
{
    if (animatingSearch || currentAnimationState <= 0)
        return; 

    currentAnimationState--;
    LoadListState(animationStates[currentAnimationState]);
}

void ds_viz::pages::LinkedListPage::SaveListState()
{
    ListState state;
    raywtk::ListNodeWidget* current = head.get();

    while (current)
    {
        state.values.push_back(current->value);
        state.colors.push_back(current->color);
        current = current->next.get();
    }

    // Save the newNode if it exists
    if (newNode)
    {
        state.newNodeValue = newNode->value;
        state.newNodeColor = newNode->color;
        state.newNodePos = newNode->position;
        state.hasNewNode = true;
    }
    else
    {
        state.hasNewNode = false;
    }

    state.currentStep = currentStep;
    animationStates.push_back(state);
}

void ds_viz::pages::LinkedListPage::LoadListState(const ListState& state)
{
    head.reset();
    raywtk::ListNodeWidget* prev = nullptr;

    for (size_t i = 0; i < state.values.size(); ++i)
    {
        auto newNode = std::make_unique<raywtk::ListNodeWidget>(state.values[i], nodeFont);
        newNode->color = state.colors[i];
        newNode->position = raylib::Vector2(headX + i * spacing, headY);

        if (!head)
            head = std::move(newNode);
        else
            prev->next = std::move(newNode);

        if (prev)
            prev = prev->next.get();
        else
            prev = head.get();
    }

    if (state.hasNewNode)
    {
        newNode = std::make_unique<raywtk::ListNodeWidget>(state.newNodeValue, nodeFont);
        newNode->color = state.newNodeColor;
        newNode->position = state.newNodePos;
    }

    else
        newNode = nullptr;
    
    currentStep = state.currentStep;
}

void ds_viz::pages::LinkedListPage::CreateNotification(std::string &Message)
{
    int screenWidth = 1280;
    int screenHeight = 720;

    int boxWidth = 400;
    int boxHeight = 40;
    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = 80;

    int fontSize = 20;
    int textWidth = MeasureText(Message.c_str(), fontSize);
    
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, RED);

    int textX = boxX + (boxWidth - textWidth) / 2;
    int textY = boxY + (boxHeight - fontSize) / 2;

    DrawText(Message.c_str(), textX, textY, fontSize, RAYWHITE);
}

void ds_viz::pages::LinkedListPage::DrawInputBox(int X, int Y, int width, int height, std::string &input)
{
    float roundness = 0.2f; 
    int segments = 8;       

    DrawRectangleRounded(raylib::Rectangle(X, Y, width, height), roundness, segments, raylib::Color::Black());
    DrawText(input.c_str(), X + 5, Y + 5, 20, RAYWHITE);
    DrawRectangleRoundedLines(raylib::Rectangle(X, Y, width, height), roundness, segments, raylib::Color::Pink());
}

void ds_viz::pages::LinkedListPage::DrawSpeedBar()
{
    DrawRectangle(speedBarX, speedBarY, speedBarWidth, speedBarHeight, RAYWHITE);
    DrawCircle(speedKnobX, (speedBarY + speedBarHeight / 2), speedKnobRadius, RED);
    DrawText("Speed:", (speedBarX - 80), (speedBarY - 5), 20, WHITE);
    
    char speedText[10];
    sprintf(speedText, "%.2fx", animationSpeed);
    DrawText(speedText, (speedBarX + speedBarWidth + 10), (speedBarY - 5), 20, YELLOW);
}

void ds_viz::pages::LinkedListPage::SetPseudoCodeSteps(const std::vector<std::string>& steps)
{
    pseudoCodeSteps = steps;
    currentStep = -1; 
    showPseudoCode = true; 
}

void ds_viz::pages::LinkedListPage::DrawPseudoCodeBlock()
{
    if (!showPseudoCode || pseudoCodeSteps.empty())
        return;

    int boxX = 867;
    int boxY = 400;
    int boxWidth = 400;
    int boxHeight = 267;
    
    float roundness = 0.1f; 
    int segments = 50;       
    DrawRectangleRoundedLines(raylib::Rectangle(boxX - 5, boxY - 5, boxWidth + 10, boxHeight + 10), roundness, segments, raylib::Color::Pink());

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, raylib::Color::Black());

    int fontSize = 18;
    int lineHeight = 23;
    for (size_t i = 0; i < pseudoCodeSteps.size(); ++i)
    {
        raylib::Font &font = *codeFont;
        if (i == currentStep)
            raylib::DrawTextEx(font, pseudoCodeSteps[i].c_str(), raylib::Vector2(boxX + 10, boxY + 20 + i * lineHeight), fontSize, 1, raylib::Color::Yellow());
        else
            raylib::DrawTextEx(font, pseudoCodeSteps[i].c_str(), raylib::Vector2(boxX + 10, boxY + 20 + i * lineHeight), fontSize, 1, raylib::Color::White());
    }
}

void ds_viz::pages::LinkedListPage::Update(float dt)
{
    raywtk::ListNodeWidget* current = head.get();
    if (current)
        current->Update(dt);

    if (errorTimer > 0) 
    {
        errorTimer -= dt; // Decrease timer
        if (errorTimer < 0) 
            errorTimer = 0; // Hide the message after timeout
    }

    if (DropTimer > 0) 
    {
        DropTimer -= dt; 
        if (DropTimer < 0) 
            DropTimer = 0; 
    }

    if (FindTimer > 0) 
    {
        FindTimer -= dt; 
        if (FindTimer < 0) 
            FindTimer = 0; 
    }

    if (showInsertAtHead)
    {  
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9') || (key == '-' && inputInsertAtHead.empty())) 
            inputInsertAtHead += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !inputInsertAtHead.empty()) 
            inputInsertAtHead.pop_back();
        else if (key == KEY_ENTER && !inputInsertAtHead.empty()) 
        {
            try 
            {
                int newVal = std::stoi(inputInsertAtHead);
            
                if (newVal < -999 || newVal > 999) 
                {
                    inputInsertAtHead.clear();
                    errorMessage = "Value must be between -999 and 999!";
                    errorTimer = 2.0f;
                }
                
                else if (size == 12)
                {
                    inputInsertAtHead.clear();
                    errorMessage = "Maximum number of nodes is 12!";
                    errorTimer = 2.0f;
                }

                else 
                {
                    inputInsertAtHead.clear();
                    if (showInsertAtHead)
                        InsertAtHead(newVal);
                    else
                        InsertAtTail(newVal);
                }
            }
            
            catch (...)
            {
                errorMessage = "Please enter a valid integer between -999 and 999!";
                errorTimer = 2.0f;
            }

            // Hide input box after processing
            showInsertAtHead = false;
        }
    }

    if (showInsertAtTail)
    {  
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9') || (key == '-' && inputInsertAtTail.empty())) 
            inputInsertAtTail += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !inputInsertAtTail.empty()) 
            inputInsertAtTail.pop_back();
        else if (key == KEY_ENTER && !inputInsertAtTail.empty()) 
        {
            try 
            {
                int newVal = std::stoi(inputInsertAtTail);
            
                if (newVal < -999 || newVal > 999) 
                {
                    inputInsertAtTail.clear();
                    errorMessage = "Value must be between -999 and 999!";
                    errorTimer = 2.0f;
                }
                
                else if (size == 12)
                {
                    inputInsertAtTail.clear();
                    errorMessage = "Maximum number of nodes is 12!";
                    errorTimer = 2.0f;
                }

                else 
                {
                    inputInsertAtTail.clear();
                    InsertAtTail(newVal);
                }
            }
            
            catch (...)
            {
                errorMessage = "Please enter a valid integer between -999 and 999!";
                errorTimer = 2.0f;
            }

            // Hide input box after processing
            showInsertAtTail = false;
        }
    }

    if (showInsertAtIndexInput)
    {
        int key = GetKeyPressed();
    
        // Enter value first then index
        if (isEnteringValue)
        {
            if ((key >= '0' && key <= '9') || (key == '-' && inputValue.empty())) 
                inputValue += static_cast<char>(key);
            else if (key == KEY_BACKSPACE && !inputValue.empty())
                inputValue.pop_back();
            else if (key == KEY_ENTER)
            {
                isEnteringValue = false; // Switch to entering index
            }
        }
        
        else // Entering index
        {
            if ((key >= '0' && key <= '9') || (key == '-' && inputIndex.empty())) 
                inputIndex += static_cast<char>(key);
            else if (key == KEY_BACKSPACE && !inputIndex.empty())
                inputIndex.pop_back();
            else if (key == KEY_ENTER && !inputValue.empty() && !inputIndex.empty())
            {
                try 
                {
                    int newValue = std::stoi(inputValue);
                    int index = std::stoi(inputIndex);

                    if (newValue < -999 || newValue > 999) 
                    {
                        inputValue.clear();
                        inputIndex.clear();
                        errorMessage = "Value must be between -999 and 999!";
                        errorTimer = 2.0f; 
                    }
                    else if (index < 0 || index > size) 
                    {
                        inputValue.clear();
                        inputIndex.clear();
                        errorMessage = "Index is out of bounds!";
                        errorTimer = 2.0f;
                    }
                    else if (size >= 12) 
                    {
                        inputValue.clear();
                        inputIndex.clear();
                        errorMessage = "Maximum number of nodes is 12!";
                        errorTimer = 2.0f;
                    }
                    else
                    {
                        InsertAtIndex(newValue, index);
                    }
                }
                
                catch (const std::exception& e) 
                {
                    errorMessage = "Please enter valid integers!";
                    errorTimer = 2.0f;
                }

                inputValue.clear();
                inputIndex.clear();
                showInsertAtIndexInput = false;
                isEnteringValue = true;
            }
        }
    }

    if (showRandomInput)
    {  
        int key = GetKeyPressed();
        if (key >= '0' && key <= '9') 
            inputRandom += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !inputRandom.empty()) 
            inputRandom.pop_back();
        else if (key == KEY_ENTER && !inputRandom.empty()) 
        {
            try 
            {
                int numNodes = std::stoi(inputRandom);
            
                if (numNodes < 1 || numNodes > 12) 
                {
                    inputRandom.clear();
                    errorMessage = "Maximum number of nodes is 12!";
                    errorTimer = 2.0f;
                }
                
                else 
                {
                    OnRandomButtonClick(numNodes);
                    inputRandom.clear();
                }
            }
            
            catch (...)
            {
                errorMessage = "Please enter a valid integer between 1 and 12!";
                errorTimer = 2.0f;
            }

            showRandomInput = false; 
        }
    }

    if (showInputValues)
    {  
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9') || key == '-' || key == ',' || key == ' ') 
            input += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !input.empty()) 
            input.pop_back();
        else if (key == KEY_ENTER && !input.empty()) 
        {
            try 
            {
                values.clear();
                inrange = true;

                std::string processedInput = input;
                std::replace(processedInput.begin(), processedInput.end(), ',', ' ');
                
                std::stringstream ss(processedInput);
                std::string token;
                
                while (ss >> token) 
                {
                    if (!token.empty())
                    {   
                        int value = std::stoi(token);
                        values.push_back(value);
                    }
                }

                for (int i: values)
                {
                    if (i < -999 || i > 999) 
                    {
                        inrange = false;
                        break;
                    }
                }

                if (!inrange)
                {
                    errorMessage = "Value must be between -999 and 999!";
                    errorTimer = 2.0f;
                }
                
                else if (values.size() > 12)
                {
                    errorMessage = "Maximum number of nodes is 12!";
                    errorTimer = 2.0f;
                }
                
                else if (values.empty())
                {
                    errorMessage = "Please enter at least one value!";
                    errorTimer = 2.0f;
                }
            
                else
                    OnInputButtonClick(values);

                input.clear();
                values.clear();
                showInputValues = false;
            }
            
            catch (...)
            {
                errorMessage = "Enter maximum 12 nodes, between -999 and 999!";
                errorTimer = 2.0f;
                input.clear();
            }
        }
    }

    if (showDeleteAtIndexInput)
    {
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9')) 
            inputDeleteIndex += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !inputDeleteIndex.empty()) 
            inputDeleteIndex.pop_back();
        else if (key == KEY_ENTER && !inputDeleteIndex.empty()) 
        {
            try 
            {
                int DelIndex = std::stoi(inputDeleteIndex);
                if (DelIndex < 0 || DelIndex >= size)
                {
                    inputDeleteIndex.clear();

                    if (size == 0)
                        errorMessage = "List is Empty!";
                    else    
                        errorMessage = "Index is out of bounds!";

                    errorTimer = 2.0f;
                }

                else
                    DeleteAtIndex(DelIndex);
            } 
            catch (...) 
            {
                errorMessage = "Invalid input!";
                errorTimer = 2.0f;
            }

            inputDeleteIndex.clear();
            showDeleteAtIndexInput = false;
        }
    }


    if (showSearchInput && searchByValue)
    {
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9') || (key == '-' && searchByValInput.empty())) 
            searchByValInput += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !searchByValInput.empty()) 
            searchByValInput.pop_back();
        else if (key == KEY_ENTER && !searchByValInput.empty()) 
        {
            try 
            {
                int searchValue = std::stoi(searchByValInput);
                SearchByValue(searchValue);
            } 
            catch (...) 
            {
                errorMessage = "Invalid input!";
                errorTimer = 2.0f;
            }

            searchByValInput.clear();
            showSearchInput = false;
        }
    }

    if (showSearchInput && !searchByValue)
    {
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9')) 
            searchByIndInput += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !searchByIndInput.empty()) 
            searchByIndInput.pop_back();
        else if (key == KEY_ENTER && !searchByIndInput.empty()) 
        {
            try 
            {
                int searchIndex = std::stoi(searchByIndInput);

                if (searchIndex < 0 || searchIndex >= size)
                {
                    errorMessage = "Index is out of bounds!";
                    errorTimer = 2.0f;
                }
                
                else
                    SearchByIndex(searchIndex);
            } 
            catch (...) 
            {
                errorMessage = "Invalid input!";
                errorTimer = 2.0f;
            }

            searchByIndInput.clear();
            showSearchInput = false;
        }
    }

    // Adjust speed
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();

        if (isDraggingSpeedKnob || CheckCollisionPointCircle(mousePos, {speedKnobX, speedBarY + speedBarHeight / 2}, speedKnobRadius))
        {
            isDraggingSpeedKnob = true;
            speedKnobX = Clamp(mousePos.x, speedBarX, speedBarX + speedBarWidth);

            // Update speed while dragging 
            float t = (speedKnobX - speedBarX) / speedBarWidth;
            animationSpeed = speedMin + t * (speedMax - speedMin);
        }
    }
    
    else if (isDraggingSpeedKnob) 
    {
        isDraggingSpeedKnob = false;

        // Snap to nearest speed
        float closest = speedSteps[0];
        for (float step : speedSteps)
        {
            if (std::abs(step - animationSpeed) < std::abs(closest - animationSpeed))
            closest = step;
        }

        animationSpeed = closest;

        float t = (animationSpeed - speedMin) / (speedMax - speedMin);
        speedKnobX = speedBarX + t * speedBarWidth;

        animatingTimer = 0.3f / animationSpeed;
    }

    AnimateSearch(dt);
    AnimateInsert(dt);
    AnimateDelete(dt);

    actionBarButton->Update(dt);

    if (actionBarVisible) 
    {
        inputButton->Update(dt);
        insertButton->Update(dt);
        deleteButton->Update(dt);
        searchButton->Update(dt);
        createButton->Update(dt);
        repositionButton->Update(dt);
    }

    if (insertDropdownOpen)
    {
        for (auto& btn : insertDropdownButtons)
            btn->Update(dt);
    }

    if (deleteDropdownOpen)
    {
        for (auto& btn : deleteDropdownButtons)
            btn->Update(dt);
    }

    if (searchDropdownOpen)
    {
        for (auto& btn : searchDropdownButtons)
            btn->Update(dt);
    }

    if (createDropdownOpen)
    {
        for (auto& btn : createDropdownButtons)
            btn->Update(dt);
    }

    pseudoToggleButton->Update(dt);
    stepForwardButton->Update(dt);
    stepBackwardButton->Update(dt);

    undoButton->Update(dt);
    redoButton->Update(dt);
    returnButton->Update(dt);
}

void ds_viz::pages::LinkedListPage::Render()
{
    // Draw title
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));

    // Render the linked list
    raywtk::ListNodeWidget* current = head.get();
    if (current)
        current->Render();

    if (newNode)
        newNode->Render();

    // Draw Head and Tail labels
    if (head)
    {
        DrawText("head", head->position.x - 30, head->position.y - 70, 30, RED);
        if (!head->next) 
            DrawText("tail", head->position.x - 20, head->position.y + 40, 30, GREEN);
        else
        {
            raywtk::ListNodeWidget* tail = head.get();
            while (tail->next)
                tail = tail->next.get();
            DrawText("tail", tail->position.x - 20, tail->position.y + 40, 30, GREEN);
        }
    }

    // Draw Speed Bar
    DrawSpeedBar();

    // Draw input bars only if active
    if (showInsertAtHead && insertDropdownOpen)
        DrawInputBox(140, 576, 128, 24, inputInsertAtHead);

    if (showInsertAtTail && insertDropdownOpen)
        DrawInputBox(268, 576, 128, 24, inputInsertAtTail);

    if (showInsertAtIndexInput && insertDropdownOpen)
    {
        DrawInputBox(396, 576, 64, 24, inputValue);
        DrawInputBox(460, 576, 64, 24, inputIndex);

        DrawText("Value", 396, 610, 20, YELLOW);
        DrawText("Index", 460, 610, 20, YELLOW);
    }

    if (showDeleteAtIndexInput && deleteDropdownOpen)
        DrawInputBox(396, 604, 128, 24, inputDeleteIndex);

    if (showRandomInput)
        DrawInputBox(140, 632, 128, 24, inputRandom);

    if (showInputValues)
        DrawInputBox(140, 520, 350, 24, input);

    if (showSearchInput && searchByValue)
        DrawInputBox(140, 660, 128, 24, searchByValInput);
    
    if (showSearchInput && !searchByValue)
        DrawInputBox(268, 660, 128, 24, searchByIndInput);
     
    // Draw error message if needed
    if (errorTimer > 0)
        CreateNotification(errorMessage);

    // Draw drop message if needed
    if (DropTimer > 0)
        CreateNotification(DropMessage);

    // Draw find message if needed
    if (FindTimer > 0)
        CreateNotification(FindMessage);

    actionBarButton->Render();

    if (actionBarVisible) 
    {
        inputButton->Render();
        insertButton->Render();
        deleteButton->Render();
        searchButton->Render();
        createButton->Render();
        repositionButton->Render();
    }

    if (insertDropdownOpen)
    {
        for (auto& btn : insertDropdownButtons)
            btn->Render();
    }

    if (deleteDropdownOpen)
    {
        for (auto& btn: deleteDropdownButtons)
            btn->Render();
    }

    if (searchDropdownOpen)
    {
        for (auto& btn: searchDropdownButtons)
            btn->Render();
    }

    if (createDropdownOpen)
    {
        for (auto& btn: createDropdownButtons)
            btn->Render();
    }


    DrawPseudoCodeBlock();
    pseudoToggleButton->Render();
    stepForwardButton->Render();
    stepBackwardButton->Render();   

    undoButton->Render();
    redoButton->Render(); 
    returnButton->Render();
}