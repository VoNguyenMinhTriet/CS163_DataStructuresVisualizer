#pragma once

#include "./interfaces.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace raywtk
{
    class Theme
    {
        public:
        // This form of IStyle resolution will not work if we want to
        // reuse the style for derived controls, e.g. ToggleButton.
        // Additionally, multi-inheritance and C++'s reflection sucks.
        std::unordered_map<std::string, std::unique_ptr<IStyle>> styles;

        IStyle &GetStyle (const char *controlID)
        {
            return *styles.at(controlID);
        }
    };

    class ThemeManager
    {
        private:
        public:
        inline static std::unique_ptr<Theme> currentTheme;

        static Theme &GetCurrentTheme ()
        {
            return *currentTheme;
        }
    };
}
