#pragma once
#include "widget_toolkit/interfaces.hpp"

namespace ds_viz
{
    class MainWindow;

    // Implemented as a state in the State pattern.
    class Page : public raywtk::IScreenElement
    {
        protected:
        MainWindow *_context;

        public:
        Page (MainWindow &context) : _context(&context) { }
        void SetContext (MainWindow *context) { _context = context; }

        virtual ~Page() { }
    };
}