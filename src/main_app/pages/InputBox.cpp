#include "InputBox.hpp"
#include <cstring>
#include <algorithm>

namespace raywtk {
    InputBox::InputBox(raylib::Rectangle rect, int fontSize, raylib::Color textColor, raylib::Color bgColor, raylib::Color borderColor, int spaceLimit)
        : rect(rect), fontSize(fontSize), textColor(textColor), bgColor(bgColor), borderColor(borderColor), focused(false), spaceLimit(spaceLimit) {
        text[0] = '\0';
    }

    void InputBox::Update(float dt) {
        if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
            focused = raylib::Mouse::GetPosition().CheckCollision(rect);
        }

        if (focused) {
            int key = GetCharPressed();
            while (key > 0) {
                bool getNewChar = false;
                if(key == (int)' '){
                    if(std::count(text, text + strlen(text), ' ') + 1 <= spaceLimit){
                        getNewChar = 1;
                    }
                }
                if ((key >= 32) && (key <= 125) && (strlen(text) < 127)) {
                    getNewChar = 1;
                }
                if(getNewChar){
                    text[strlen(text)] = (char)key;
                    text[strlen(text) + 1] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (strlen(text) > 0) {
                    text[strlen(text) - 1] = '\0';
                }
            }
            if(IsKeyPressed(KEY_ENTER)){
                return;
            }
        }
    }

    void InputBox::Render() {
        DrawRectangleRec(rect, bgColor);
        DrawRectangleLinesEx(rect, 2, borderColor);
        raylib::DrawText(text, (int)(rect.x + 10), (int)(rect.y + 10), 20, textColor);
    }

    const char* InputBox::GetText() const {
        return text;
    }
}