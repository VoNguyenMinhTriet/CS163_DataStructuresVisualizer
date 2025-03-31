#include "./LinkedList.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <iostream>


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
    
    // return button
    returnButton = std::make_unique<raywtk::Button>();
    returnButton->buttonRect = raylib::Rectangle(1700, 800, 80, 80); // Position and size
    returnButton->Click.append([this]() { OnReturnButtonClick(); });
    returnButton->style = std::make_unique<ds_viz::themes::dark_simple::BackButtonStyle>();
}


// Handle return button click event
void ds_viz::pages::LinkedListPage::OnReturnButtonClick()
{
    Nodes.clear();

    if (_context)
        _context->ChangePage(std::make_shared<ds_viz::pages::MainMenuPage>());
    else
        std::cerr<<"Error"<<std::endl;
}

// Insert a new node at the tail
void ds_viz::pages::LinkedListPage::InsertNode(int value)
{
    float startX = 100;  // Initial X position
    float startY = 400;  // Fixed Y position

    // Compute new node position (each node is spaced 100px apart)
    float posX = startX + Nodes.size() * 100;

    auto newNode = std::make_unique<raywtk::NodeWidget>();
    newNode->position = raylib::Vector2(posX, startY);
    newNode->color = DARKBLUE;

    // Set content to display value inside node
    newNode->content = [newNode = newNode.get(), value]()
    {
        std::string text = std::to_string(value);
        int fontSize = 20;
        int textWidth = MeasureText(text.c_str(), fontSize); // Measure text width
        int textHeight = fontSize;  // Font height is usually close to font size

        // Adjust to center inside the node
        float centeredX = newNode->position.x - (textWidth / 2);
        float centeredY = newNode->position.y - (textHeight / 2);

        DrawText(text.c_str(), centeredX, centeredY, fontSize, RAYWHITE);
    };

    Nodes.push_back(std::move(newNode));
}

void ds_viz::pages::LinkedListPage::Update(float dt)
{
    
    for (auto& node : Nodes)
        node->Update(dt);

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
                
                else if (Nodes.size() >= 18) 
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

    insertAtTail->Update(dt);
    returnButton->Update(dt);
}

void ds_viz::pages::LinkedListPage::Render()
{
    // Draw title
    title.Draw(raylib::Vector2(
        (_context->ref_raylib_window->GetWidth() - title.MeasureEx().x) / 2.0,
        0));

        for (size_t i = 0; i < Nodes.size(); i++)
        {
            Nodes[i]->Render();
        
            if (i < Nodes.size() - 1) // Draw connections except for the last node
            {
                raylib::Vector2 start = Nodes[i]->position;
                raylib::Vector2 end = Nodes[i + 1]->position;
        
                // Compute direction vector from start to end
                raylib::Vector2 direction = (end - start).Normalize();
        
                // Get perimeter intersection points
                raylib::Vector2 startEdge = start + (direction * Nodes[i]->radius);
                raylib::Vector2 endEdge = end - (direction * Nodes[i + 1]->radius);
        
                // Draw the adjusted line
                DrawLineEx(startEdge, endEdge, 5, RAYWHITE);
        
                // === Draw Arrowhead ===
                float arrowSize = 12.0f;
                float arrowAngle = 30.0f * DEG2RAD; // Angle of arrowhead
                
                raylib::Vector2 leftWing = endEdge - direction * arrowSize +
                raylib::Vector2(cos(arrowAngle) * -direction.x - sin(arrowAngle) * -direction.y,
                    sin(arrowAngle) * -direction.x + cos(arrowAngle) * -direction.y) * arrowSize;

                raylib::Vector2 rightWing = endEdge - direction * arrowSize +
                raylib::Vector2(cos(-arrowAngle) * -direction.x - sin(-arrowAngle) * -direction.y,
                    sin(-arrowAngle) * -direction.x + cos(-arrowAngle) * -direction.y) * arrowSize;

                // Draw arrowhead as a triangle
                DrawTriangle(endEdge, leftWing, rightWing, RAYWHITE);
            }
        }

    // Draw Head and Tail labels
    if (!Nodes.empty())
    {
        DrawText("head", Nodes.front()->position.x - 30, Nodes.front()->position.y - 50, 20, RED);
        DrawText("tail", Nodes.back()->position.x - 20, Nodes.back()->position.y + 50, 20, GREEN);
    }

    if (showInputBar)
    {
        DrawRectangle(100, 850, 150, 50, DARKGRAY);
        DrawText(inputValue.c_str(), 120, 865, 20, RAYWHITE);
    }

    insertAtTail->Render();
    returnButton->Render();

    if (errorTimer > 0)
    {
    int screenWidth = _context->ref_raylib_window->GetWidth();
    int screenHeight = _context->ref_raylib_window->GetHeight();

    int boxWidth = 600;  // Increased width
    int boxHeight = 40;
    int boxX = (screenWidth - boxWidth) / 2;  // Center horizontally
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