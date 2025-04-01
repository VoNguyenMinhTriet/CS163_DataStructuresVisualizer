#include "./LinkedList.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>

ds_viz::pages::LinkedListPage::LinkedListPage() 
{
    font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
    title = raylib::Text("Singly-Linked List", 128, raylib::Color::White(), *font, 0);
    
    // insert button
    insertAtTail = std::make_unique<raywtk::Button>();
    insertAtTail->buttonText = "Insert At Tail";
    insertAtTail->buttonRect = raylib::Rectangle(100, 800, 150, 50);
    insertAtTail->Click.append([this]() { showInputBar = true; });
    insertAtTail->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    
    // random create button
    randomButton = std::make_unique<raywtk::Button>();
    randomButton->buttonText = "Create Random";
    randomButton->buttonRect = raylib::Rectangle(100, 700, 150, 50);
    randomButton->Click.append([this]() { OnRandomButtonClick(); });
    randomButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // clear all button
    clearAllButton = std::make_unique<raywtk::Button>();
    clearAllButton->buttonText = "Clear All";
    clearAllButton->buttonRect = raylib::Rectangle(100, 900, 150, 50);
    clearAllButton->Click.append([this]() { OnClearButtonClick(); });
    clearAllButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Reposition Nodes Button
    repositionButton = std::make_unique<raywtk::Button>();
    repositionButton->buttonText = "Reposition";
    repositionButton->buttonRect = raylib::Rectangle(300, 900, 180, 50);
    repositionButton->Click.append([this]() { RepositionNodes(); });
    repositionButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Search by Value Button
    searchByValueButton = std::make_unique<raywtk::Button>();
    searchByValueButton->buttonText = "Search by Value";
    searchByValueButton->buttonRect = raylib::Rectangle(300, 700, 180, 50);
    searchByValueButton->Click.append([this]() { searchByValue = true; showSearchInput = true; });
    searchByValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Search by Index Button
    searchByIndexButton = std::make_unique<raywtk::Button>();
    searchByIndexButton->buttonText = "Search by Index";
    searchByIndexButton->buttonRect = raylib::Rectangle(300, 800, 180, 50);
    searchByIndexButton->Click.append([this]() { searchByValue = false; showSearchInput = true; });
    searchByIndexButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // return button
    returnButton = std::make_unique<raywtk::Button>();
    returnButton->buttonRect = raylib::Rectangle(1700, 800, 80, 80); 
    returnButton->Click.append([this]() { OnReturnButtonClick(); });
    returnButton->style = std::make_unique<ds_viz::themes::dark_simple::BackButtonStyle>();
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
void ds_viz::pages::LinkedListPage::OnRandomButtonClick()
{
    showRandomInput = true; 
    randomInputValue.clear(); 
}

// Clear All List
void ds_viz::pages::LinkedListPage::OnClearButtonClick()
{
    head.reset(); 
    size = 0;
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


// Insert a new node at the tail
void ds_viz::pages::LinkedListPage::InsertAtTail(int value)
{
    ResetColor();

    auto newNode = std::make_unique<raywtk::NodeWidget>(value);
    
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
    searchTimer = 0.5f; // Delay per step

    found = false;

    while (searchCurrent)
    {
        searchCurrent->color = raylib::Color::Orange(); // Highlight step
        WaitTime(searchTimer); // Pause for animation

        if (searchCurrent->value == val) // Check value
        {
            searchCurrent->color = raylib::Color::Green(); // Found!
            found = true;
            break;
        }

        searchCurrent->color = raylib::Color::Blue(); // Reset color
        searchCurrent = searchCurrent->next.get();
    }

    if (!found)
    {
        errorMessage = "Value not found!";
        errorTimer = 2.0f;
    }
}

void ds_viz::pages::LinkedListPage::SearchByIndex(int index)
{   
    ResetColor();
    if (index < 0 || index >= size)
    {
        errorMessage = "Index out of bounds!";
        errorTimer = 2.0f;
        return;
    }

    if (!head)
    {
        errorMessage = "List is empty!";
        errorTimer = 2.0f;
        return;
    }

    animatingSearch = true;
    searchCurrent = head.get();
    searchIndex = index;
    currentIndex = 0;
    searchTimer = 0.5f;

    while (searchCurrent)
    {
        searchCurrent->color = raylib::Color::Orange(); // Highlight step
        WaitTime(searchTimer);

        if (currentIndex == searchIndex)
        {
            searchCurrent->color = raylib::Color::Green(); // Found!
            return;
        }

        searchCurrent->color = raylib::Color::Blue();
        searchCurrent = searchCurrent->next.get();
        currentIndex++;
    }
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

    if (showInputBar)
    {
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9') || (key == '-' && inputValue.empty())) 
            inputValue += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !inputValue.empty())
            inputValue.pop_back();
        else if (key == KEY_ENTER && !inputValue.empty())
        {
            try 
            {
                int newValue = std::stoi(inputValue);

                if (newValue < -999 || newValue > 999) 
                {
                    errorMessage = "Invalid input! Value must be between -999 and 999.";
                    errorTimer = 2.0f; // Show error for 2 seconds
                }
                
                else if (size>=18) 
                {
                    errorMessage = "Maximum number of nodes is 18!";
                    errorTimer = 2.0f;
                }
                
                else
                    InsertAtTail(newValue);
            }
            
            catch (const std::exception& e) 
            {
                errorMessage = "Invalid input! Please enter a valid integer.";
                errorTimer = 2.0f;
            }

            inputValue.clear();
            showInputBar = false;
        }
    }

    if (showRandomInput)
    {  
        int key = GetKeyPressed();
        if (key >= '0' && key <= '9') 
            randomInputValue += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !randomInputValue.empty()) 
            randomInputValue.pop_back();
        else if (key == KEY_ENTER && !randomInputValue.empty()) 
        {
            try 
            {
                int numNodes = std::stoi(randomInputValue);
            
                if (numNodes < 1 || numNodes > 18) 
                {
                    errorMessage = "Maximum number of nodes is 18!";
                    errorTimer = 2.0f;
                }
                
                else 
                {
                    OnClearButtonClick(); 
                    srand(time(nullptr)); // Start Generator

                    for (int i = 0; i < numNodes; i++)
                    {
                        int randomValue = rand() % 1999 - 999; 
                        InsertAtTail(randomValue);
                    }
                }
            }
            
            catch (...)
            {
                errorMessage = "Invalid input! Please enter a valid integer between 1 and 18!";
                errorTimer = 2.0f;
            }

            showRandomInput = false; // Hide input box after processing
        }
    }

    if (showSearchInput)
    {
        int key = GetKeyPressed();
        if ((key >= '0' && key <= '9') || (key == '-' && searchInput.empty())) 
            searchInput += static_cast<char>(key);
        else if (key == KEY_BACKSPACE && !searchInput.empty()) 
            searchInput.pop_back();
        else if (key == KEY_ENTER && !searchInput.empty()) 
        {
            try 
            {
                int searchValue = std::stoi(searchInput);
                if (searchByValue) 
                    SearchByValue(searchValue);
                else 
                    SearchByIndex(searchValue);
            } 
            catch (...) 
            {
                errorMessage = "Invalid input!";
                errorTimer = 2.0f;
            }

            searchInput.clear();
            showSearchInput = false;
        }
    }

    if (animatingSearch && searchCurrent)
    {
        searchTimer -= dt;
        if (searchTimer <= 0)
        {
            searchTimer = 0.5f; // Reset delay

            // Highlight the current node in orange
            searchCurrent->color = raylib::Color::Orange();

            if (searchByValue)
            {
                if (searchCurrent->value == searchTarget)
                {
                    searchCurrent->color = raylib::Color::Green(); // Found!
                    animatingSearch = false;
                    searchCurrent = nullptr;
                    return;
                }
            }

            else
            {
                if (currentIndex == searchIndex)
                {
                    searchCurrent->color = raylib::Color::Green(); // Found!
                    animatingSearch = false;
                    searchCurrent = nullptr;
                    return;
                }
            }

            // Move to next node
            searchCurrent->color = raylib::Color::Blue(); // Reset color to normal
            searchCurrent = searchCurrent->next.get();
            currentIndex++;

            // If the value is not found
            if (!found)
            {
                if (searchByValue)
                {
                    errorMessage = "Value not found!";
                    errorTimer = 2.0f;
                }
                
                animatingSearch = false;
            }
        }
    }

    insertAtTail->Update(dt);
    randomButton->Update(dt);
    clearAllButton->Update(dt);
    repositionButton->Update(dt);
    searchByValueButton->Update(dt);
    searchByIndexButton->Update(dt);
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

    // Draw input bar if active
    if (showInputBar)
    {
        DrawRectangle(100, 850, 150, 50, DARKGRAY);
        DrawText(inputValue.c_str(), 120, 865, 20, RAYWHITE);
    }

    // Draw random generator bar if active
    if (showRandomInput)
    {
        DrawRectangle(100, 750, 150, 50, DARKGRAY);
        DrawText(randomInputValue.c_str(), 120, 765, 20, RAYWHITE);
    }

    if (showSearchInput && searchByValue)
    {
        DrawRectangle(300, 750, 180, 50, DARKGRAY);
        DrawText(searchInput.c_str(), 320, 765, 20, RAYWHITE);
    }
    
    if (showSearchInput && !searchByValue)
    {
        DrawRectangle(300, 850, 180, 50, DARKGRAY);
        DrawText(searchInput.c_str(), 320, 865, 20, RAYWHITE);
    }
     
    // Draw error message if needed
    if (errorTimer > 0)
    {
        int screenWidth = _context->ref_raylib_window->GetWidth();
        int screenHeight = _context->ref_raylib_window->GetHeight();

        int boxWidth = 600;
        int boxHeight = 40;
        int boxX = (screenWidth - boxWidth) / 2;
        int boxY = screenHeight - 100;

        int fontSize = 20;
        int textWidth = MeasureText(errorMessage.c_str(), fontSize);
    
        // Draw background box
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, RED);

        // Adjust text position to center inside the box
        int textX = boxX + (boxWidth - textWidth) / 2;
        int textY = boxY + (boxHeight - fontSize) / 2;

        DrawText(errorMessage.c_str(), textX, textY, fontSize, RAYWHITE);
    }

    // Render buttons
    insertAtTail->Render();
    randomButton->Render();
    clearAllButton->Render();
    repositionButton->Render();
    searchByValueButton->Render();
    searchByIndexButton->Render(); 
    returnButton->Render();
}