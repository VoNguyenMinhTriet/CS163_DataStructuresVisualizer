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

// Insert a new node at the tail
void ds_viz::pages::LinkedListPage::InsertNode(int value)
{
    float startX = 100;  // Initial X position
    float startY = 400;  // Fixed Y position

    auto newNode = std::make_unique<raywtk::NodeWidget>(value);
    
    // Compute position
    if (!head) 
    {
        newNode->position = raylib::Vector2(startX, startY);
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
        float posX = startX + (index + 1) * 100;
        newNode->position = raylib::Vector2(posX, startY);

        current->next = std::move(newNode);
    }

    size++;
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
                    InsertNode(newValue);
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
                        InsertNode(randomValue);
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

    insertAtTail->Update(dt);
    randomButton->Update(dt);
    clearAllButton->Update(dt);
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

    // Render buttons
    insertAtTail->Render();
    randomButton->Render();
    clearAllButton->Render();
    returnButton->Render();

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
}