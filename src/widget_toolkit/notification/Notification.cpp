#include "Notification.hpp"
#include <algorithm> // For std::remove_if

namespace raywtk
{

    Notification::Notification(const std::string& message, NotificationType type, int x, int y)
        : message(message), type(type), x(x), y(y)
    {
    }

    void Notification::Update(float deltaTime)
    {
    }

    void Notification::Render()
    {
        DrawText(message.c_str(), x + 10, y + 10, 20, GetColor(type));
    }

    Color Notification::GetColor(NotificationType type)
    {
        switch (type)
        {
        case NotificationType::SUCCESS: return GREEN;
        case NotificationType::ERROR: return RED;
        case NotificationType::INFO: return BLUE;
        default: return WHITE;
        }
    }
}