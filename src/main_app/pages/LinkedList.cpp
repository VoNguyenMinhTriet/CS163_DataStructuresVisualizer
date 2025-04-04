#include "./LinkedList.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/image_button.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib> // For rand()
#include <ctime>

ds_viz::pages::LinkedListPage::LinkedListPage() 
{
    font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
    title = raylib::Text("Singly-Linked List", 128, raylib::Color::White(), *font, 0);
    
    // insert at index button
    insertAtIndexButton = std::make_unique<raywtk::Button>();
    insertAtIndexButton->buttonText = "Insert At Index";
    insertAtIndexButton->buttonRect = raylib::Rectangle(500, 700, 180, 50);
    insertAtIndexButton->Click.append([this]() { 
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = true; 
        showRandomInput = false; 
        showSearchInput = false;
        showDeleteAtHead = false;
        showDeleteAtTail = false;
        showDeleteAtIndexInput = false; 
    });
    insertAtIndexButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // insert at head button
    insertAtHeadButton = std::make_unique<raywtk::Button>();
    insertAtHeadButton->buttonText = "Insert At Head";
    insertAtHeadButton->buttonRect = raylib::Rectangle(500, 800, 180, 50);
    insertAtHeadButton->Click.append([this]() { 
        showInsertAtHead = true;
        showInsertAtTail = false;
        showInsertAtIndexInput = false; 
        showRandomInput = false; 
        showSearchInput = false;
        showDeleteAtHead = false;
        showDeleteAtTail = false;
        showDeleteAtIndexInput = false; 
    });
    insertAtHeadButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // insert at tail button
    insertAtTailButton = std::make_unique<raywtk::Button>();
    insertAtTailButton->buttonText = "Insert At Tail";
    insertAtTailButton->buttonRect = raylib::Rectangle(500, 900, 180, 50);
    insertAtTailButton->Click.append([this]() { 
        showInsertAtHead = false;
        showInsertAtTail = true;
        showInsertAtIndexInput = false; 
        showRandomInput = false; 
        showSearchInput = false;
        showDeleteAtHead = false;
        showDeleteAtTail = false;
        showDeleteAtIndexInput = false; 
    });
    insertAtTailButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    
    // delete at index button
    deleteAtIndexButton = std::make_unique<raywtk::Button>();
    deleteAtIndexButton->buttonText = "Delete At Index";
    deleteAtIndexButton->buttonRect = raylib::Rectangle(1000, 700, 180, 50);
    deleteAtIndexButton->Click.append([this]() { 
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false; 
        showRandomInput = false;
        showDeleteAtHead = false;
        showDeleteAtTail = false;
        showDeleteAtIndexInput = true; 
        showSearchInput = false;
    });
    deleteAtIndexButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // delete at head button
    deleteAtHeadButton = std::make_unique<raywtk::Button>();
    deleteAtHeadButton->buttonText = "Delete At Head";
    deleteAtHeadButton->buttonRect = raylib::Rectangle(1000, 800, 180, 50);
    deleteAtHeadButton->Click.append([this]() { DeleteAtHead(); });
    deleteAtHeadButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // delete at tail button
    deleteAtTailButton = std::make_unique<raywtk::Button>();
    deleteAtTailButton->buttonText = "Delete At Tail";
    deleteAtTailButton->buttonRect = raylib::Rectangle(1000, 900, 180, 50);
    deleteAtTailButton->Click.append([this]() { DeleteAtTail(); });
    deleteAtTailButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // random create button
    randomButton = std::make_unique<raywtk::Button>();
    randomButton->buttonText = "Create Random";
    randomButton->buttonRect = raylib::Rectangle(100, 700, 180, 50);
    randomButton->Click.append([this]() { 
        showInsertAtHead = false;
        showInsertAtTail = false;
        showRandomInput = true; 
        showInsertAtIndexInput = false; 
        showSearchInput = false;
        showDeleteAtHead = false;
        showDeleteAtTail = false;
        showDeleteAtIndexInput = false; 
    });
    randomButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // clear all button
    clearAllButton = std::make_unique<raywtk::Button>();
    clearAllButton->buttonText = "Clear All";
    clearAllButton->buttonRect = raylib::Rectangle(100, 900, 180, 50);
    clearAllButton->Click.append([this]() { OnClearButtonClick(); });
    clearAllButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // repositioning nodes button
    repositionButton = std::make_unique<raywtk::Button>();
    repositionButton->buttonText = "Reposition";
    repositionButton->buttonRect = raylib::Rectangle(300, 900, 180, 50);
    repositionButton->Click.append([this]() { RepositionNodes(); });
    repositionButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Load from file button
    loadFileButton = std::make_unique<raywtk::Button>();
    loadFileButton->buttonText = "Load from File";
    loadFileButton->buttonRect = raylib::Rectangle(100, 800, 180, 50);
    loadFileButton->Click.append([this]() { OnLoadFileButtonClick(); });
    loadFileButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // search by value button
    searchByValueButton = std::make_unique<raywtk::Button>();
    searchByValueButton->buttonText = "Search by Value";
    searchByValueButton->buttonRect = raylib::Rectangle(300, 700, 180, 50);
    searchByValueButton->Click.append([this]() { 
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false; 
        searchByValue = true; 
        showSearchInput = true; 
        showRandomInput = false;
        showDeleteAtHead = false;
        showDeleteAtTail = false;
        showDeleteAtIndexInput = false; 
    });
    searchByValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // search by index button
    searchByIndexButton = std::make_unique<raywtk::Button>();
    searchByIndexButton->buttonText = "Search by Index";
    searchByIndexButton->buttonRect = raylib::Rectangle(300, 800, 180, 50);
    searchByIndexButton->Click.append([this]() { 
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false; 
        searchByValue = false; 
        showSearchInput = true;
        showRandomInput = false; 
        showDeleteAtHead = false;
        showDeleteAtTail = false;
        showDeleteAtIndexInput = false; 
    });
    searchByIndexButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    undoButtonTex = raylib::Texture(raylib::Image("./images/undo_button.png"));
    undoButton = std::make_unique<raywtk::Button>();
    undoButton->buttonRect = raylib::Rectangle(1600, 30, 80, 80);
    undoButton->Click.append([this]() { OnUndoButtonClick(); });
    undoButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&undoButtonTex);

    redoButtonTex = raylib::Texture(raylib::Image("./images/redo_button.png"));
    redoButton = std::make_unique<raywtk::Button>();
    redoButton->buttonRect = raylib::Rectangle(1700, 30, 80, 80);
    redoButton->Click.append([this]() { OnRedoButtonClick(); });
    redoButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&redoButtonTex);

    returnButtonTex = raylib::Texture(raylib::Image("./images/return_button.png"));
    returnButton = std::make_unique<raywtk::Button>();
    returnButton->buttonRect = raylib::Rectangle(1800, 30, 80, 80);
    returnButton->Click.append([this]() { OnReturnButtonClick(); });
    returnButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&returnButtonTex);
}

