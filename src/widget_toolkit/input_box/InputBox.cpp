#include "InputBox.hpp"
#include <cstring>
#include <algorithm>

namespace raywtk {
    InputBox::InputBox(raylib::Rectangle rect, raylib::Color textColor, raylib::Color bgColor, raylib::Color borderColor, int number, bool processing)
        : rect(rect), textColor(textColor), bgColor(bgColor), borderColor(borderColor), number(number), processing(processing) 
    {
        text[0] = '\0';
        values.clear();
    }

    void InputBox::Reset()
    {
        processing = 0;
        text[0] = '\0';
        values.clear();
    }

    std::vector<int> InputBox::GetValues()
    {
        return values;
    }

    void InputBox::Update(float dt) {
        int key = GetCharPressed();
        while (key > 0 && (strlen(text) < 127)) {
            bool getNewChar = false;
            if(strlen(text) > 0 && key == int(' '))
            {
                if(std::count(text, text + strlen(text), ' ') + 1 < number && text[strlen(text) - 1] != ' ')
                {
                    getNewChar = true;
                }
            }

            bool check = (key == '0' && (strlen(text) > 1 && text[strlen(text) - 1] != ' ' && text[strlen(text) - 2] != ' ' || strlen(text) == 0 || text[strlen(text) - 1] != '0' && text[strlen(text) - 1] != '-'));
            check |= (strlen(text) == 0 || text[strlen(text) - 1] != '0' || strlen(text) > 1 && text[strlen(text) - 1] != ' ' && text[strlen(text) - 2] != ' ') && (key >= '1') && (key <= '9');
            if (check || (key == '-' && (strlen(text) == 0 || text[strlen(text) - 1] == ' ')))
            {
                getNewChar = true;
            }
            
            if(getNewChar)
            {
                text[strlen(text) + 1] = '\0';
                text[strlen(text)] = (char)key;
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) 
        {
            if (strlen(text) > 0) 
            {
                text[strlen(text) - 1] = '\0';
            }
        }

        if(IsKeyPressed(KEY_ENTER))
        {
            values.push_back(0);

            int sign = 1;
            for (int i = 0; text[i] != '\0'; ++i) 
            {
                if(text[i] == '-')
                {
                    continue;
                }

                if(text[i] != ' ') 
                {
                    values.back() = values.back() * 10 + (text[i] - '0') * sign;
                    if(i > 0 && text[i - 1] == '-') {
                        sign = -1;
                        values.back() *= -1;
                    }
                } else 
                {
                    sign = 1;
                    values.push_back(0);
                }
            }

            if(values.back() == 0 && (strlen(text) == 0 || text[strlen(text) - 1] == ' '))
                values.pop_back();

            processing = false;
            return;
        }
    }

    void InputBox::Render() 
    {
        if(processing) 
        {
            DrawRectangleRec(rect, bgColor);
            DrawRectangleLinesEx(rect, 2, borderColor);
            raylib::DrawText(text, (int)(rect.x + 10), int(rect.y + (rect.height - 16) / 2.0), 16, textColor);
        }
    }

    const char* InputBox::GetText() const 
    {
        return text;
    }
}