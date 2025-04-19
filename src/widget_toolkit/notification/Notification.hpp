#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/interfaces.hpp"
#include <string>
#include <vector>
#include <chrono>

namespace raywtk
{
    enum class NotificationType
    {
        SUCCESS,
        ERROR,
        INFO
    };

    class Notification : public IScreenElement
    {
    public:
        Notification(const std::string& message, NotificationType type, int x, int y);

        // Implementations of IRenderable and IUpdateable
        void Render() override;
        void Update(float deltaTime) override;

        void show(const std::string& message, NotificationType type, int x, int y);

    private:
        std::string message;
        NotificationType type;
        int x, y;

        void drawNotification();
        Color GetColor(NotificationType type);
    };
}

