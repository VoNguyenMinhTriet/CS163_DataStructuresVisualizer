#include "InputBox.hpp"
#include <cstring>
#include <algorithm>
#include <cassert>

namespace raywtk {
    InputBox::InputBox(raylib::Rectangle rect, raylib::Color textColor, raylib::Color bgColor, raylib::Color borderColor, int number, bool processing, bool decimalFlag)
        : rect(rect), textColor(textColor), bgColor(bgColor), borderColor(borderColor), number(number), processing(processing), decimalFlag(decimalFlag) {
        text[0] = '\0';
        values.clear();
    }

    void InputBox::Reset(){
        text[0] = '\0';
        values.clear();
    }

    std::vector<float> InputBox::GetValues(){
        return values;
    }

    void InputBox::Update(float dt) {
        int key = GetCharPressed();
        while (key > 0) {
            bool getNewChar = false;

            if (key == (int)' ') {
                if (std::count(text, text + strlen(text), ' ') + 1 < number) {
                    getNewChar = true;
                }
            } else if (key == '.' && decimalFlag) {
                if (std::count(text, text + strlen(text), '.') == 0 && strlen(text) < 127) {
                    getNewChar = true;
                }
            } else {
                if ((key >= '0') && (key <= '9') && (strlen(text) < 127)) {
                    getNewChar = true;
                }
            }

            if (getNewChar) {
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

        if (IsKeyPressed(KEY_ENTER) && std::count(text, text + strlen(text), ' ') == number - 1) {
            //values.push_back(0);
            std::string currentNumber;
            for (size_t i = 0; text[i] != '\0'; i++) {
                if (text[i] != ' ') {
                    currentNumber += text[i];
                } else {
                    if (decimalFlag) {
                        values.push_back(std::stof(currentNumber)); // Convert to float if decimalFlag is true
                    } else {
                        values.push_back(std::stoi(currentNumber)); // Convert to int otherwise
                    }
                    currentNumber.clear();
                }
            }
            if (!currentNumber.empty()) {
                if (decimalFlag) {
                    values.push_back(std::stof(currentNumber));
                } else {
                    values.push_back(std::stoi(currentNumber));
                }
            }
            assert(values.size() == number);
            processing = 0;
        }
    }
    

    void InputBox::Render() {
        if(processing){
            rect.DrawRoundedLines(0.3f, 10, PINK);
            raylib::DrawText(text, (int)(rect.x + 10), int(rect.y + (rect.height - 16) / 2.0), 16, textColor);
        }
    }

    const char* InputBox::GetText() const {
        return text;
    }
}