ds_viz::pages::LinkedListPage::~LinkedListPage()
{
    returnButtonTex.Unload();
    undoButtonTex.Unload();
    redoButtonTex.Unload();
}


// Handle return button click event
void ds_viz::pages::LinkedListPage::OnReturnButtonClick()
{
    if (_context)
        _context->ChangePage(std::make_shared<ds_viz::pages::MainMenuPage>());
    else
        std::cerr<<"Error"<<std::endl;
}

// Generate Random List
void ds_viz::pages::LinkedListPage::OnRandomButtonClick(int numNodes)
{ 
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

// Clear All List
void ds_viz::pages::LinkedListPage::OnClearButtonClick()
{
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
    raywtk::NodeWidget* current = head.get();
    while (current)
    {
        current->color = raylib::Color::Blue(); // Reset to default
        current = current->next.get();
    }
}

// Repositioning the nodes if dragged
void ds_viz::pages::LinkedListPage::RepositionNodes()
{
    raywtk::NodeWidget* current = head.get();
    int index = 0;

    while (current)
    {
        current->position = raylib::Vector2(headX + index * spacing, headY);
        current = current->next.get();
        index++;
    }
}

// Load list from textfile, must save before uploading
void ds_viz::pages::LinkedListPage::OnLoadFileButtonClick()
{
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

        for (int i = 0; i < std::min(n, 18); i++) 
        {
            int x;
            fin >> x;
            InsertRandom(x);
        }

        if (n > 18)
        {
            errorMessage = "Maximum number of nodes is 18!";
            errorTimer = 2.0f;
        }

        fin.close();
        UnloadDroppedFiles(droppedFiles);
        Append();
    }
}

