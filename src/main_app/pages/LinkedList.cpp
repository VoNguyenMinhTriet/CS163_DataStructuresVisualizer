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
    
    
    // repositioning nodes button
    repositionButton = std::make_unique<raywtk::Button>();
    repositionButton->buttonText = "Reposition";
    repositionButton->buttonRect = raylib::Rectangle(100, 820, 180, 50);
    repositionButton->Click.append([this]() { RepositionNodes(); });
    repositionButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    
    // Insert button (Main button)
    insertButton = std::make_unique<raywtk::Button>();
    insertButton->buttonText = "Insert";
    insertButton->buttonRect = raylib::Rectangle(100, 600, 180, 50);
    insertButton->Click.append([this]() { 
        insertDropdownOpen = !insertDropdownOpen;
        deleteDropdownOpen = false; 
        searchDropdownOpen = false;
        otherDropdownOpen = false;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
    });
    insertButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Dropdown insert buttons
    for (size_t i = 0; i < insertOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = insertOptions[i];
        btn->buttonRect = raylib::Rectangle(100 + (i + 1) * 180, 600, 180, 50);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

        if (insertOptions[i] == "At Head")
        {
            btn->Click.append([this]() {
                showInsertAtHead = true;
                showInsertAtTail = false;
                showInsertAtIndexInput = false;
            });
        }

        else if (insertOptions[i] == "At Tail")
        {
            btn->Click.append([this]() {
                showInsertAtHead = false;
                showInsertAtTail = true;
                showInsertAtIndexInput = false;
            });
        }

        else if (insertOptions[i] == "At Index")
        {
            btn->Click.append([this]() {
                showInsertAtHead = false;
                showInsertAtTail = false;
                showInsertAtIndexInput = true;
            });
        }

        insertDropdownButtons.push_back(std::move(btn));
    }

    // delete button (Main button)
    deleteButton = std::make_unique<raywtk::Button>();
    deleteButton->buttonText = "Delete";
    deleteButton->buttonRect = raylib::Rectangle(100, 655, 180, 50);
    deleteButton->Click.append([this]() { 
        insertDropdownOpen = false;
        deleteDropdownOpen = !deleteDropdownOpen; 
        searchDropdownOpen = false;
        otherDropdownOpen = false;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
    });
    deleteButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Dropdown buttons
    for (size_t i = 0; i < deleteOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = deleteOptions[i];
        btn->buttonRect = raylib::Rectangle(100 + (i + 1) * 180, 655, 180, 50);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

        if (deleteOptions[i] == "At Head")
        {
            btn->Click.append([this]() {
                showDeleteAtIndexInput = false;
                DeleteAtHead();
            });
        }

        else if (deleteOptions[i] == "At Tail")
        {
            btn->Click.append([this]() {
                showDeleteAtIndexInput = false;
                DeleteAtTail();
            });
        }

        else if (deleteOptions[i] == "At Index")
        {
            btn->Click.append([this]() {
                showDeleteAtIndexInput = true;
            });
        }

        deleteDropdownButtons.push_back(std::move(btn));
    }

    // search button (Main button)
    searchButton = std::make_unique<raywtk::Button>();
    searchButton->buttonText = "Search";
    searchButton->buttonRect = raylib::Rectangle(100, 710, 180, 50);
    searchButton->Click.append([this]() { 
        insertDropdownOpen = false;
        deleteDropdownOpen = false; 
        searchDropdownOpen = !searchDropdownOpen;
        otherDropdownOpen = false;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
    });
    searchButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Dropdown buttons
    for (size_t i = 0; i < searchOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = searchOptions[i];
        btn->buttonRect = raylib::Rectangle(100 + (i + 1) * 180, 710, 180, 50);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

        if (searchOptions[i] == "By Value")
        {
            btn->Click.append([this]() {
                showSearchInput = true;
                searchByValue = true;
            });
        }

        else if (searchOptions[i] == "By Index")
        {
            btn->Click.append([this]() {
                showSearchInput = true;
                searchByValue = false;
            });
        }

        searchDropdownButtons.push_back(std::move(btn));
    }

    // other button (Main button)
    otherButton = std::make_unique<raywtk::Button>();
    otherButton->buttonText = "Other";
    otherButton->buttonRect = raylib::Rectangle(100, 765, 180, 50);
    otherButton->Click.append([this]() { 
        insertDropdownOpen = false;
        deleteDropdownOpen = false; 
        searchDropdownOpen = false;
        otherDropdownOpen = !otherDropdownOpen;
        showInsertAtHead = false;
        showInsertAtTail = false;
        showInsertAtIndexInput = false;
        showDeleteAtIndexInput = false;
        showSearchInput  = false;
        showRandomInput = false;
    });
    otherButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Dropdown buttons
    for (size_t i = 0; i < otherOptions.size(); i++)
    {
        auto btn = std::make_unique<raywtk::Button>();
        btn->buttonText = otherOptions[i];
        btn->buttonRect = raylib::Rectangle(100 + (i + 1) * 180, 765, 180, 50);
        btn->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

        if (otherOptions[i] == "Random")
        {
            btn->Click.append([this]() {
                showRandomInput = true;
            });
        }

        else if (otherOptions[i] == "Clear")
        {
            btn->Click.append([this]() {
                showRandomInput = false;
                OnClearButtonClick();
            });
        }

        else if (otherOptions[i] == "Load file")
        {
            btn->Click.append([this]() {
                showRandomInput = false;
                OnLoadFileButtonClick();
            });
        }

        otherDropdownButtons.push_back(std::move(btn));
    }

    // undo button
    undoButtonTex = raylib::Texture(raylib::Image("./images/undo_button.png"));
    undoButton = std::make_unique<raywtk::Button>();
    undoButton->buttonRect = raylib::Rectangle(1600, 30, 80, 80);
    undoButton->Click.append([this]() { OnUndoButtonClick(); });
    undoButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&undoButtonTex);

    // redo button
    redoButtonTex = raylib::Texture(raylib::Image("./images/redo_button.png"));
    redoButton = std::make_unique<raywtk::Button>();
    redoButton->buttonRect = raylib::Rectangle(1700, 30, 80, 80);
    redoButton->Click.append([this]() { OnRedoButtonClick(); });
    redoButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&redoButtonTex);

    // return button
    returnButtonTex = raylib::Texture(raylib::Image("./images/return_button.png"));
    returnButton = std::make_unique<raywtk::Button>();
    returnButton->buttonRect = raylib::Rectangle(1800, 30, 80, 80);
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

