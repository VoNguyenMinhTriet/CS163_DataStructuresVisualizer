#pragma once

namespace raywtk
{
    class IRenderable
    {
        public:
        virtual void Render () = 0;
    };

    class IUpdateable
    {
        public:
        virtual void Update (float deltaTime) = 0;
    };

    class IScreenElement : public IRenderable, public IUpdateable { };

    class IStyle
    {
        public:
        virtual void RenderWithStyle (IRenderable *self) = 0;
    };
}
