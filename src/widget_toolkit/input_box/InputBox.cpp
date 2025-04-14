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

    void InputBox::Render() {
        if(processing){
            DrawRectangleRec(rect, bgColor);
            DrawRectangleLinesEx(rect, 2, borderColor);
            raylib::DrawText(text, (int)(rect.x + 10), (int)(rect.y + 10), 20, textColor);
        }
    }

    const char* InputBox::GetText() const {
        return text;
    }
}