// Handle return button click event
void ds_viz::pages::LinkedListPage::OnReturnButtonClick()
{
    if (_context)
        _context->ChangePage(std::make_shared<ds_viz::pages::MainMenuPage>());
    else
        std::cerr<<"Error"<<std::endl;
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
    DrawRectangle(X, Y, 85, 50, DARKGRAY);
    DrawText(input.c_str(), X + 10, Y + 15, 20, RAYWHITE);
    DrawRectangleLines(X - 2, Y - 2, 89, 54, BLUE);
}

void ds_viz::pages::LinkedListPage::DrawInputBox2(int X, int Y, std::string &input)
{
    DrawRectangle(X, Y, 180, 50, DARKGRAY);
    DrawText(input.c_str(), X + 10, Y + 15, 20, RAYWHITE);
    DrawRectangleLines(X - 2, Y - 2, 184, 54, BLUE);
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

    repositionButton->Update(dt);
    
    insertButton->Update(dt);
    if (insertDropdownOpen)
    {
        for (auto& btn : insertDropdownButtons)
            btn->Update(dt);
    }

    deleteButton->Update(dt);
    if (deleteDropdownOpen)
    {
        for (auto& btn : deleteDropdownButtons)
            btn->Update(dt);
    }

    searchButton->Update(dt);
    if (searchDropdownOpen)
    {
        for (auto& btn : searchDropdownButtons)
            btn->Update(dt);
    }

    otherButton->Update(dt);
    if (otherDropdownOpen)
    {
        for (auto& btn : otherDropdownButtons)
            btn->Update(dt);
    }

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
    if (showInsertAtHead && insertDropdownOpen)
        DrawInputBox2(280, 655, inputInsertAtHead);

    if (showInsertAtTail && insertDropdownOpen)
        DrawInputBox2(460, 655, inputInsertAtTail);

    if (showInsertAtIndexInput && insertDropdownOpen)
    {
        DrawInputBox(643, 655, inputValue);
        DrawInputBox(732, 655, inputIndex);

        DrawText("Value", 643, 715, 20, YELLOW);
        DrawText("Index", 732, 715, 20, YELLOW);
    }

    if (showDeleteAtIndexInput && deleteDropdownOpen)
        DrawInputBox2(640, 710, inputDeleteIndex);

    if (showRandomInput)
        DrawInputBox2(280, 820, inputRandom);

    if (showSearchInput && searchByValue)
        DrawInputBox2(280, 765, searchByValInput);
    
    if (showSearchInput && !searchByValue)
        DrawInputBox2(460, 765, searchByIndInput);
     
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
    repositionButton->Render();

    insertButton->Render();
    if (insertDropdownOpen)
    {
        for (auto& btn : insertDropdownButtons)
            btn->Render();
    }

    deleteButton->Render();
    if (deleteDropdownOpen)
    {
        for (auto& btn: deleteDropdownButtons)
            btn->Render();
    }

    searchButton->Render();
    if (searchDropdownOpen)
    {
        for (auto& btn: searchDropdownButtons)
            btn->Render();
    }

    otherButton->Render();
    if (otherDropdownOpen)
    {
        for (auto& btn: otherDropdownButtons)
            btn->Render();
    }

    undoButton->Render();
    redoButton->Render(); 
    returnButton->Render();
}