void ds_viz::pages::LinkedListPage::Append()
{
    std::vector<int> ListState;
    GetListState(ListState);
    UndoStack.push(ListState);
    
    while(!RedoStack.empty())
        RedoStack.pop();
}

// Function to get the current state of linked list
void ds_viz::pages::LinkedListPage::GetListState(std::vector<int> &ListState)
{   
    raywtk::NodeWidget* temp = head.get();

    while (temp)
    {
        ListState.push_back(temp->value);
        temp = temp->next.get();
    }
}

// Undo
void ds_viz::pages::LinkedListPage::OnUndoButtonClick()
{
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
    if (RedoStack.empty())
        return;
    
    UndoStack.push(RedoStack.top());
    RedoStack.pop();
    
    head.reset();
    size = 0;
    for (int i: UndoStack.top())
        InsertRandom(i);
}

// A.k.a insert at tail in 1 frame
void ds_viz::pages::LinkedListPage::InsertRandom(int value)
{
    ResetColor();

    auto newNode = std::make_unique<raywtk::NodeWidget>(value, sharedFont);
    
    // Compute position
    if (!head) 
    {
        newNode->position = raylib::Vector2(headX, headY);
        head = std::move(newNode);
    } 
    
    else 
    {
        raywtk::NodeWidget* current = head.get();
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

// Insert a new node at head
void ds_viz::pages::LinkedListPage::InsertAtHead(int value)
{
    InsertAtIndex(value, 0);
}

// Insert a new node at tail
void ds_viz::pages::LinkedListPage::InsertAtTail(int value)
{
    InsertAtIndex(value, size);
}

// Insert a new node at an index
void ds_viz::pages::LinkedListPage::InsertAtIndex(int value, int index)
{
    ResetColor();
    if (index == 0)
    {
        auto newNode = std::make_unique<raywtk::NodeWidget>(value, sharedFont);
        newNode->position = raylib::Vector2(headX, headY);
        newNode->next = std::move(head);
        head = std::move(newNode);

        auto shift = head->next.get();
        float newposX = headX + spacing;
        while (shift)
        {
            shift->position = raylib::Vector2(newposX, headY);
            newposX += spacing;
            shift = shift->next.get();
        }

        size++;
        head->color = raylib::Color::Maroon();
        RepositionNodes();
        Append();
        return;
    }
    
    animatingInsert = true;
    insertCurrent = head.get();
    insertValue = value;
    insertIndex = index;
    currentInsertIndex = 0;
    animatingTimer = 0.3f;
    insertState = 0;
    
    RepositionNodes();
}

// Delete at head
void ds_viz::pages::LinkedListPage::DeleteAtHead()
{
    DeleteAtIndex(0);   
}

// Delete at tail
void ds_viz::pages::LinkedListPage::DeleteAtTail()
{
    DeleteAtIndex(size - 1);
}

// Delete at an index
void ds_viz::pages::LinkedListPage::DeleteAtIndex(int index)
{
    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }

    ResetColor();
    animatingDelete = true;
    deleteCurrent = head.get();
    deleteIndex = index;
    currentdeleteIndex = 0;
    animatingTimer = 0.3f;
    deleteState = 0;
    
    RepositionNodes();
}

// Search nodes by value
void ds_viz::pages::LinkedListPage::SearchByValue(int val)
{
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
    if (animatingInsert && insertCurrent)
    {
        animatingTimer -= dt * animationSpeed;
        if (animatingTimer <= 0)
        {
            if (insertState == 0) 
            {
                insertCurrent->color = raylib::Color::Orange();
                insertState = 1;
                animatingTimer = 0.3f / animationSpeed; // Delay between each node
            } 
            else if (insertState == 1) 
            {
                if (currentInsertIndex == insertIndex - 1)
                {
                    auto newNode = std::make_unique<raywtk::NodeWidget>(insertValue, sharedFont);
                    float posX = headX + (insertIndex) * spacing;
                    newNode->position = raylib::Vector2(posX, headY);
                    insertCurrent->color = raylib::Color::Blue();
                    newNode->color = raylib::Color::Maroon();
                    newNode->next = std::move(insertCurrent->next);
                    insertCurrent->next = std::move(newNode);

                    auto shift = insertCurrent->next.get()->next.get();
                    float newposX = posX + spacing;
                    while (shift)
                    {
                        shift->position = raylib::Vector2(newposX, headY);
                        newposX += spacing;
                        shift = shift->next.get();
                    }
        
                    animatingInsert = false;
                    size++;
                    Append();
                    return;
                }
    
                insertCurrent->color = raylib::Color::Blue();
                insertCurrent = insertCurrent->next.get();
                currentInsertIndex++;

                insertState = 0;
                animatingTimer = 0.3f / animationSpeed; // Delay between each node
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
            if (deleteState == 0)
            {
                if (deleteCurrent)
                {
                    if (currentdeleteIndex == deleteIndex)
                        deleteCurrent->color = raylib::Color::Maroon();                
                    else
                        deleteCurrent->color = raylib::Color::Orange();
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
                    }

                    else
                    {
                        if (deleteCurrent)
                            head = std::move(deleteCurrent->next);
                    }


                    float newposX = headX;
                    raywtk::NodeWidget* temp = head.get();
                    while (temp)
                    {
                        temp->position = raylib::Vector2(newposX, headY);
                        newposX += spacing;
                        temp = temp->next.get();
                    }

                    animatingDelete = false;
                    size--;
                    Append();
                    return; 
                }
                
                deleteCurrent->color = raylib::Color::Blue();
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
            if (searchState == 0) 
            {
                searchCurrent->color = raylib::Color::Orange();
                searchState = 1;
                animatingTimer = 0.3f / animationSpeed; // Delay between each node
            } 
            else if (searchState == 1) 
            {
                if (searchByValue && searchCurrent->value == searchTarget)
                {
                    searchCurrent->color = raylib::Color::DarkGreen();
                    FindMessage = "Node found!";
                    FindTimer = 2.0f;
                    animatingSearch = false;
                    searchCurrent = nullptr;
                    return;
                }
                else if (!searchByValue && currentSearchIndex == searchIndex)
                {
                    searchCurrent->color = raylib::Color::DarkGreen();
                    FindMessage = "Node found!";
                    FindTimer = 2.0f;
                    animatingSearch = false;
                    searchCurrent = nullptr;
                    return;
                }

                searchCurrent->color = raylib::Color::Blue();
                searchCurrent = searchCurrent->next.get();
                currentSearchIndex++;

                if (!searchCurrent)
                {
                    animatingSearch = false;
                    errorMessage = "Value not found";
                    errorTimer = 2.0f;
                    return;
                }

                searchState = 0;
                animatingTimer = 0.3f / animationSpeed; // Delay between each node
            }
        }
    }
}

void ds_viz::pages::LinkedListPage::CreateNotification(std::string &Message)
{
    int screenWidth = _context->ref_raylib_window->GetWidth();
    int screenHeight = _context->ref_raylib_window->GetHeight();

    int boxWidth = 600;
    int boxHeight = 40;
    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = screenHeight - 800;

    int fontSize = 20;
    int textWidth = MeasureText(Message.c_str(), fontSize);
    
    // Draw background box
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, RED);

    // Adjust text position to center inside the box
    int textX = boxX + (boxWidth - textWidth) / 2;
    int textY = boxY + (boxHeight - fontSize) / 2;

    DrawText(Message.c_str(), textX, textY, fontSize, RAYWHITE);
}

void ds_viz::pages::LinkedListPage::DrawInputBox(int X, int Y, std::string &input)
{
    DrawRectangle(X, Y, 90, 50, DARKGRAY);
    DrawText(input.c_str(), X + 20, Y + 15, 20, RAYWHITE);
    DrawRectangleLines(X - 2, Y - 2, 94, 54, BLUE);
}

void ds_viz::pages::LinkedListPage::DrawSpeedBar()
{
    DrawRectangle(speedBarX, speedBarY, speedBarWidth, speedBarHeight, RAYWHITE);
    DrawCircle(speedKnobX, speedBarY + speedBarHeight / 2, speedKnobRadius, RED);
    DrawText("Speed:", speedBarX - 80, speedBarY - 5, 20, WHITE);
    
    char speedText[10];
    sprintf(speedText, "%.2fx", animationSpeed);
    DrawText(speedText, speedBarX + speedBarWidth + 10, speedBarY - 5, 20, YELLOW);
}

void ds_viz::pages::LinkedListPage::Update(float dt)
{
    raywtk::NodeWidget* current = head.get();
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
                
                else if (size == 18)
                {
                    inputInsertAtHead.clear();
                    errorMessage = "Maximum number of nodes is 18!";
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
                
                else if (size == 18)
                {
                    inputInsertAtTail.clear();
                    errorMessage = "Maximum number of nodes is 18!";
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
                    else if (size >= 18) 
                    {
                        inputValue.clear();
                        inputIndex.clear();
                        errorMessage = "Maximum number of nodes is 18!";
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
            
                if (numNodes < 1 || numNodes > 18) 
                {
                    inputRandom.clear();
                    errorMessage = "Maximum number of nodes is 18!";
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
                errorMessage = "Please enter a valid integer between 1 and 18!";
                errorTimer = 2.0f;
            }

            showRandomInput = false; 
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

    insertAtHeadButton->Update(dt);
    insertAtTailButton->Update(dt);
    insertAtIndexButton->Update(dt);
    deleteAtHeadButton->Update(dt);
    deleteAtTailButton->Update(dt);
    deleteAtIndexButton->Update(dt);
    randomButton->Update(dt);
    clearAllButton->Update(dt);
    repositionButton->Update(dt);
    loadFileButton->Update(dt);
    searchByValueButton->Update(dt);
    searchByIndexButton->Update(dt);
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
    raywtk::NodeWidget* current = head.get();
    if (current)
        current->Render();

    // Draw Head and Tail labels
    if (head)
    {
        DrawText("head", head->position.x - 20, head->position.y - 50, 20, RED);
        if (!head->next) 
            DrawText("tail", head->position.x - 20, head->position.y + 30, 20, GREEN);
        else
        {
            raywtk::NodeWidget* tail = head.get();
            while (tail->next)
                tail = tail->next.get();
            DrawText("tail", tail->position.x - 20, tail->position.y + 30, 20, GREEN);
        }
    }

    // Draw Speed Bar
    DrawSpeedBar();

    // Draw input bars only if active
    if (showInsertAtHead)
        DrawInputBox(690, 800, inputInsertAtHead);

    if (showInsertAtTail)
        DrawInputBox(690, 900, inputInsertAtTail);

    if (showInsertAtIndexInput)
    {
        DrawInputBox(690, 700, inputValue);
        DrawInputBox(790, 700, inputIndex);

        DrawText("Value:", 690, 680, 20, YELLOW);
        DrawText("Index:", 790, 680, 20, YELLOW);
    }

    if (showDeleteAtIndexInput)
        DrawInputBox(1190, 700, inputDeleteIndex);

    if (showRandomInput)
        DrawInputBox(100, 750, inputRandom);

    if (showSearchInput && searchByValue)
        DrawInputBox(300, 750, searchByValInput);
    
    if (showSearchInput && !searchByValue)
        DrawInputBox(300, 850, searchByIndInput);
     
    // Draw error message if needed
    if (errorTimer > 0)
        CreateNotification(errorMessage);

    // Draw drop message if needed
    if (DropTimer > 0)
        CreateNotification(DropMessage);

    // Draw find message if needed
    if (FindTimer > 0)
        CreateNotification(FindMessage);

    // Render buttons
    insertAtHeadButton->Render();
    insertAtTailButton->Render();
    insertAtIndexButton->Render();
    deleteAtHeadButton->Render();
    deleteAtTailButton->Render();
    deleteAtIndexButton->Render();
    randomButton->Render();
    clearAllButton->Render();
    repositionButton->Render();
    loadFileButton->Render();
    searchByValueButton->Render();
    searchByIndexButton->Render();
    undoButton->Render();
    redoButton->Render(); 
    returnButton->Render();
}