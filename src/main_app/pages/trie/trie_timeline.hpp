#pragma once

#include <functional>
#include <list>
#include <memory>
#include <optional>
#include "raylib-cpp/raylib-cpp.hpp"
#include "./trie_scene.hpp"

namespace ds_viz::pages::trie 
{
    struct TrieNode;
    class TrieScene;

    class ITimeline
    {
    public:
        virtual void RenderCurrentState (TrieScene &scene) = 0;
        virtual void StepForward () = 0;
        virtual void StepBackward () = 0;
        virtual void ApplyTimeline (TrieScene &scene) = 0;
    };

    class SearchTimeline : public ITimeline
    {
        class IAction 
        { 
        public:
            virtual ~IAction () = default;
            virtual void Do (SearchTimeline& timeline) = 0;
            virtual void Undo (SearchTimeline& timeline) = 0;
        };

        template<typename T> class SetVariableStep : public IAction 
        {
            T &_variable;
            T _previousValue;
            std::function<T()> _newValueExpression;
        public:
            SetVariableStep(T &variable, std::function<T()> newValueExpression)
                : _variable(variable), _newValueExpression(newValueExpression) {
                _previousValue = variable;
            }

            void Do (SearchTimeline& timeline) override 
            {
                _previousValue = _variable;
                _variable = _newValueExpression();
            }

            void Undo (SearchTimeline& timeline) override 
            {
                _variable = _previousValue;
            }
        };

    private:
        raylib::Color nodeDefaultColor = raylib::Color::White();
        raylib::Color nodeHighlightColor = raylib::Color::Yellow();
        raylib::Color edgeHighlightColor = raylib::Color::Yellow();
        raylib::Color nodeReturnColor = raylib::Color::Green();

        std::list<std::unique_ptr<IAction>> stepTimeline;
        std::list<std::unique_ptr<IAction>>::iterator currentStep;

        TrieScene &scene;

        TrieNode *var_curNode = nullptr;
        std::optional<bool> returnValue = std::nullopt;

    public:
        SearchTimeline(TrieScene &scene, const std::string &key);

        void RenderCurrentState(TrieScene &scene) override;

        void RenderUtil(TrieNode *node);

        void StepForward() override;

        void StepBackward() override;

        void ApplyTimeline(TrieScene &scene) override;
    };
}