#include "InputBox.hpp"
#include <cstring>
#include <algorithm>
#include <cassert>

namespace raywtk {
    InputBox::InputBox(raylib::Rectangle rect, raylib::Color textColor, raylib::Color bgColor, raylib::Color borderColor, int number, bool processing)
        : rect(rect), textColor(textColor), bgColor(bgColor), borderColor(borderColor), number(number), processing(processing) {
        text[0] = '\0';
        values.clear();
    }

    void InputBox::Reset(){
        text[0] = '\0';
        values.clear();
    }

    std::vector<int> InputBox::GetValues(){
        return values;
    }

    void InputBox::Update(float dt) {
        int key = GetCharPressed();
        while (key > 0) {
            bool getNewChar = false;
            if(key == (int)' '){
                if(std::count(text, text + strlen(text), ' ') + 1 < number){
                    getNewChar = true;
                }
            }
            else{
                if ((key >= '0') && (key <= '9') && (strlen(text) < 127)) {
                    getNewChar = true;
                }
            }
            if(getNewChar){
                int x = strlen(text);
                int k = strlen(text);
                text[k] = (char)key;
                text[k + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (strlen(text) > 0) {
                text[strlen(text) - 1] = '\0';
            }
        }
        if(IsKeyPressed(KEY_ENTER) && std::count(text, text + strlen(text), ' ') == number - 1){
            values.push_back(0);
            for(size_t i = 0; text[i] != '\0'; i++){
                if(text[i] != ' ') values.back() = values.back() * 10 + text[i] - '0';
                else values.push_back(0);
            }
            assert(values.size() == number);
            processing = 0;
        }
    }

    void DrawRoundedRectangleBorder(Rectangle rec, float roundness, int segments, int lineThickness, Color color) {
        // Calculate the radius for the rounded corners
        float radius = roundness * fminf(rec.width, rec.height) / 2.0f;
    
        // Draw the straight edges of the rectangle
        DrawRectangle(rec.x + radius, rec.y, rec.width - 2 * radius, lineThickness, color); // Top edge
        DrawRectangle(rec.x + radius, rec.y + rec.height - lineThickness, rec.width - 2 * radius, lineThickness, color); // Bottom edge
        DrawRectangle(rec.x, rec.y + radius, lineThickness, rec.height - 2 * radius, color); // Left edge
        DrawRectangle(rec.x + rec.width - lineThickness, rec.y + radius, lineThickness, rec.height - 2 * radius, color); // Right edge
    
        // Draw the rounded corners
        DrawCircleSector(
            {rec.x + radius, rec.y + radius}, // Top-left corner
            radius,
            180,
            270,
            segments,
            color
        );
        DrawCircleSector(
            {rec.x + rec.width - radius, rec.y + radius}, // Top-right corner
            radius,
            270,
            360,
            segments,
            color
        );
        DrawCircleSector(
            {rec.x + radius, rec.y + rec.height - radius}, // Bottom-left corner
            radius,
            90,
            180,
            segments,
            color
        );
        DrawCircleSector(
            {rec.x + rec.width - radius, rec.y + rec.height - radius}, // Bottom-right corner
            radius,
            0,
            90,
            segments,
            color
        );
    }
    

    void InputBox::Render() {
        if(processing){
            //DrawRectangleRec(rect, bgColor);
            DrawRoundedRectangleBorder(rect, 0.3f, 10, 3, borderColor);
            raylib::DrawText(text, (int)(rect.x + 10), (int)(rect.y + 10), 20, textColor);
        }
    }

    const char* InputBox::GetText() const {
        return text;
    }
}