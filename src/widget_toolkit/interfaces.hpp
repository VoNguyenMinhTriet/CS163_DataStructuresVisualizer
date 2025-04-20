#pragma once

namespace raywtk
{
    class IRenderable
    {
        public:
        virtual ~IRenderable () = default;
        virtual void Render () = 0;
    };

    class IUpdateable
    {
        public:
        virtual ~IUpdateable () = default;
        virtual void Update (float deltaTime) = 0;
    };

    class IScreenElement : public IRenderable, public IUpdateable { };

    class IStyle
    {
        public:
        virtual ~IStyle () = default;
        virtual void RenderWithStyle (IRenderable *self) = 0;
    };
}
